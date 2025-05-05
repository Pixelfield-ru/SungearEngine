#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"
#include "sg_shaders/impl/glsl4/defines.glsl"

#subpass [GeometryPass]

#attribute [Culling] [true]

#vertex

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;
layout (location = 2) in vec3 normalsAttribute;

out VSOut
{
    vec3 vertexPos;
    vec3 fragPos;
    vec2 UV;
} vsOut;

void main()
{
    vsOut.vertexPos = positionsAttribute;
    vsOut.fragPos = vec3(objectTransform.modelMatrix * vec4(positionsAttribute, 1.0));
    vsOut.UV = UVAttribute.xy;

    gl_Position = vec4(positionsAttribute.xyz, 1.0);
}

#end

// =====================================================================================================================

#tess_control

layout(vertices = 4) out;

out TessControlOut
{
    vec2 UV;
} tessControlOut[];

in VSOut
{
    vec3 vertexPos;
    vec3 fragPos;
    vec2 UV;
} vsIn[];

int getTessLevel(vec3 p, float maxLevel, float maxDistance){
    float l=distance(p,camera.position);
    return int((maxLevel-1)*(1.0-clamp(l,0,maxDistance)/maxDistance))+1;
}

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tessControlOut[gl_InvocationID].UV = vsIn[gl_InvocationID].UV;

    const int MIN_TESS_LEVEL = 10;
    const int MAX_TESS_LEVEL = 70;
    const float MIN_DISTANCE = 0;
    const float MAX_DISTANCE = 800;

    if(gl_InvocationID == 0)
    {
        vec4 eye0 = camera.viewMatrix * objectTransform.modelMatrix * gl_in[0].gl_Position;
        vec4 eye1 = camera.viewMatrix * objectTransform.modelMatrix * gl_in[1].gl_Position;
        vec4 eye2 = camera.viewMatrix * objectTransform.modelMatrix * gl_in[2].gl_Position;
        vec4 eye3 = camera.viewMatrix * objectTransform.modelMatrix * gl_in[3].gl_Position;

        eye0.z = int(eye0.z / 100.0) * 100.0;
        eye1.z = int(eye1.z / 100.0) * 100.0;
        eye2.z = int(eye2.z / 100.0) * 100.0;
        eye3.z = int(eye3.z / 100.0) * 100.0;

        float distance00 = clamp((abs(eye0.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance01 = clamp((abs(eye1.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance10 = clamp((abs(eye2.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance11 = clamp((abs(eye3.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);

        float tessLevel0 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00));
        float tessLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01));
        float tessLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11));
        float tessLevel3 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10));

        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }
}

#end

// =====================================================================================================================

#tess_eval

layout(quads, fractional_odd_spacing, cw) in;

out TessEvalOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    vec3 tangent;
    vec3 bitangent;
    mat3 TBN;
} tessEvalOut;

in TessControlOut
{
    vec2 UV;
} tessControlIn[];

uniform sampler2D mat_displacementSamplers[1];
uniform int mat_displacementSamplers_CURRENT_COUNT;

uniform float u_terrainHeightScale;
uniform vec2 u_terrainUVScale;

float getHeight(vec2 xz)
{
    return texture(mat_displacementSamplers[0], xz).r * u_terrainHeightScale;

    // return 0;
}

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 p_0 = (p01 - p00) * u + p00;
    vec4 p_1 = (p11 - p10) * u + p10;

    // vec4 pos = (p_1 - p_0) * v + p_0;
    vec4 pos = mix(mix(p00, p01, u), mix(p11, p10, u), v);

    vec2 uv0 = tessControlIn[0].UV;
    vec2 uv1 = tessControlIn[1].UV;
    vec2 uv2 = tessControlIn[2].UV;
    vec2 uv3 = tessControlIn[3].UV;

    // generating uv
    vec2 uv_ = mix(mix(uv0, uv3, gl_TessCoord.y), mix(uv1, uv2, gl_TessCoord.y), gl_TessCoord.x);
    vec2 heightSamplePos = uv_;

    vec3 dp1 = p01.xyz - p00.xyz;
    vec3 dp2 = p11.xyz - p00.xyz;

    vec2 duv1 = uv1 - uv0;
    vec2 duv2 = uv3 - uv0;

    float r = 1.0 / (duv1.x * duv2.y - duv1.y * duv2.x);
    vec3 tangent = normalize((dp1 * duv2.y - dp2 * duv1.y) * r);
    vec3 bitangent = normalize((dp2 * duv1.x - dp1 * duv2.x) * r);

    // read neightbor heights using an arbitrary small offset
    vec3 off = vec3(0.001, 0.001, 0);
    float hL = getHeight(heightSamplePos - off.xz);
    float hR = getHeight(heightSamplePos + off.xz);
    float hD = getHeight(heightSamplePos - off.zy);
    float hU = getHeight(heightSamplePos + off.zy);

    // deduce terrain normal
    vec3 normal;
    normal.x = hL - hR;
    normal.y = 2.0;
    normal.z = hD - hU;
    normal = normalize(normal);

    pos.y += getHeight(heightSamplePos);

    tessEvalOut.normal = normal;
    tessEvalOut.tangent = -normalize(tangent - normal * dot(normal, tangent));
    tessEvalOut.bitangent = -cross(normal, tessEvalOut.tangent);
    tessEvalOut.worldNormal = normalize(mat3(transpose(inverse(objectTransform.modelMatrix))) * tessEvalOut.normal);
    tessEvalOut.vertexPos = pos.xyz;
    tessEvalOut.fragPos = vec3(objectTransform.modelMatrix * pos);

    tessEvalOut.UV = uv_ * u_terrainUVScale;

    vec3 T = normalize(vec3(objectTransform.modelMatrix * vec4(tessEvalOut.tangent, 0.0)));
    vec3 B = normalize(vec3(objectTransform.modelMatrix * vec4(tessEvalOut.bitangent, 0.0)));
    vec3 N = normalize(vec3(objectTransform.modelMatrix * vec4(tessEvalOut.normal, 0.0)));
    tessEvalOut.TBN = mat3(T, B, N);

    gl_Position = camera.projectionSpaceMatrix * objectTransform.modelMatrix * pos;
}

