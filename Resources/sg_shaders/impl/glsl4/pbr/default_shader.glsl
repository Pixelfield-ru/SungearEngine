#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/random.glsl"
#include "sg_shaders/impl/glsl4/disks.glsl"
#include "sg_shaders/impl/glsl4/math.glsl"
#include "sg_shaders/impl/glsl4/defines.glsl"
#include "sg_shaders/impl/glsl4/color_correction/aces.glsl"
#include "sg_shaders/impl/glsl4/color_correction/reinhard.glsl"
#include "sg_shaders/impl/glsl4/color_correction/lottes.glsl"
#include "sg_shaders/impl/glsl4/color_correction/filmic.glsl"
#include "sg_shaders/impl/glsl4/color_correction/neutral.glsl"
#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"
#include "sg_shaders/impl/glsl4/alpha_resolving/wboit.glsl"
#include "sg_shaders/impl/glsl4/alpha_resolving/stochastic_transparency.glsl"
#include "sg_shaders/impl/glsl4/random.glsl"
#include "dir_lights_shadows_calc.glsl"

#subpass [GeometryPass]

#attribute [Culling] [true]

float ambient = 0.1;

#vertex

#include "sg_shaders/impl/glsl4/instancing.glsl"

layout (location = SG_VS_POSITIONS_ATTRIBUTE_LOC) in vec3 positionsAttribute;
layout (location = SG_VS_UV_ATTRIBUTE_LOC) in vec3 UVAttribute;
layout (location = SG_VS_NORMALS_ATTRIBUTE_LOC) in vec3 normalsAttribute;
layout (location = SG_VS_TANGENTS_ATTRIBUTE_LOC) in vec3 tangentsAttribute;
layout (location = SG_VS_BITANGENTS_ATTRIBUTE_LOC) in vec3 bitangentsAttribute;
layout (location = SG_VS_BONES_IDS_ATTRIBUTE0_LOC) in ivec4 bonesIDsAttribute0;
layout (location = SG_VS_BONES_IDS_ATTRIBUTE1_LOC) in ivec4 bonesIDsAttribute1;
layout (location = SG_VS_BONES_WEIGHTS_ATTRIBUTE0_LOC) in vec4 bonesWeightsAttribute0;
layout (location = SG_VS_BONES_WEIGHTS_ATTRIBUTE1_LOC) in vec4 bonesWeightsAttribute1;
#if SG_VS_VERTEX_COLOR_ATTRIBUTE0_LOC < SG_VS_MAX_ATTRIBS_COUNT
    layout (location = SG_VS_VERTEX_COLOR_ATTRIBUTE0_LOC) in vec4 vertexColor0Attribute;
#endif
#if SG_VS_VERTEX_COLOR_ATTRIBUTE1_LOC < SG_VS_MAX_ATTRIBS_COUNT
    layout (location = SG_VS_VERTEX_COLOR_ATTRIBUTE1_LOC) in vec4 vertexColor1Attribute;
#endif

#include "sg_shaders/impl/glsl4/animation/bones_calculation.glsl"
#include "sg_shaders/impl/glsl4/vs_attribs_utils.glsl"

out VSOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;

    vec4 vertexColor0;
    vec4 vertexColor1;

    vec4 bonesWeights0;
    vec4 bonesWeights1;
} vsOut;

void main()
{
    vec4 totalPosition = vec4(0.0);
    vec3 totalNormal = vec3(0.0);

    mat4 finalModelMatrix = getCurrentInstanceModelMatrix();

    bool isMeshAffectedByBones = calculateVertexPosAndNormal(positionsAttribute, normalsAttribute, totalPosition, totalNormal);

    /*totalPosition.xyz = positionsAttribute;
    totalPosition.w = 1.0;
    totalNormal = normalsAttribute;*/

    if(isMeshAffectedByBones)
    {
        finalModelMatrix = mat4(1.0);
    }

    vsOut.UV = UVAttribute.xy;
    vsOut.normal = normalize(totalNormal);
    vsOut.worldNormal = normalize(mat3(transpose(inverse(finalModelMatrix))) * totalNormal);

    vsOut.vertexPos = totalPosition.xyz;
    vsOut.fragPos = vec3(finalModelMatrix * totalPosition);

    vsOut.bonesWeights0 = bonesWeightsAttribute0;
    vsOut.bonesWeights1 = bonesWeightsAttribute1;

    // 0.0 IN w COMPONENT IS CORRECT!!
    vec3 T = normalize(vec3(finalModelMatrix * vec4(tangentsAttribute, 0.0)));
    vec3 B = normalize(vec3(finalModelMatrix * vec4(bitangentsAttribute, 0.0)));
    vec3 N = normalize(vec3(finalModelMatrix * vec4(vsOut.normal, 0.0)));
    vsOut.TBN = mat3(T, B, N);

    vsOut.vertexColor0 = getVertexColor0();
    vsOut.vertexColor1 = getVertexColor1();

    // gl_Position = camera.projectionSpaceMatrix * vec4(vsOut.fragPos, 1.0);
    gl_Position = camera.projectionSpaceMatrix * objectTransform.modelMatrix * vec4(positionsAttribute, 1.0);
}

