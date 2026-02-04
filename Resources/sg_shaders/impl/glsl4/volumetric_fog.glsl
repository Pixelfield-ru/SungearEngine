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

uniform sampler3D mat_noiseSamplers[1];
uniform int mat_noiseSamplers_CURRENT_COUNT;

float u_sgVolumetricCoverage = 100.0;
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
float perlinScale = 0.001;
float worleyScale = 0.00005;

float scatteringRatio = 0.01;
// float scatteringCoefficient = 0.1;
float scatteringCoefficient = 0.4;

float anisotropy = 0.85;

float absorptionCoefficient = 0.0002;

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

vec4 renderClouds()
{
    return textureLod(mat_noiseSamplers[0], vec3(0.0), 0.0);
    // return vec4(0.0);
    /*vec3 rayDir = normalize(vsIn.fragPos - camera.position);

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
            float perlinNoise = texture(mat_noiseSamplers[0], shiftedCoords * perlinScale).r;
            vec3 worleyNoise = texture(mat_noiseSamplers[0], shiftedCoords * worleyScale).gba;

            float cloudBase = 1.0 - saturate(increaseContrast(worleyNoise.g, 10.0));

            float cloudDetails = cloudBase * (1.0 + saturate(increaseContrast(perlinNoise, 10.0)) * 0.5);

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

    return vec4(cloudColor * finalLight, fogFactor);*/
}

void main()
{
    vec4 cloudResult = renderClouds();

    layerColor = vec4(cloudResult.rgb, 1.0 - cloudResult.a);
}

#end