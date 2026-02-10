#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"

#subpass [VolumetricsPass]

#attribute [Culling] [true]

#vertex

#include "sg_shaders/impl/glsl4/instancing.glsl"

layout (location = SG_VS_POSITIONS_ATTRIBUTE_LOC) in vec3 positionsAttribute;
layout (location = SG_VS_UV_ATTRIBUTE_LOC) in vec3 UVAttribute;
layout (location = SG_VS_NORMALS_ATTRIBUTE_LOC) in vec3 normalsAttribute;
layout (location = SG_VS_TANGENTS_ATTRIBUTE_LOC) in vec3 tangentsAttribute;
layout (location = SG_VS_BITANGENTS_ATTRIBUTE_LOC) in vec3 bitangentsAttribute;
#if SG_VS_VERTEX_COLOR_ATTRIBUTE0_LOC < SG_VS_MAX_ATTRIBS_COUNT
layout (location = SG_VS_VERTEX_COLOR_ATTRIBUTE0_LOC) in vec4 vertexColor0Attribute;
#endif
#if SG_VS_VERTEX_COLOR_ATTRIBUTE1_LOC < SG_VS_MAX_ATTRIBS_COUNT
layout (location = SG_VS_VERTEX_COLOR_ATTRIBUTE1_LOC) in vec4 vertexColor1Attribute;
#endif

out VSOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;
    mat4 invModelMatrix;
} vsOut;

#include "sg_shaders/impl/glsl4/vs_attribs_utils.glsl"

void main()
{
    mat4 instanceModelMatrix = getCurrentInstanceModelMatrix();

    vsOut.UV = UVAttribute.xy;
    vsOut.normal = normalize(normalsAttribute);
    vsOut.worldNormal = normalize(mat3(transpose(inverse(instanceModelMatrix))) * vsOut.normal);

    vsOut.vertexPos = positionsAttribute.xyz;
    vsOut.fragPos = vec3(instanceModelMatrix * vec4(positionsAttribute, 1.0));

    vec3 T = normalize(vec3(instanceModelMatrix * vec4(tangentsAttribute, 0.0)));
    vec3 B = normalize(vec3(instanceModelMatrix * vec4(bitangentsAttribute, 0.0)));
    vec3 N = normalize(vec3(instanceModelMatrix * vec4(vsOut.normal, 0.0)));
    vsOut.TBN = mat3(T, B, N);

    vsOut.invModelMatrix = inverse(instanceModelMatrix);

    gl_Position = camera.projectionSpaceMatrix * vec4(vsOut.fragPos, 1.0);
}

#end

#fragment

// REQUIRED COLORS!!! ===========
layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;
// COLOR FOR STOCHASTIC TRANSPARNCY
layout(location = 3) out vec4 layerSTColor;
// REQUIRED COLORS!!! ===========

#include "sg_shaders/impl/glsl4/math.glsl"
#include "sg_shaders/impl/glsl4/pbr_base.glsl"
#include "sg_shaders/impl/glsl4/color_correction/aces.glsl"
#include "sg_shaders/impl/glsl4/shadows_sampling/csm.glsl"
#include "pbr/lighting_calc.glsl"

in VSOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;
    mat4 invModelMatrix;
} vsIn;

// REQUIRED UNIFORM!!
uniform vec3 u_pickingColor;

uniform int SGPP_CurrentLayerIndex;

// Noises.
// mat_noiseSamplers[0] - low frequency noise
// mat_noiseSamplers[1] - high frequency noise
uniform sampler3D mat_noiseSamplers[2];
uniform int mat_noiseSamplers_CURRENT_COUNT;

// Weather Map.
// R-channel — cloud coverage at low altitude
// G-channel — cloud coverage at high altitude
// B-channel — maximum cloud height
// A-channel — cloud density
uniform sampler2D mat_baseColorSamplers[1];
uniform int mat_baseColorSamplers_CURRENT_COUNT;

float u_sgVolumetricCoverage = 2.0;
float u_sgVolumetricDensity = 0.1;

// uniform float u_sgVolumetricCoverage;
// uniform float u_sgVolumetricDensity;
uniform vec3 u_sgMeshAABBMin;
uniform vec3 u_sgMeshAABBMax;

float detailScale = 10.0;
float detailStrength = 10.0;

float remap(float value, float originalMin, float originalMax, float newMin, float newMax)
{
    return newMin + (value - originalMin) / (originalMax - originalMin) * (newMax - newMin);
}

float sdfBox(vec3 p, vec3 boxCenter, vec3 halfSize)
{
    vec3 d = abs(p - boxCenter) - vec3(halfSize.x, halfSize.y, halfSize.z);
    return length(max(d, 0.0)) + min(max(d.x, max(d.y, d.z)), 0.0);
}

vec3 ambientColor = vec3(0.0);

vec3 cloudScrolling = vec3(50.0);
float perlinScale = 0.00001;
float worleyScale = 0.00005;