#end

// =====================================================================================================================

#fragment

// REQUIRED COLORS!!! ===========
layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;
layout(location = 4) out vec3 layerWorldPosColor;
layout(location = 5) out vec3 layerFragmentNormalColor;
layout(location = 6) out vec3 layerVertexNormalColor;
// REQUIRED COLORS!!! ===========

#include "sg_shaders/impl/glsl4/math.glsl"
#include "sg_shaders/impl/glsl4/pbr_base.glsl"
#include "sg_shaders/impl/glsl4/color_correction/aces.glsl"

uniform sampler2D mat_diffuseSamplers[1];
uniform int mat_diffuseSamplers_CURRENT_COUNT;

uniform sampler2D mat_metalnessSamplers[1];
uniform int mat_metalnessSamplers_CURRENT_COUNT;

uniform sampler2D mat_specularSamplers[1];
uniform int mat_specularSamplers_CURRENT_COUNT;

uniform sampler2D mat_normalsSamplers[1];
uniform int mat_normalsSamplers_CURRENT_COUNT;

uniform sampler2D mat_lightmapSamplers[1];
uniform int mat_lightmapSamplers_CURRENT_COUNT;

uniform sampler2D mat_diffuseRoughnessSamplers[1];
uniform int mat_diffuseRoughnessSamplers_CURRENT_COUNT;

uniform sampler2D mat_heightSamplers[1];
uniform int mat_heightSamplers_CURRENT_COUNT;

// REQUIRED UNIFORM!!
uniform vec3 u_pickingColor;

uniform int SGPP_CurrentLayerIndex;

const float PARALLAX_FACTOR = 0.05;

in TessEvalOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    vec3 tangent;
    vec3 bitangent;
    mat3 TBN;
} tessEvalIn;