#end

// ============================================================================
// ============================================================================
// ============================================================================

#fragment

// REQUIRED COLORS!!! ===========
layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;
// COLOR FOR STOCHASTIC TRANSPARNCY
layout(location = 3) out vec4 layerSTColor;
layout(location = 4) out vec3 layerWorldPosColor;
layout(location = 5) out vec3 layerFragmentNormalColor;
layout(location = 6) out vec3 layerVertexNormalColor;
// REQUIRED COLORS!!! ===========

#include "sg_shaders/impl/glsl4/pbr_base.glsl"
#include "sg_shaders/impl/glsl4/shadows_sampling/csm.glsl"

uniform sampler2D mat_diffuseSamplers[3];
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

uniform int SGPP_CurrentLayerIndex;

// REQUIRED UNIFORM!!
uniform vec3 u_pickingColor;

// REQUIRED UNIFORM!!
uniform int u_verticesColorsAttributesCount;

// REQUIRED UNIFORM!!
uniform int u_isTransparentPass;

in VSOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;
    vec4 vertexColor0;
    vec4 vertexColor1;

    vec4 bonesWeights0;
    vec4 bonesWeights1;
} vsIn;

void main()
{
    vec3 normalizedNormal = vsIn.normal;

    vec4 diffuseColor = vec4(u_materialDiffuseCol);
    vec4 aoRoughnessMetallic = vec4(materialAmbientFactor, u_materialRoughnessFactor, u_materialMetallicFactor, 1);
    float specularCoeff = 0.0f;
    vec3 normalMapColor = vec3(0);
    vec3 finalNormal = vec3(0);

    vec2 finalUV = vsIn.UV;
    #ifdef FLIP_TEXTURES_Y
        finalUV.y = 1.0 - vsIn.UV.y;
    #endif

    // ===============================================================================================
    // ===============================        load textures       ====================================
    // ===============================================================================================

    {
        if(mat_diffuseSamplers_CURRENT_COUNT > 0)
        {
            float mixCoeff = 1.0 / float(mat_diffuseSamplers_CURRENT_COUNT);

            diffuseColor.rgba = vec4(0.0, 0.0, 0.0, 0.0);

            for (int i = 0; i < mat_diffuseSamplers_CURRENT_COUNT; ++i)
            {
                diffuseColor += texture(mat_diffuseSamplers[i], finalUV) * mixCoeff;
            }
        }
    }

    // if(diffuseColor.a < 0.05) discard;

    {
        if(mat_specularSamplers_CURRENT_COUNT > 0)
        {
            float mixCoeff = 1.0 / float(mat_specularSamplers_CURRENT_COUNT);

            specularCoeff = 0.0;

            for (int i = 0; i < mat_specularSamplers_CURRENT_COUNT; ++i)
            {
                specularCoeff += texture(mat_specularSamplers[i], finalUV).r * mixCoeff;
            }
        }
    }


    /*if(diffuseColor.a < 0.1)
    {
        discard;
    }*/

    {
        {
            if(mat_lightmapSamplers_CURRENT_COUNT > 0)
            {
                float mixCoeff = 1.0 / float(mat_lightmapSamplers_CURRENT_COUNT);

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
                float mixCoeff = 1.0 / float(mat_diffuseRoughnessSamplers_CURRENT_COUNT);

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
                float mixCoeff = 1.0 / float(mat_metalnessSamplers_CURRENT_COUNT);

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
        float mixCoeff = 1.0 / float(mat_normalsSamplers_CURRENT_COUNT);

        for (int i = 0; i < mat_normalsSamplers_CURRENT_COUNT; ++i)
        {
            normalMapColor += texture(mat_normalsSamplers[i], finalUV).rgb * mixCoeff;
        }

        finalNormal = normalize(vsIn.TBN * (normalMapColor * 2.0 - 1.0));
    }
    else
    {
        finalNormal = vsIn.worldNormal;
    }

    // if(diffuseColor.a < 0.1) discard;

    /*gFragPos = vec4(vsIn.fragPos, 1.0);
    gNormal = vec4(normalMapColor.rg, 0.0, 1.0);
    gAlbedoSpec = vec4(diffuseColor.rgb, 1.0);*/

    //unknownAttachment0 = vec3(1.0);
    //unknownAttachment1 = vec3(1.0);

    // ===============================================================================================
    // ===============================================================================================
    // ===============================================================================================

    // TODO: make customizable with defines
    // PBR pipeline (using Cook-Torrance BRDF) --------------------

    vec3 viewDir = normalize(camera.position - vsIn.fragPos);

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
    for (int i = 0; i < directionalLightsCount; i++)
    {
        DirectionalLight dirLight = directionalLights[i];

        vec3 lightDir = normalize(dirLight.position - vsIn.fragPos);// TRUE
        vec3 halfWayDir = normalize(lightDir + viewDir);// TRUE

        float distance = length(dirLight.position - vsIn.fragPos);// TRUE
        float attenuation = (1.0 / (distance * distance)) * dirLight.intensity;// TRUE
        vec3 radiance = dirLight.color.rgb * attenuation;// TRUE

        // energy brightness coeff (коэфф. энергетической яркости)
        float NdotL = max(dot(finalNormal, lightDir), 0.0);
        float NdotVD = max(dot(finalNormal, viewDir), 0.0);

        vec3 finalRadiance = NdotL * radiance + radiance * 0.04;

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

        float cosTheta = max(dot(halfWayDir, viewDir), 0.0);

        // это по сути зеркальная часть (kS)
        vec3 F = SchlickFresnel(cosTheta, F0);// kS
        // geometry function
        float G = GeometrySmith(NdotVD, NdotL, roughness * (1.0 - specularCoeff));// TRUE

        vec3 diffuse = vec3(1.0) - F;
        diffuse *= (1.0 - metalness);// check diffuse color higher

        vec3 ctNumerator = D * F * G;
        float ctDenominator = 4.0 * NdotVD * NdotL;
        vec3 specular = (ctNumerator / max(ctDenominator, 0.001)) * u_materialSpecularCol.rgb;

        lo += (diffuse * albedo.rgb / PI + specular) * radiance * NdotL;

        ambient += 0.025 * radiance;
        // lo = vec3(1.0);
    }

    float shadow = getCSMShadow(atmosphere.sunPosition, vsIn.fragPos);

    // calculating sun
    {
        // ambient += atmosphere.sunAmbient;

        vec3 lightDir = normalize(atmosphere.sunPosition);// TRUE
        vec3 halfWayDir = normalize(lightDir + viewDir);// TRUE

        // energy brightness coeff (коэфф. энергетической яркости)
        float NdotL = saturate(dot(finalNormal, lightDir));
        float NdotVD = abs(dot(finalNormal, viewDir)) + 1e-5f;

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

    ambient = albedo.rgb * ao * dot(atmosphere.sunPosition, vec3(0, 1, 0));
    vec3 finalCol = ambient * vec3(1.0) * materialAmbientFactor + lo;

    if(u_verticesColorsAttributesCount > 0)
    {
        finalCol.rgb = finalCol * vsIn.vertexColor0.rgb;
    }

    // layerColor.rgb = vsIn.bonesWeights.rgb;

    // DEBUG ==================================
    // base color
    // finalCol.rgb = albedo.rgb; // PASSED
    // finalCol.rgb = vec3(albedo.r, albedo.g, albedo.b); // PASSED
    // finalCol.rgb = vec3(metalness); // PASSED
    // finalCol.rgb = vec3(roughness); // PASSED
    // finalCol.rgb = vec3(specularCoeff); // PASSED
    // finalCol.rgb = vec3(u_materialSpecularCol.r); // PASSED
    // finalCol.rgb = finalNormal;
    // finalCol.rgb = vsIn.worldNormal;
    // finalCol.rgb = normalizedNormal;
    // finalCol.rgb = normalMapColor; // PASSED
    // finalCol.rgb = vec3(ao); // PASSED
    // finalCol.rgb = vec3(vsIn.UV, 1.0);

    // todo: make
    // if(u_isStochasticTransparencyEnabled) // todo: impl
    /*{

        bool isSTNotPassed = calculateStochasticTransparencyComponents(finalCol.rgb, diffuseColor.a, layerSTColor, layerColor, vsIn.UV, u_isTransparentPass);

        if(isSTNotPassed)
        {
            discard;
        }
    }*/

    layerColor.rgba = vec4(1.0);



    // discard;

    // layerColor.a = diffuseColor.a;

    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);

    // if(u_isTransparentPass == 0)
    {
        pickingColor = vec3(u_pickingColor);
    }

    layerWorldPosColor = vsIn.fragPos;
    layerFragmentNormalColor = finalNormal;
    layerVertexNormalColor = vsIn.worldNormal;

    vec4 bonesColor = vec4(0.0);
    /*for(int i = 0; i < 4; ++i)
    {
        if(vsIn.bonesIDs[i] != -1)
        {
            bonesColor[i] += vsIn.bonesWeights[i];
        }
    }*/
    /*vec3 debugColor = vec3(0.0);
    vec3 d = vec3(1.0, 1.0, 1.0);

    debugColor += vsIn.bonesWeights0.rgb * d;

    layerColor.rgb = debugColor;*/
    //  layerColor = bonesColor;
    // layerColor.rgb = vec3(vsIn.bonesIDs);

    // pickingColor.r = depthFromTexture;

    /*fragColor0 = vec4(normalMapColor, 1.0);
    fragColor1 = vec4(normalMapColor, 1.0);*/

    // layerColor.rgb = vec3(1.0);
}

#end