float scatteringRatio = 0.01;
// float scatteringCoefficient = 0.1;
float scatteringCoefficient = 0.4;

float anisotropy = 0.85;

float absorptionCoefficient = 0.0005;

vec3 cloudColor = vec3(1.0, 0.95, 0.9);

float henyeyGreenstein(float cosAngle, float g)
{
    float g2 = g * g;
    return ((1.0f - g2) / pow(1.0f + g2 - 2.0f * g * cosAngle, 1.5f)) / (4.0f * 3.1415f);
}

float combinedPhaseFunction(float cosAngle, float gHG, float ratio)
{
    float hg = henyeyGreenstein(cosAngle, gHG);
    float rayleigh = (3.0f / (16.0f * 3.1415f)) * (1.0f + cosAngle * cosAngle);
    return mix(rayleigh, hg, ratio);
}

vec3 calculateLighting(vec3 currentPos, vec3 lightPosition, vec3 lightColor, vec3 rayDir, float stepDensity, float noise)
{
    vec3 lightDir = normalize(lightPosition);

    float cosTheta = saturate(dot(lightDir, normalize(-rayDir)));
    float phase = combinedPhaseFunction(cosTheta, anisotropy, scatteringRatio);

    return lightColor * phase * stepDensity * noise;
}

float increaseContrast(float value, float contrast)
{
    float midpoint = 0.5;
    return (value - midpoint) * contrast + midpoint;
}

bool intersectRayAABB(vec3 ro, vec3 rd, vec3 boxMin, vec3 boxMax, out float t1, out float t2)
{
    vec3 invDir = 1.0 / rd;
    vec3 t0 = (boxMin - ro) * invDir;
    vec3 t1Temp = (boxMax - ro) * invDir;

    vec3 tmin = min(t0, t1Temp);
    vec3 tmax = max(t0, t1Temp);

    float tminFinal = max(tmin.x, max(tmin.y, tmin.z));
    float tmaxFinal = min(tmax.x, min(tmax.y, tmax.z));

    if(tmaxFinal < 0.0 || tminFinal > tmaxFinal)
    {
        return false;
    }

    t1 = max(0.0, tminFinal);
    t2 = tmaxFinal;
    return true;
}

// todo:
float getCloudHeight(vec3 rayPosition)
{
    // return 15000.0;
    // return rayPosition.y;
    return u_sgMeshAABBMin.y;
}

float sampleCloudDensity(vec3 rayPosition)
{
    // todo:
    // rayPosition += cloudScrolling * programData.currentTime;

    vec3 relativePos = (rayPosition - objectTransform.position);
    rayPosition = (relativePos + 1.0) * 0.5;

    // sampling weather from weather map
    vec4 weather = textureLod(mat_baseColorSamplers[0], rayPosition.xz / textureSize(mat_baseColorSamplers[0], 0).xy + vec2(0.2, 0.1), 0);
    weather.a = 1.0;

    float height = getCloudHeight(rayPosition);

    // rounding clouds from below
    float SRb = clamp(remap(height, 0.0, 0.07, 0.0, 1.0), 0.0, 1.0);
    // density linear attenuation from 0 to 1 using weather map
    float SRt = clamp(remap(height, weather.b * 0.2, weather.b, 1.0, 0.0), 0.0, 1.0);
    float SA = SRb * SRt;

    // one more variable for rounding clouds from below
    float DRb = height * clamp(remap(height, 0, 0.15, 0.0, 1.0), 0.0, 1.0);
    // rounding clouds from above
    float DRt = height * clamp(remap(height, 0.9, 1.0, 1.0, 0.0), 0.0, 1.0);

    // combining results using density from weather map and custom density uniform
    float DA = DRb * DRt * weather.a * 2.0 * u_sgVolumetricDensity;

    float cloudNoise = textureLod(mat_noiseSamplers[0], rayPosition / 48.0, 0.0).x * 0.85f + textureLod(mat_noiseSamplers[1], rayPosition / 4.8, 0).x * 0.15;

    // calculating sky coverage (percentage of sky that is covered by clouds)
    float WMc = max(weather.r, clamp(u_sgVolumetricCoverage - 0.5, 0.0, 1.0) * weather.g * 2.0);

    // final density
    float d = clamp(remap(cloudNoise * SA, 1.0 - u_sgVolumetricCoverage * WMc, 1.0, 0.0, 1.0), 0.0, 1.0) * DA;

    return d;
}

float sampleCloudDirectDensity(vec3 rayPosition, vec3 sunDir)
{
    // calculating step size
    float avrStep = (u_sgMeshAABBMax.y - u_sgMeshAABBMin.y) * 0.01;
    float sumDensity = 0.0;

    for(int i = 0; i < 4; ++i)
    {
        float step = avrStep;

        // multiplying last step by 6
        if(i == 3) step = step * 6.0;

        // updating ray position
        rayPosition += sunDir * step;

        // getting cloud density
        float density = sampleCloudDensity(rayPosition) * step;

        sumDensity += density;
    }

    return sumDensity;
}

