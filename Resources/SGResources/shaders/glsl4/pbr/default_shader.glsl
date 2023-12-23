// TODO: make optimization

#include "../rendering_structs_decl.glsl"
#include "../uniform_bufs_decl.glsl"
#include "../random.glsl"
#include "../disks.glsl"
#include "../math.glsl"
#include "../defines.glsl"
#include "../color_correction/aces.glsl"
#include "dir_lights_shadows_calc.glsl"

/*in int gl_FrontFacing;
in vec4 gl_FragCoord;*/

float ambient = 0.1;

#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;
    layout (location = 3) in vec3 tangentsAttribute;
    layout (location = 4) in vec3 bitangentsAttribute;

    out VSOut
    {
        vec2 UV;
        vec3 normal;

        vec3 vertexPos;
        vec3 fragPos;
        mat3 TBN;
    } vsOut;

    void main()
    {
        vsOut.UV = UVAttribute.xy;
        vsOut.normal = normalsAttribute;

        vsOut.vertexPos = positionsAttribute;
        vsOut.fragPos = vec3(objectModelMatrix * vec4(positionsAttribute, 1.0));

        vec3 T = normalize(vec3(objectModelMatrix * vec4(tangentsAttribute,   0.0)));
        vec3 B = normalize(vec3(objectModelMatrix * vec4(bitangentsAttribute, 0.0)));
        vec3 N = normalize(vec3(objectModelMatrix * vec4(normalsAttribute,    0.0)));
        vsOut.TBN = mat3(T, B, N);

        gl_Position = camera.spaceMatrix * vec4(vsOut.fragPos, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    layout(location = 0) out vec4 fragColor0;
    layout(location = 1) out vec4 fragColor1;
    layout(location = 2) out vec4 gFragPos;
    layout(location = 3) out vec4 gNormal;
    layout(location = 4) out vec4 gAlbedoSpec;
    // layout(location = 4) out vec3 unknownAttachment0;
    // layout(location = 5) out vec3 unknownAttachment1;

    /*
        SGUSampler2D diffuseSamplers[] = SGCollectFromMaterial(SGTT_DIFFUSE);
        раскладывается в
        uniform sampler2D diffuseSamplers[КОЛ_ВО_НАЙДЕННЫХ_ТЕКСТУР_У_МАТЕРИАЛА];
        uniform int diffuseSamplers_COUNT;

        SGUSampler2D catTex = SGGetTexture("cat");
        раскладывается в
        uniform sampler2D catTex;

        SGUSampler2D catsTex[] = SGGetTextures("cat0", "cat1", "cat2");
        раскладывается в
        uniform sampler2D catsTex[3];
        uniform int catsTex_COUNT;
    */
    uniform int sgmat_diffuseSamplers_COUNT = 0;

    uniform int sgmat_lightmapSamplers_COUNT = 0;
    uniform int sgmat_diffuseRoughnessSamplers_COUNT = 0;
    uniform int sgmat_metalnessSamplers_COUNT = 0;

    uniform int sgmat_normalsSamplers_COUNT = 0;

    uniform sampler2D sgmat_diffuseSamplers[SGMAT_SAMPLERS_OF_TYPE_MAX];

    uniform sampler2D sgmat_lightmapSamplers[SGMAT_SAMPLERS_OF_TYPE_MAX];
    uniform sampler2D sgmat_diffuseRoughnessSamplers[SGMAT_SAMPLERS_OF_TYPE_MAX];
    uniform sampler2D sgmat_metalnessSamplers[SGMAT_SAMPLERS_OF_TYPE_MAX];

    uniform sampler2D sgmat_normalsSamplers[SGMAT_SAMPLERS_OF_TYPE_MAX];

    in VSOut
    {
        vec2 UV;
        vec3 normal;

        vec3 vertexPos;
        vec3 fragPos;
        mat3 TBN;
    } vsIn;

    // N - normal
    // VD - view dir
    float GeometryShlickGGX(const in float NdotVD, const in float roughness)
    {
        float r = (roughness + 1.0);
        float k = (r * r) / 8.0;

        float denom = NdotVD * (1.0 - k) + k;

        return NdotVD / denom;
    }

    // площадь поверхности, где микроскопические неровности перекрывают друг друга
    float GeometrySmith(const in vec3 normal, const in float NdotVD, const in float NdotL, float roughness)
    {
        // косинус между направлением камеры и нормалью к поверхности
        // float NdotVD = _NdotVD;
        // косинус между направлением источника света и нормалью к поверхности
        // float NdotL = _NdotL;

        // ggx from view dir
        // насколько освещён фрагмент при виде от камеры
        float ggx2 = GeometryShlickGGX(NdotVD, roughness);
        // ggx from light dir
        // насколько освещён тот же фрагмент при виде от источника света
        float ggx1 = GeometryShlickGGX(NdotL, roughness);

        // если ggx1 = 1, а ggx2 = 0, то это geometry shadowing
        // если ggx1 = 0, а ggx2 = 1, то это geometry obstruction
        return ggx1 * ggx2;
    }

    // Trowbridge-Reitz GGX (Normal Distribution Function)
    float GGXTR(const in vec3 normal, const in vec3 medianVec, const in float roughness)
    {
        float roughness4 = pow(roughness, 4.0);

        // косинус между медианным вектором и нормалью фрагмента
        float NdotMV = max(dot(normal, medianVec), 0.0);
        float NdotMV2 = NdotMV * NdotMV;

        // числитель
        float numerator = roughness4;
        // заменатель
        float denominator = (NdotMV2 * (roughness4 - 1.0) + 1.0);
        denominator = PI * denominator * denominator;

        return numerator / denominator;
    }

    vec3 SchlickFresnel(const in float cosTheta, const in vec3 F0)
    {
        return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
    }

    void main()
    {
        vec3 normalizedNormal = normalize(vsIn.normal);

        vec4 diffuseColor = vec4(materialDiffuseCol.rgb, 1);
        vec4 aoRoughnessMetallic = vec4(1, materialRoughnessFactor, materialMetallicFactor, 1);
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
            if(sgmat_diffuseSamplers_COUNT > 0)
            {
                float mixCoeff = 1.0 / sgmat_diffuseSamplers_COUNT;

                diffuseColor.rgb = vec3(0.0, 0.0, 0.0);

                for (int i = 0; i < sgmat_diffuseSamplers_COUNT; ++i)
                {
                    diffuseColor += texture(sgmat_diffuseSamplers[i], finalUV) * mixCoeff;
                }
            }
        }

        {
            {
                if(sgmat_lightmapSamplers_COUNT > 0)
                {
                    float mixCoeff = 1.0 / sgmat_lightmapSamplers_COUNT;

                    aoRoughnessMetallic.r = 0.0;

                    for (int i = 0; i < sgmat_lightmapSamplers_COUNT; ++i)
                    {
                        aoRoughnessMetallic.r += texture(sgmat_lightmapSamplers[i], finalUV).r * mixCoeff;
                    }
                }
            }

            {
                if(sgmat_diffuseRoughnessSamplers_COUNT > 0)
                {
                    float mixCoeff = 1.0 / sgmat_diffuseRoughnessSamplers_COUNT;

                    aoRoughnessMetallic.g = 0.0;

                    for (int i = 0; i < sgmat_diffuseRoughnessSamplers_COUNT; ++i)
                    {
                        aoRoughnessMetallic.g += texture(sgmat_diffuseRoughnessSamplers[i], finalUV).g * mixCoeff;
                    }

                    aoRoughnessMetallic.g *= materialRoughnessFactor;
                }
            }

            {
                if(sgmat_metalnessSamplers_COUNT > 0)
                {
                    float mixCoeff = 1.0 / sgmat_metalnessSamplers_COUNT;

                    aoRoughnessMetallic.b = 0.0;

                    for (int i = 0; i < sgmat_metalnessSamplers_COUNT; ++i)
                    {
                        aoRoughnessMetallic.b += texture(sgmat_metalnessSamplers[i], finalUV).b * mixCoeff;
                    }

                    aoRoughnessMetallic.b *= materialMetallicFactor;
                }
            }
        }

        if(sgmat_normalsSamplers_COUNT > 0)
        {
            float mixCoeff = 1.0 / sgmat_normalsSamplers_COUNT;

            for (int i = 0; i < sgmat_normalsSamplers_COUNT; ++i)
            {
                normalMapColor += texture(sgmat_normalsSamplers[i], finalUV).rgb * mixCoeff;
            }

            finalNormal = normalize(vsIn.TBN * (normalMapColor * 2.0 - 1.0));
        }
        else
        {
            finalNormal = normalizedNormal;
        }

        gFragPos = vec4(vsIn.fragPos, 1.0);
        // gNormal = vec4(finalNormal, 1.0);
        gNormal = vec4(normalMapColor.rg, 0.0, 1.0);
        gAlbedoSpec = vec4(diffuseColor.rgb, 1.0);

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

        vec3 lo = vec3(0.0);
        for (int i = 0; i < DIRECTIONAL_LIGHTS_COUNT; i++)
        {
            ILight lightPart = directionalLights[i].lightPart;
            IRenderingComponent renderingPart = lightPart.renderingPart;

            vec3 lightDir = normalize(renderingPart.position - vsIn.fragPos);// TRUE
            vec3 halfWayDir = normalize(lightDir + viewDir);// TRUE

            float distance = length(renderingPart.position - vsIn.fragPos);// TRUE
            float attenuation = (1.0 / (distance * distance)) * lightPart.intensity;// TRUE
            vec3 radiance = lightPart.color.rgb * attenuation;// TRUE

            // energy brightness coeff (коэфф. энергетической яркости)
            float NdotL = max(dot(finalNormal, lightDir), 0.0);
            float NdotVD = max(dot(finalNormal, viewDir), 0.0);

            // ===================        shadows calc        =====================

            /*dirLightsShadowCoeff += calcDirLightShadow(
                directionalLights[i],
                vsIn.fragPos,
                finalNormal,
                sgmat_shadowMap2DSamplers[i]
            ) * NdotL * radiance + radiance * 0.04;*/

            // ====================================================================

            // cooktorrance func: DFG /

            // NDF (normal distribution func)
            float D = GGXTR(
                finalNormal,
                halfWayDir,
                roughness
            );// TRUE

            float cosTheta = max(dot(halfWayDir, viewDir), 0.0);

            // это по сути зеркальная часть (kS)
            vec3 F = SchlickFresnel(cosTheta, F0);// kS
            // geometry function
            float G = GeometrySmith(finalNormal, NdotVD, NdotL, roughness);// TRUE

            vec3 diffuse = vec3(1.0) - F;
            diffuse *= (1.0 - metalness);// check diffuse color higher

            vec3 ctNumerator = D * F * G;
            float ctDenominator = 4.0 * NdotVD * NdotL;
            vec3 specular = (ctNumerator / max(ctDenominator, 0.001)) * materialSpecularCol.rgb;

            lo += (diffuse * albedo.rgb / PI + specular) * radiance;
        }

        // dirLightsShadowCoeff /= DIRECTIONAL_LIGHTS_COUNT;

        /*vec3 resultPhong = vec3(0.0);
        vec3 resultDiffuse = vec3(0.0);
        vec3 resultSpecular = vec3(0.0);

        for (int i = 0; i < DIRECTIONAL_LIGHTS_COUNT; i++)
        {
            vec3 lightDir = normalize(directionalLights[i].position - vsIn.fragPos);
            float diff = max(dot(finalNormal, lightDir), 0.0);
            vec3 diffuse = vec3(diff);

            // Specular
            float specularStrength = 0.5f;
            vec3 reflectDir = reflect(-lightDir, finalNormal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = vec3(specularStrength * spec);

            // diffuse += vec3(shadowCoeff);

            // shadowCoeff *= diff * 1000.0;

            resultDiffuse += diffuse;
            resultSpecular += specular;
            // resultPhong += (diffuse + specular);
        }

        // resultDiffuse += vec3(shadowCoeff);
        resultDiffuse = saturate(resultDiffuse);
        resultPhong = resultDiffuse + resultSpecular;*/

        vec3 ambient = vec3(0.22) * albedo.rgb * ao;
        vec3 finalCol = materialAmbientCol.rgb + ambient + lo;
        float exposure = 1.3;

        // finalCol *= dirLightsShadowCoeff;

        // HDR standard tonemapper
        finalCol = finalCol / (finalCol + vec3(1.0));
        finalCol = pow(finalCol, vec3(1.0 / exposure));

        fragColor0.a = diffuseColor.a;
        fragColor0.rgb = finalCol;

        fragColor1 = vec4(finalCol, diffuseColor.a);
        // fragColor = vec4(1.0);

        // DEBUG ==================================
        // base color
        // fragColor.rgb = albedo; // PASSED
        // fragColor.rgb = vec3(metalness); // PASSED
        // fragColor.rgb = vec3(roughness); // PASSED
        // fragColor.rgb = normalMapColor; // PASSED
        // fragColor.rgb = vec3(ao); // PASSED
    }
#endif