vec2 parallaxMapping(vec2 UV, vec3 viewDir, float heightScale)
{
    const float minLayers = 8.0;
    const float maxLayers = 64.0;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords     = UV;
    float currentDepthMapValue = texture(mat_heightSamplers[0], currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(mat_heightSamplers[0], currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = (texture(mat_heightSamplers[0], prevTexCoords).r) - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{
    vec3 normalizedNormal = tessEvalIn.normal;

    vec4 diffuseColor = vec4(u_materialDiffuseCol);
    vec4 aoRoughnessMetallic = vec4(materialAmbientFactor, u_materialRoughnessFactor, u_materialMetallicFactor, 1);
    float specularCoeff = 0.0f;
    vec3 normalMapColor = vec3(0);
    vec3 finalNormal = vec3(0);
    float parallaxHeight = 0.0;
    vec3 normalizedNormalMapColor = vec3(0);

    vec2 finalUV = 1.0 - tessEvalIn.UV;
    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - tessEvalIn.UV.y;
    #endif

    vec2 originalUV = finalUV;

    // ===============================================================================================
    // ===============================        loading textures       =================================
    // ===============================================================================================

    if(mat_heightSamplers_CURRENT_COUNT > 0)
    {
        vec3 viewDirToParallax = normalize(transpose(tessEvalIn.TBN) * (camera.position - tessEvalIn.fragPos));
        finalUV = parallaxMapping(finalUV, viewDirToParallax, PARALLAX_FACTOR);

        // if(finalUV.x > 1.0 || finalUV.y > 1.0 || finalUV.x < 0.0 || finalUV.y < 0.0) discard;
    }

    {
        if(mat_diffuseSamplers_CURRENT_COUNT > 0)
        {
            float mixCoeff = 1.0 / mat_diffuseSamplers_CURRENT_COUNT;

            diffuseColor.rgba = vec4(0.0, 0.0, 0.0, 0.0);

            for (int i = 0; i < mat_diffuseSamplers_CURRENT_COUNT; ++i)
            {
                diffuseColor += texture(mat_diffuseSamplers[i], finalUV) * mixCoeff;
            }
        }
    }

    if(diffuseColor.a < 0.05) discard;

    {
        if(mat_specularSamplers_CURRENT_COUNT > 0)
        {
            float mixCoeff = 1.0 / mat_specularSamplers_CURRENT_COUNT;

            specularCoeff = 0.0;

            for (int i = 0; i < mat_specularSamplers_CURRENT_COUNT; ++i)
            {
                specularCoeff += texture(mat_specularSamplers[i], finalUV).r * mixCoeff;
            }
        }
    }

    {
        {
            if(mat_lightmapSamplers_CURRENT_COUNT > 0)
            {
                float mixCoeff = 1.0 / mat_lightmapSamplers_CURRENT_COUNT;

                aoRoughnessMetallic.r = 0.0;

                for (int i = 0; i < mat_lightmapSamplers_CURRENT_COUNT; ++i)
                {
                    aoRoughnessMetallic.r += texture(mat_lightmapSamplers[i], finalUV).r * mixCoeff;
                }
            }
        }

        {
            if(mat_diffuseRoughnessSamplers_CURRENT_COUNT > 0)
            {
                float mixCoeff = 1.0 / mat_diffuseRoughnessSamplers_CURRENT_COUNT;

                aoRoughnessMetallic.g = 0.0;

                for (int i = 0; i < mat_diffuseRoughnessSamplers_CURRENT_COUNT; ++i)
                {
                    aoRoughnessMetallic.g += texture(mat_diffuseRoughnessSamplers[i], finalUV).g * mixCoeff;
                }

                aoRoughnessMetallic.g *= 1.0;
            }
        }

        {
            if(mat_metalnessSamplers_CURRENT_COUNT > 0)
            {
                float mixCoeff = 1.0 / mat_metalnessSamplers_CURRENT_COUNT;

                aoRoughnessMetallic.b = 0.0;

                for (int i = 0; i < mat_metalnessSamplers_CURRENT_COUNT; ++i)
                {
                    aoRoughnessMetallic.b += texture(mat_metalnessSamplers[i], finalUV).b * mixCoeff;
                }

                aoRoughnessMetallic.b *= 1.0;
            }
        }
    }

    if(mat_normalsSamplers_CURRENT_COUNT > 0)
    {
        float mixCoeff = 1.0 / mat_normalsSamplers_CURRENT_COUNT;

        for (int i = 0; i < mat_normalsSamplers_CURRENT_COUNT; ++i)
        {
            normalMapColor += texture(mat_normalsSamplers[i], finalUV).rgb * mixCoeff;
        }

        finalNormal = normalize(tessEvalIn.TBN * (normalMapColor * 2.0 - 1.0));
        normalizedNormalMapColor = normalize(normalMapColor * 2.0 - 1.0);
    }
    else
    {
        finalNormal = tessEvalIn.worldNormal;
        normalizedNormalMapColor = tessEvalIn.worldNormal;
    }

    vec3 viewDir = normalize(camera.position - tessEvalIn.fragPos);

    // colorFromRoughness.r = AO MAP
    // colorFromRoughness.g = ROUGHNESS
    // colorFromRoughness.b = METALNESS

    vec3 albedo         = diffuseColor.rgb;
    float ao            = aoRoughnessMetallic.r;
    float roughness     = aoRoughnessMetallic.g;
    float metalness     = aoRoughnessMetallic.b;

    // для формулы Шлика-Френеля
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metalness);

    vec3 dirLightsShadowCoeff = vec3(0.0);

    vec3 ambient = vec3(0.0);
    vec3 lo = vec3(0.0);

    // calculating sun
    {
        vec3 lightDir = normalize(atmosphere.sunPosition);
        vec3 halfWayDir = normalize(lightDir + viewDir);

        // energy brightness coeff (коэфф. энергетической яркости)
        float NdotL = saturate(dot(finalNormal, lightDir));
        float NdotVD = abs(dot(finalNormal, viewDir)) + 1e-5f;

        // ===================        shadows calc        =====================

        /*dirLightsShadowCoeff += calcDirLightShadow(
            directionalLights[i],
            vsIn.fragPos,
            finalNormal,
            sgmat_shadowMap2DSamplers[i]
        ) * finalRadiance;*/

        // ====================================================================

        // cooktorrance func: DFG /

        // NDF (normal distribution func)
        float D = GGXTR(
        finalNormal,
        halfWayDir,
        roughness * (1.0 - specularCoeff)
        );// TRUE

        float cosTheta = saturate(dot(halfWayDir, viewDir));

        // это по сути зеркальная часть (kS)
        vec3 F = SchlickFresnel(cosTheta, F0);// kS
        // geometry function
        float G = GeometrySmith(NdotVD, NdotL, roughness * (1.0 - specularCoeff));// TRUE

        vec3 diffuse = vec3(1.0) - F;
        diffuse *= (1.0 - metalness);// check diffuse color higher

        vec3 ctNumerator = D * F * G;
        float ctDenominator = 1.0 * NdotVD * NdotL;
        vec3 specular = (ctNumerator / max(ctDenominator, 0.001)) * u_materialSpecularCol.r;

        lo += (diffuse * albedo.rgb / PI + specular) * max(atmosphere.sunColor.rgb, vec3(0, 0, 0)) * NdotL * 1.0;
    }

    // ambient = ambient * albedo.rgb * ao;
    ambient = albedo.rgb * ao;
    vec3 finalCol = ambient * u_materialAmbientCol.rgb * materialAmbientFactor + lo + ambient;
    float exposure = 0.7;
    finalCol = ACESTonemap(finalCol, exposure);

    // finalCol = vec3(tessEvalIn.UV, 0.0);

    // finalCol = vec3(tessEvalIn.UV.xy, 0.0);
    // finalCol = tessEvalIn.vertexPos;
    // finalCol = tessEvalIn.normal;
    // finalCol = tessEvalIn.tangent;
    // finalCol = tessEvalIn.bitangent;
    // finalCol = tessEvalIn.worldNormal;
    // finalCol = normalMapColor;
    // finalCol = finalNormal;
    // finalCol = vec3(aoRoughnessMetallic.r);
    // finalCol = vec3(aoRoughnessMetallic.g);
    // finalCol = vec3(aoRoughnessMetallic.b);
    // finalCol = aoRoughnessMetallic.rgb;
    // finalCol = vec3(albedo.rgb);

    // finalCol = vec3(1.0);

    vec2 parallaxOffset = (finalUV - originalUV) * (1.0 / PARALLAX_FACTOR); // в texel space
    vec3 offsetTangent = vec3(parallaxOffset, 0.0);

    vec3 offsetWorld = tessEvalIn.TBN * offsetTangent;

    layerColor = vec4(finalCol, 1.0);
    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);
    pickingColor = vec3(u_pickingColor);
    layerWorldPosColor = tessEvalIn.fragPos + offsetWorld;
    layerFragmentNormalColor = finalNormal;
    layerVertexNormalColor = tessEvalIn.worldNormal;
}

#end

/*#geometry

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

*//*in TessEvalOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    vec3 tangent;
    vec3 bitangent;
    mat3 TBN;
} tessEvalIn[];*//*

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        int j = (i + 1) % 3;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
        gl_Position = gl_in[j].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
}

#end*/
