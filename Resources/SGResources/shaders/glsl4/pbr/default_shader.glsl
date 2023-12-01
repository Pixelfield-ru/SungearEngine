// TODO: make optimization

#include "../light_structs_decl.glsl"
#include "../uniform_bufs_decl.glsl"
#include "../random.glsl"
#include "../disks.glsl"
#include "../math.glsl"
#include "../defines.glsl"
#include "../color_correction/aces.glsl"

/*in int gl_FrontFacing;
in vec4 gl_FragCoord;*/

float ambient = 0.1;

uniform int DIRECTIONAL_LIGHTS_COUNT;

uniform ShadowsCaster shadowsCasters[SHADOWS_CASTERS_MAX_COUNT];

uniform DirectionalLight directionalLights[DIRECTIONAL_LIGHTS_MAX_COUNT];

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
    const float shadowsBias = 0.000025;
    //const float shadowsBias = 0.0002;

    float sampleShadowMap(
        const in vec2 coords,
        const in sampler2D shadowMao,
        const in float compare,
        const in float lightMinCoeff
    )
    {
        return texture(shadowMao, coords).z < compare ? lightMinCoeff : 1.0;
    }

    // PCF FUNCTIONS ------------------------------

    float calculateLinearDepth(
        const in vec3 projCoords,
        const in sampler2D shadowMap,
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

        float blTexel = sampleShadowMap(startTexel, shadowMap, finalProjZ, lightMinCoeff);
        float brTexel = sampleShadowMap(startTexel + vec2(texelSize.x, 0.0), shadowMap, finalProjZ, lightMinCoeff);
        float tlTexel = sampleShadowMap(startTexel + vec2(0.0, texelSize.y), shadowMap, finalProjZ, lightMinCoeff);
        float trTexel = sampleShadowMap(startTexel + texelSize, shadowMap, finalProjZ, lightMinCoeff);

        float mixA = mix(blTexel, tlTexel, fractPart.y);
        float mixB = mix(brTexel, trTexel, fractPart.y);

        return mix(mixA, mixB, fractPart.x);
    }

    float calculatePCF(
        const in vec3 projCoords,
        const in sampler2D shadowMap,
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
                                    shadowMap, texelSize, lightMinCoeff, shadowsCasterSpaceFragPos);
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
        const in sampler2D shadowMap,
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

            float occluder = texture(shadowMap, projCoords.xy + offset).r;

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
            if(texture(shadowMap,
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
        const in vec3 normal,
        const in sampler2D shadowMap,
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

        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

        // PCF ------------------

        /*float pcfShadow = calculatePCF(projCoords, shadowMap, 0.35, texelSize, shadowsCasterSpaceFragPos);

        return pcfShadow;*/

        // -----------------------

        // PCSS ------------------

        /*float pcssShadow = calculatePCSS(normal, shadowsCasterSpaceFragPos, fragPos, projCoords, shadowMap, shadowsCasterIdx, texelSize, 0.4);

        return pcssShadow;*/

        // -----------------------

            const float shadowsMinCoeff = 0.2;
            const int samplesNum = 16;

            float visibility = 1.0;
            const float downstep = (1.0 - shadowsMinCoeff) / samplesNum;

            float rand = random(projCoords.xy);
            float rotAngle = rand * PI;
            vec2 rotTrig = vec2(cos(rotAngle), sin(rotAngle));

            for(int i = 0; i < samplesNum; i++)
            {
                if(texture(shadowMap, projCoords.xy + rotate(poissonDisk[i], rotTrig) / 750.0).z < projCoords.z - shadowsBias)
                {
                    visibility -= downstep;
                }
            }

            return visibility;

        // VSM (VARIANCE SHADOW MAPPING) -------------------

        /*vec2 moments = texture(shadowMap, projCoords.xy).rg;

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

    out vec4 fragColor;

    uniform int sgmat_diffuseSamplers_COUNT = 0;
    uniform int sgmat_diffuseRoughnessSamplers_COUNT = 0;
    uniform int sgmat_normalsSamplers_COUNT = 0;
    uniform int sgmat_shadowMapSamplers_COUNT = 0;

    uniform sampler2D sgmat_diffuseSamplers[SGMAT_SAMPLERS_OF_TYPE_MAX];

    uniform sampler2D sgmat_diffuseRoughnessSamplers[SGMAT_SAMPLERS_OF_TYPE_MAX];

    uniform sampler2D sgmat_normalsSamplers[SGMAT_SAMPLERS_OF_TYPE_MAX];

    uniform sampler2D sgmat_shadowMapSamplers[SHADOWS_CASTERS_MAX_COUNT];

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
    float GeometrySmith(const in vec3 normal, const in float cosTheta, const in float NdotVD, const in float NdotL, float roughness)
    {
        // косинус между направлением камеры и нормалью к поверхности
        // float NdotVD = _NdotVD;
        // косинус между направлением источника света и нормалью к поверхности
        // float NdotL = _NdotL;

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

        {
            float mixCoeff = 1.0 / sgmat_diffuseSamplers_COUNT;

            for (int i = 0; i < sgmat_diffuseSamplers_COUNT; ++i)
            {
                diffuseColor += texture(sgmat_diffuseSamplers[i], finalUV) * mixCoeff;
            }
        }

        {
            float mixCoeff = 1.0 / sgmat_diffuseRoughnessSamplers_COUNT;

            for (int i = 0; i < sgmat_diffuseRoughnessSamplers_COUNT; ++i)
            {
                diffuseRoughnessColor += texture(sgmat_diffuseRoughnessSamplers[i], finalUV) * mixCoeff;
            }
        }

        if(sgmat_normalsSamplers_COUNT > 0)
        {
            float mixCoeff = 1.0 / sgmat_normalsSamplers_COUNT;

            for (int i = 0; i < sgmat_normalsSamplers_COUNT; ++i)
            {
                normalMapColor += texture(sgmat_normalsSamplers[i], finalUV).rgb * mixCoeff;
            }

            normalMapColor = normalize(vsIn.TBN * (normalMapColor * 2.0 - 1.0));
        }
        else
        {
            normalMapColor = normalizedNormal;
        }

        float shadowCoeff = 1.0;

        for (int i = 0; i < SHADOWS_CASTERS_MAX_COUNT && i < sgmat_shadowMapSamplers_COUNT; ++i)
        {
            float calculatedShadowCoeff = 1.0 - calculateShadow(
                shadowsCasters[i].shadowsCasterSpace * vec4(vsIn.fragPos, 1.0),
                vsIn.fragPos,
                normalMapColor,
                sgmat_shadowMapSamplers[i],
                i
            );

            if(!gl_FrontFacing) shadowCoeff += calculatedShadowCoeff;
            else shadowCoeff -= calculatedShadowCoeff;
        }

        // todo: fix multiple directional lights
        // TODO: make customizable with defines
        // PBR pipeline (using Cook-Torrance BRDF) --------------------

        vec3 viewDir = normalize(viewDirection - vsIn.fragPos);

        // colorFromRoughness.r = AO MAP
        // colorFromRoughness.g = ROUGHNESS
        // colorFromRoughness.b = METALNESS

        vec3 albedo =       diffuseColor.rgb;
        float ao =          diffuseRoughnessColor.r;
        float metalness =   diffuseRoughnessColor.b * materialMetallicFactor;
        float roughness =   diffuseRoughnessColor.g * materialRoughnessFactor;

        // для формулы Шлика-Френеля
        vec3 F0 = vec3(0.1);
        F0 = mix(F0, albedo, metalness);

        //float geomRoughness = ((colorFromRoughness.g + 1.0) * (colorFromRoughness.g + 1.0)) / 8.0;

        // TODO: MOVE TO LIGHT COMPONENT
        const float lightIntensity = 300.0;

        vec3 lo = vec3(0.0);
        for (int i = 0; i < DIRECTIONAL_LIGHTS_COUNT; i++)
        {
            vec3 lightDir = normalize(directionalLights[i].position - vsIn.fragPos);
            vec3 halfWayDir = normalize(lightDir + viewDir);

            float distance = length(directionalLights[i].position - vsIn.fragPos);
            float attenuation = 1.0 / (distance * distance);
            vec3 radiance = directionalLights[i].color.rgb * attenuation * lightIntensity;
            //vec3 radiance = directionalLights[i].color.rgb;

            // energy brightness coeff (коэфф. энергетической яркости)
            float NdotL = max(dot(normalMapColor, lightDir), 0.0);
            float NdotVD = max(dot(normalMapColor, viewDir), 0.0);

            // shadowCoeff += NdotL * attenuation * lightIntensity;

            // cooktorrance func: DFG /

            // NDF (normal distribution func)
            float D = GGXTR(
                normalMapColor,
                halfWayDir,
                roughness
            );

            float cosTheta = max(dot(halfWayDir, viewDir), 0.0);

            vec3 F = SchlickFresnel(cosTheta, F0);
            // geometry function
            float G = GeometrySmith(normalMapColor, cosTheta, NdotVD, NdotL, roughness);

            vec3 diffuse = vec3(1.0) - F;
            diffuse *= (1.0 - metalness) * materialDiffuseCol.rgb;
            //diffuse *= max((1.0 - metalness) * materialDiffuseCol.rgb, vec3(115.0 / 255.0, 133.0 / 255.0, 179.0 / 255.0) / 2.0);

            vec3 ctNumerator = D * F * G;
            float ctDenominator = 4.0 * NdotVD * NdotL;
            vec3 specular = (ctNumerator / max(ctDenominator, 0.001)) * materialSpecularCol.rgb * 2.0;

            lo += (diffuse * albedo.rgb / PI + specular) * radiance * NdotL;

            // shadowCoeff *= NdotL / attenuation * lightIntensity;
            // shadowCoeff *= (1.0 - NdotL);
        }

        shadowCoeff = saturate(shadowCoeff);

        vec3 ambient = vec3(0.05) * albedo.rgb * ao;
        vec3 finalCol = materialAmbientCol.rgb + ambient + lo;

        // HDR standard tonemapper
        finalCol = finalCol / (finalCol + vec3(1.0));
        finalCol = pow(finalCol, vec3(1.0 / 2.2));

        fragColor.a = diffuseColor.a;
        fragColor.rgb = finalCol * shadowCoeff;

        // shadows apply ---------------------------------------
    }
#endif
