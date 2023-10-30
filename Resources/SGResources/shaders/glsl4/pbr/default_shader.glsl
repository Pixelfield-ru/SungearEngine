// TODO: make optimization

#include "../light_structs_decl.glsl"
#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"
#include "../random.glsl"
#include "../disks.glsl"
#include "../math.glsl"

/*in int gl_FrontFacing;
in vec4 gl_FragCoord;*/

float ambient = 0.1;

#ifdef SHADOWS_CASTERS_NUM
    uniform ShadowsCaster shadowsCasters[SHADOWS_CASTERS_NUM];
    uniform sampler2D shadowsCastersShadowMaps[SHADOWS_CASTERS_NUM];
#endif

#ifdef DIRECTIONAL_LIGHTS_NUM
    uniform DirectionalLight directionalLights[DIRECTIONAL_LIGHTS_NUM];
#endif

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

        gl_Position = projectionMatrix * viewMatrix * vec4(vsOut.fragPos, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    // shadows impl
    #if defined(SHADOWS_CASTERS_NUM) && defined(sgmat_shadowMap_MAX_TEXTURES_NUM)

    const float shadowsBias = 0.0000165;
    //const float shadowsBias = 0.0002;

    float sampleShadowMap(
        const in vec2 coords,
        const in int shadowsCasterIdx,
        const in float compare,
        const in float lightMinCoeff
    )
    {
        return texture(shadowsCastersShadowMaps[shadowsCasterIdx], coords).z < compare ? lightMinCoeff : 1.0;
    }

    // PCF FUNCTIONS ------------------------------

    float calculateLinearDepth(
        const in vec3 projCoords,
        const in int shadowsCasterIdx,
        const in vec2 texelSize,
        const in float lightMinCoeff,
        const in vec4 shadowsCasterSpaceFragPos
    )
    {
        float finalProjZ = projCoords.z - shadowsBias;

        float shadow = 0.0;

        vec2 pixelPos = projCoords.xy / texelSize;
        vec2 fractPart = fract(pixelPos);
        vec2 startTexel = (pixelPos - fractPart) * texelSize;

        float blTexel = sampleShadowMap(startTexel, shadowsCasterIdx, finalProjZ, lightMinCoeff);
        float brTexel = sampleShadowMap(startTexel + vec2(texelSize.x, 0.0), shadowsCasterIdx, finalProjZ, lightMinCoeff);
        float tlTexel = sampleShadowMap(startTexel + vec2(0.0, texelSize.y), shadowsCasterIdx, finalProjZ, lightMinCoeff);
        float trTexel = sampleShadowMap(startTexel + texelSize, shadowsCasterIdx, finalProjZ, lightMinCoeff);

        float mixA = mix(blTexel, tlTexel, fractPart.y);
        float mixB = mix(brTexel, trTexel, fractPart.y);

        return mix(mixA, mixB, fractPart.x);
    }

    float calculatePCF(
        const in vec3 projCoords,
        const in int shadowsCasterIdx,
        const in float lightMinCoeff,
        const in vec2 texelSize,
        const in vec4 shadowsCasterSpaceFragPos
    )
    {
        const float samplesNum = 5.0;
        const float samplesStart = (samplesNum - 1.0) / 2.0;
        const float samplesNumSquared = samplesNum * samplesNum;

        float resShadow = 0.0;

        const int sn = 9;

        for(float y = -samplesStart; y <= samplesStart; y += 1.0)
        {
            for(float x = -samplesStart; x <= samplesStart; x += 1.0)
            {
                vec2 offset = vec2(x, y) * texelSize;
                resShadow += calculateLinearDepth(vec3(projCoords.xy + offset, projCoords.z),
                    shadowsCasterIdx, texelSize, lightMinCoeff, shadowsCasterSpaceFragPos);
            }
        }

        return resShadow / samplesNumSquared;
    }

    // -------------------------------------------------
    // PCSS FUNCTIONS ----------------------------------

    float calculatePCSS(
        const in vec3 normal,
        const in vec4 shadowsCasterSpaceFragPos,
        vec3 fragPos,
        const in vec3 projCoords,
        const in int shadowsCasterIdx,
        const in vec2 texelSize,
        const in float lightMinCoeff
    )
    {
        //float finalBias = max(0.05 * (1.0 - dot(normal, shadowsCasters[shadowsCasterIdx].position)), shadowsBias);
        //float finalBias = 0.00005;

        vec3 lightDir = normalize(shadowsCasters[shadowsCasterIdx].position - fragPos);
        float cosTheta = clamp(dot(normal, lightDir), 0.0, 1.0);
        //float finalBias = clamp(0.000007 * tan(acos(cosTheta)), 0.0, 0.01);
        float finalBias = 0.0;

        float finalProjZ = projCoords.z;

        const int numBlockerSamples = 16;
        const int numAASamples = 16;
        const float nearPlane = 1.1;
        const float lightWorldSize = 10.0;
        const float lightFrustumWidth = 3.75;
        const float lightSizeUV = lightWorldSize / lightFrustumWidth;
        //const float lightSizeUV = lightFrustumWidth / lightWorldSize;

        //float finalBias = shadowsBias;
        //float searchWidth = lightSizeUV * (finalProjZ - nearPlane) / shadowsCasterSpaceFragPos.z;
        float searchWidth = lightSizeUV * (finalProjZ - nearPlane) / finalProjZ;

        float blockers = 0.0;
        float avgBlocker = 0.0;

        float shadow = 0.0;

        float rand = random(projCoords.xy);
        //rand = mad(rand, 2.0, -1.0);
        float rotAngle = rand * PI;
        vec2 rotTrig = vec2(cos(rotAngle), sin(rotAngle));

        // get blocker distance
        for (int j = 0; j < numBlockerSamples; ++j)
        {
            vec2 offset = poissonDisk[j] * searchWidth;

            float occluder = texture(shadowsCastersShadowMaps[shadowsCasterIdx], projCoords.xy + offset).r;

            if(occluder < finalProjZ - finalBias)
            {
                blockers += 1.0;
                avgBlocker += occluder;
            }
        }

        if(blockers < 1.0)
        {
            return 1.0;
        }

        avgBlocker /= blockers;

        float penumbraSize = (finalProjZ - avgBlocker) / avgBlocker;
        float filterRadiusUV = penumbraSize * lightSizeUV * nearPlane / finalProjZ;

        float visibility = 1.0;
        float m = (visibility - lightMinCoeff) / numAASamples;

        for(int i = 0; i < numAASamples; i++)
        {
            if(texture(shadowsCastersShadowMaps[shadowsCasterIdx],
            projCoords.xy + rotate(poissonDisk[i] * filterRadiusUV, rotTrig)).z < finalProjZ - finalBias)
            {
                visibility -= m;
            }
        }

        return visibility;
    }

    // --------------------------------------------------

    float calculateShadow(
        const in vec4 shadowsCasterSpaceFragPos,
        const in vec3 fragPos,
                 vec3 normal,
        const in int shadowsCasterIdx
    )
    {
        vec3 projCoords = shadowsCasterSpaceFragPos.xyz / shadowsCasterSpaceFragPos.w;
        projCoords = projCoords * 0.5 + 0.5;

        if(projCoords.z > 1.0)
        {
            return 1.0;
        }

        /*float ct = dot(normalize(shadowsCasters[shadowsCasterIdx].position -
        fragPos), normal);

        // make customizable
        if(ct < 0.125)
        {
            return 1.0;
        }*/

        //if(!gl_FrontFacing) normal *= -1;
        /*if(!gl_FrontFacing) normal *= -1;

        vec3 shadowCasterDir = normalize(shadowsCasters[shadowsCasterIdx].position -
                                         fragPos);
        float shadowFactor = dot(normal, shadowCasterDir);

        if(shadowFactor < 0.0)
        {
            return 1.0;
        }*/

        vec2 texelSize = 1.0 / textureSize(shadowsCastersShadowMaps[shadowsCasterIdx], 0);

        // PCF ------------------

        /*float pcfShadow = calculatePCF(projCoords, shadowsCasterIdx, 0.35, texelSize, shadowsCasterSpaceFragPos);

        return pcfShadow;*/

        // -----------------------

        // PCSS ------------------

        /*float pcssShadow = calculatePCSS(normal, shadowsCasterSpaceFragPos, fragPos, projCoords, shadowsCasterIdx, texelSize, 0.4);

        return pcssShadow;*/

        // -----------------------

        const float shadowsMinCoeff = 0.55;
        const int samplesNum = 24;

        float visibility = 1.0;
        const float downstep = (1.0 - shadowsMinCoeff) / samplesNum;

        float rand = random(projCoords.xy);
        rand = mad(rand, 2.0, -1.0);
        float rotAngle = rand * PI;
        vec2 rotTrig = vec2(cos(rotAngle), sin(rotAngle));

        for(int i = 0; i < samplesNum; i++)
        {
            if(texture(shadowsCastersShadowMaps[shadowsCasterIdx], projCoords.xy + rotate(poissonDisk[i], rotTrig) / 1500.0).z < projCoords.z - shadowsBias)
            {
                visibility -= downstep;
            }
        }

        return visibility;

        // VSM (VARIANCE SHADOW MAPPING) -------------------

        /*vec2 moments = texture(shadowsCastersShadowMaps[shadowsCasterIdx], projCoords.xy).rg;

        float p = 0.0;
        //float depth = length(shadowsCasters[shadowsCasterIdx].position - fragPos);
        float depth = projCoords.z;

        if (depth <= moments.x)
        {
            return 1.0;
        }

        float variance = moments.y - (moments.x * moments.x);
        variance = max(variance, 0.00001);

        float d = depth - moments.x;
        float pmax = variance / (variance + d * d);

        return pmax;*/

        // -------------------------------------------------
    }
    #endif

    #ifdef DIRECTIONAL_LIGHTS_NUM
        void BlinnPhong_calculateDiffuseAndSpecularColor(
            const in vec3 normal,
            const in vec3 lightPos,
            const in vec3 fragPos,
            const in vec4 lightColor,
            out vec3 diffuseColor,
            out vec3 specularColor
        )
        {
            vec3 lightDir = normalize(lightPos - fragPos);
            vec3 viewDir = normalize(viewDirection - fragPos);
            vec3 halfWayDir = normalize(lightDir + viewDir);

            float finalDiffuse = max(dot(normal, lightDir), 0.0) * 1.0;

            diffuseColor = vec3(finalDiffuse) * lightColor.rgb * materialDiffuseCol.rgb;

            float spec = pow(max(dot(normal, halfWayDir), 0.0), materialShininess);

            specularColor = spec * lightColor.rgb * materialSpecularCol.rgb;
        }
    #endif

    out vec4 fragColor;

    #ifdef sgmat_diffuseRoughnessSamplers_COUNT
        uniform sampler2D sgmat_diffuseRoughnessSamplers[sgmat_diffuseRoughnessSamplers_COUNT];
    #endif

    #ifdef sgmat_diffuseSamplers_COUNT
        uniform sampler2D sgmat_diffuseSamplers[sgmat_diffuseSamplers_COUNT];
    #endif

    #ifdef sgmat_normalsSamplers_COUNT
        uniform sampler2D sgmat_normalsSamplers[sgmat_normalsSamplers_COUNT];
    #endif

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
    float GeometrySmith(const in vec3 normal, const in vec3 viewDir, const in vec3 lightDir, float roughness)
    {
        // косинус между направлением камеры и нормалью к поверхности
        float NdotVD = max(dot(normal, viewDir), 0.0);
        // косинус между направлением источника света и нормалью к поверхности
        float NdotL = max(dot(normal, lightDir), 0.0);

        // ggx from view dir
        // насколько освещён фрагмент при виде от камеры
        float ggx1 = GeometryShlickGGX(NdotVD, roughness);
        // ggx from light dir
        // насколько освещён тот же фрагмент при виде от источника света
        float ggx2 = GeometryShlickGGX(NdotL, roughness);

        // если ggx1 = 1, а ggx2 = 0, то это geometry shadowing
        // если ggx1 = 0, а ggx2 = 1, то это geometry obstruction
        return ggx1 * ggx2;
    }

    // Trowbridge-Reitz GGX (Normal Distribution Function)
    float GGXTR(const in vec3 normal, const in vec3 medianVec, const in float roughness)
    {
        float roughness4 = pow(roughness, 6.0);

        // косинус между медианным вектором и нормалью фрагмента
        float NdotMV = max(dot(normal, medianVec), 0.0);
        float NdotMV2 = NdotMV * NdotMV;

        // числитель
        float numerator = roughness4;
        // заменатель
        float denominator = (NdotMV2 * (roughness4 - 1.0) + 1.0);
        denominator =  PI * denominator * denominator;

        return numerator / denominator;
    }

    vec3 SchlickFresnel(const in float cosTheta, const in vec3 F0)
    {
        return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
    }

    void main()
    {
        vec3 normalizedNormal = normalize(vsIn.normal);

        vec4 diffuseColor = vec4(0, 0, 0, 1);
        vec4 diffuseRoughnessColor = vec4(0, 0, 0, 1);
        vec3 normalMapColor = vec3(0);

        vec2 finalUV = vsIn.UV;
        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vsIn.UV.y;
        #endif

        #ifdef sgmat_diffuseSamplers_COUNT
        {
            float mixCoeff = 1.0 / sgmat_diffuseSamplers_COUNT;

            for (int i = 0; i < sgmat_diffuseSamplers_COUNT; i++)
            {
                diffuseColor += texture(sgmat_diffuseSamplers[i], finalUV) * mixCoeff;
            }
        }
        #endif

        #ifdef sgmat_diffuseRoughnessSamplers_COUNT
        {
            float mixCoeff = 1.0 / sgmat_diffuseRoughnessSamplers_COUNT;

            for (int i = 0; i < sgmat_diffuseRoughnessSamplers_COUNT; i++)
            {
                diffuseRoughnessColor += texture(sgmat_diffuseRoughnessSamplers[i], finalUV) * mixCoeff;
            }
        }
        #endif

        #ifdef sgmat_normalsSamplers_COUNT
        {
            float mixCoeff = 1.0 / sgmat_normalsSamplers_COUNT;

            for (int i = 0; i < sgmat_normalsSamplers_COUNT; i++)
            {
                normalMapColor += texture(sgmat_normalsSamplers[i], finalUV).rgb * mixCoeff;
            }

            normalMapColor = normalize(vsIn.TBN * (normalMapColor * 2.0 - 1.0));
        }
        #else
            normalMapColor = normalizedNormal;
        #endif

        // todo: fix multiple directional lights
        #ifdef DIRECTIONAL_LIGHTS_NUM
            // TODO: make customizable with defines
            // PBR pipline (using Cook-Torrance BRDF) --------------------

            vec3 viewDir = normalize(viewDirection - vsIn.fragPos);

            // colorFromRoughness.r = AO MAP
            // colorFromRoughness.g = ROUGHNESS
            // colorFromRoughness.b = METALNESS

            vec3 albedo = diffuseColor.rgb;
            float ao = diffuseRoughnessColor.r;
            float roughness = diffuseRoughnessColor.g * materialRoughnessFactor;
            float metalness = diffuseRoughnessColor.b * materialMetallicFactor;

            // для формулы Шлика-Френеля
            vec3 F0 = vec3(0.04);
            F0 = mix(F0, albedo, metalness);

            //float geomRoughness = ((colorFromRoughness.g + 1.0) * (colorFromRoughness.g + 1.0)) / 8.0;

            vec3 lo = vec3(0.0);
            for(int i = 0; i < DIRECTIONAL_LIGHTS_NUM; i++)
            {
                vec3 lightDir = normalize(directionalLights[i].position - vsIn.fragPos);
                vec3 halfWayDir = normalize(lightDir + viewDir);

                float distance = length(directionalLights[i].position - vsIn.fragPos);
                float attenuation = 1.0 / (distance * distance);
                vec3 radiance = directionalLights[i].color.rgb * attenuation * 700.0;
                //radiance = vec3(1.0);

                // energy brightness coeff (коэфф. энергетической яркости)
                float ebCoeff = max(dot(normalMapColor, lightDir), 0.0);

                // cooktorrance func: DFG /

                // NDF (normal distribution func)
                float D = GGXTR(
                    normalMapColor,
                    halfWayDir,
                    roughness
                );
                vec3 F = SchlickFresnel(max(dot(halfWayDir, viewDir), 0.0), F0);
                // geometry function
                float G = GeometrySmith(normalMapColor, viewDir, lightDir, roughness);

                vec3 diffuse = vec3(1.0) - F;
                diffuse *= (1.0 - metalness) * materialDiffuseCol.rgb;
                //diffuse *= max((1.0 - metalness) * materialDiffuseCol.rgb, vec3(115.0 / 255.0, 133.0 / 255.0, 179.0 / 255.0) / 2.0);

                vec3 ctNumerator = D * F * G;
                float ctDenominator = 4.0 * max(dot(normalMapColor, viewDir), 0.0) * ebCoeff;
                vec3 specular = (ctNumerator / max(ctDenominator, 0.001)) * materialSpecularCol.rgb;

                lo += (diffuse * albedo.rgb / PI + specular) * radiance * ebCoeff;
            }
            vec3 ambient = vec3(0.03) * albedo.rgb * ao;
            vec3 finalCol = materialAmbientCol.rgb + ambient + lo;

            // HDR standard tonemapper
            finalCol = finalCol / (finalCol + vec3(1.0));
            finalCol = pow(finalCol, vec3(1.0 / 2.2));

            fragColor.a = diffuseColor.a;
            fragColor.rgb = finalCol;

            // blinn-phong pipeline  -----------------------------

            //fragColor.rgb *= materialAmbientCol.rgb + ggxtrAccum * geomSmithAccum;
            // ----------------------------------

            /*vec3 finalDiffuse = vec3(0.0);
            vec3 finalSpecular = vec3(0.0);

            vec3 intermediateDiffuse = vec3(0.0);
            vec3 intermediateSpecular = vec3(0.0);

            for(int i = 0; i < DIRECTIONAL_LIGHTS_NUM; i++)
            {
                BlinnPhong_calculateDiffuseAndSpecularColor(
                    colorFromNormalMap,
                    directionalLights[i].position,
                    vsIn.fragPos,
                    directionalLights[i].color,
                    intermediateDiffuse,
                    intermediateSpecular
                );

                finalDiffuse += intermediateDiffuse;
                finalSpecular += intermediateSpecular;
            }

            fragColor.a = colorFromDiffuse.a;
            fragColor.rgb = colorFromBase.rgb * (materialAmbientCol.rgb + finalDiffuse + finalSpecular);*/
        #endif

        #if defined(SHADOWS_CASTERS_NUM) && defined(sgmat_shadowMap_MAX_TEXTURES_NUM)
            float shadowCoeff = 0.0;

            for(int i = 0; i < SHADOWS_CASTERS_NUM && i < sgmat_shadowMap_MAX_TEXTURES_NUM; i += 1)
            {
                fragColor.rgb *= calculateShadow(
                    shadowsCasters[i].shadowsCasterSpace * vec4(vsIn.fragPos, 1.0),
                    vsIn.fragPos,
                    normalMapColor,
                    i
                );
            }
        #endif

        //fragColor.rgb = vec3(colorFromRoughness.b);
        fragColor.rgb = ACESFilm(fragColor.rgb);
    }
#endif
