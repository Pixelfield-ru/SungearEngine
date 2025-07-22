#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"

#subpass [DecalsPass]

#attribute [Culling] [true]

#vertex

#include "sg_shaders/impl/glsl4/instancing.glsl"

layout (location = SG_VS_POSITIONS_ATTRIBUTE_LOC) in vec3 positionsAttribute;
layout (location = SG_VS_UV_ATTRIBUTE_LOC) in vec3 UVAttribute;
layout (location = SG_VS_NORMALS_ATTRIBUTE_LOC) in vec3 normalsAttribute;
layout (location = SG_VS_TANGENTS_ATTRIBUTE_LOC) in vec3 tangentsAttribute;
layout (location = SG_VS_BITANGENTS_ATTRIBUTE_LOC) in vec3 bitangentsAttribute;

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

#include "sg_shaders/impl/glsl4/transform_utils.glsl"

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

uniform sampler2D u_GBufferWorldPos;
uniform sampler2D u_GBufferNormal;

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

void main()
{
    // calculating projection ==============================================================================

    vec2 uv = gl_FragCoord.xy / programData.primaryMonitorSize;

    vec3 gBufWorldPos = texture(u_GBufferWorldPos, uv).xyz;

    vec3 localPos = (vsIn.invModelMatrix * vec4(gBufWorldPos, 1.0)).xyz;

    if(any(lessThan(localPos, vec3(-0.5))) || any(greaterThan(localPos, vec3(0.5)))) discard;

    vec3 gBufNormal = normalize(texture(u_GBufferNormal, uv).xyz);

    vec2 decalUV = localPos.xz + 0.5;

    // =====================================================================================================
    // =====================================================================================================
    // =====================================================================================================

    vec2 finalUV = decalUV;

    vec3 normalizedNormal = vsIn.normal;

    vec4 diffuseColor = vec4(u_materialDiffuseCol);
    vec4 aoRoughnessMetallic = vec4(materialAmbientFactor, u_materialRoughnessFactor, u_materialMetallicFactor, 1);
    float specularCoeff = 0.0f;
    vec3 normalMapColor = vec3(0);
    vec3 finalNormal = vec3(0);
    float parallaxHeight = 0.0;
    vec3 normalizedNormalMapColor = vec3(0);

    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - vsIn.UV.y;
    #endif

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

        // Выбираем временный вектор "up", который не параллелен N
        vec3 up = abs(gBufNormal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(0.0, 1.0, 0.0);

        // Строим ортонормальный базис
        vec3 T = normalize(cross(up, gBufNormal));
        vec3 B = cross(gBufNormal, T);
        mat3 TBN = mat3(T, B, gBufNormal);

        finalNormal = normalize(TBN * (normalMapColor * 2.0 - 1.0));
        normalizedNormalMapColor = normalize(normalMapColor * 2.0 - 1.0);
    }
    else
    {
        finalNormal = gBufNormal;
        normalizedNormalMapColor = gBufNormal;
    }

    vec3 viewDir = normalize(camera.position - gBufWorldPos);

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

    float shadow = getCSMShadow(atmosphere.sunPosition, vsIn.fragPos);

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

        lo += (diffuse * albedo.rgb / PI + specular) * max(atmosphere.sunColor.rgb, vec3(0, 0, 0)) * NdotL * shadow * 1.0;
    }

    // ambient = ambient * albedo.rgb * ao;
    ambient = albedo.rgb * ao * dot(atmosphere.sunPosition, vec3(0, 1, 0));
    vec3 finalCol = ambient * materialAmbientFactor + lo;

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

    layerColor = vec4(finalCol, 1.0);
    // layerColor = vec4(decalUV, 0.0, 1.0);
    // layerColor = vec4(gBufWorldPos, 1.0);
    // layerColor = vec4(1.0, 1.0, 0.0, 1.0);
    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);
    pickingColor = vec3(u_pickingColor);
}

#end