vec4 marchClouds(vec3 viewDir, vec3 sunDir)
{
    float rayStart;
    float rayEnd;

    if(!intersectRayAABB(camera.position, viewDir, u_sgMeshAABBMin, u_sgMeshAABBMax, rayStart, rayEnd))
    {
        return vec4(0.0);
    }

    vec3 rayPosition = viewDir * rayStart;

    float avrStep = (u_sgMeshAABBMax.y - u_sgMeshAABBMin.y) / 128.0;

    vec3 color = vec3(0.0);
    float transmittance = 1.0f;

    const float sunIntensity = 100.0;
    const float volumetricAttenuation = 0.1;
    const float volumetricAttenuation2 = volumetricAttenuation * volumetricAttenuation;

    for(int i = 0; i < 128; ++i)
    {
        float cloudDensityInPoint = sampleCloudDensity(rayPosition) * avrStep;
        if(cloudDensityInPoint > 0.0)
        {
            float sunDensity = sampleCloudDirectDensity(rayPosition, sunDir);

            float m2 = exp(-volumetricAttenuation * sunDensity);
            float m3 = volumetricAttenuation2 * cloudDensityInPoint;
            float light = sunIntensity * m2 * m3;

            color += atmosphere.sunColor * light * transmittance;
            transmittance *= exp(-volumetricAttenuation * cloudDensityInPoint);
        }

        rayPosition += viewDir * avrStep;

        if(transmittance < 0.05 || length(rayPosition) > rayEnd) break;
        // if(transmittance < 0.05 || length(rayPosition) > u_sgMeshAABBMax.y) break;
    }

    return vec4(color, 1.0 - transmittance);
}

vec4 renderClouds()
{
    vec3 rayDir = normalize(vsIn.fragPos - camera.position);

    float stepSize = 100.0;
    float density = 0;

    vec3 accumulatedLight = vec3(0.0);

    float depthBufferValue = gl_FragCoord.z;

    int error = 0;

    float rayStart;
    float rayEnd;
    if(!intersectRayAABB(camera.position, rayDir, u_sgMeshAABBMin, u_sgMeshAABBMax, rayStart, rayEnd))
    {
        return vec4(0.0);
    }

    float distance = rayStart;

    while (distance < rayEnd)
    {
        vec3 currentPos = camera.position + rayDir * distance;
        vec3 halfCubeSize = (u_sgMeshAABBMax - u_sgMeshAABBMin) / 2.0;
        vec3 cubeCenter = objectTransform.position;

        float distanceToCube = sdfBox(currentPos, cubeCenter, halfCubeSize);

        float normalizeDepth = distance / rayEnd;

        if (normalizeDepth > depthBufferValue)
        {
            break;
        }

        if (distanceToCube < stepSize)
        {
            vec3 relativePos = (currentPos - cubeCenter);
            vec3 texCoords = (relativePos + 1.0) * 0.5;

            vec3 shiftedCoords = texCoords.xyz + vec3(cloudScrolling.x * programData.currentTime, cloudScrolling.y * programData.currentTime, cloudScrolling.z * programData.currentTime);

            float lowFreqNoise = texture(mat_noiseSamplers[0], shiftedCoords * worleyScale).x;
            float highFreqNoise = texture(mat_noiseSamplers[1], shiftedCoords * perlinScale).x;

            // float cloudBase = 1.0 - saturate(increaseContrast(lowFreqNoise, 1.0));
            float cloudBase = 1.0 - lowFreqNoise;
            // float cloudDetails = cloudBase * (1.0 + saturate(increaseContrast(highFreqNoise, 5.0)));
            // float cloudDetails = cloudBase;
            float cloudDetails = cloudBase * 0.4 + highFreqNoise * 0.6;

            float noise = saturate(cloudDetails * 1.5 - 0.3);

            float stepDensity = (1.0 - distanceToCube / stepSize) * stepSize * noise;
            density += stepDensity;

            accumulatedLight += calculateLighting(currentPos, atmosphere.sunPosition, atmosphere.sunColor.rgb, rayDir, stepDensity, noise);
        }

        distance += stepSize;
    }

    float normalizeDepth = distance / rayEnd;

    float fogFactor = saturate(exp(-absorptionCoefficient * 0.1 * density));

    vec3 finalLight = accumulatedLight * exp(-absorptionCoefficient * scatteringCoefficient * density);

    return vec4(cloudColor * finalLight, fogFactor);
}

void main()
{
    // vec3 viewDir = normalize(camera.position - vsIn.fragPos);
    vec3 viewDir = normalize(vsIn.fragPos - camera.position);
    // vec4 cloudResult = marchClouds(viewDir, normalize(atmosphere.sunPosition));
    vec4 cloudResult = renderClouds();

    // layerColor = vec4(cloudResult.rgb, cloudResult.a);
    layerColor = vec4(cloudResult.rgb, 1.0 - cloudResult.a);
    // layerColor = vec4(cloudResult.rgb, 1.0);
}

#end