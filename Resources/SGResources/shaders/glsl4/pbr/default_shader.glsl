// TODO: make optimization

#sg_include "../light_structs_decl.glsl"
#sg_include "../uniform_bufs_decl.glsl"
#sg_include "../color_correction/aces.glsl"
#sg_include "../random.glsl"
#sg_include "../disks.glsl"
#sg_include "../math.glsl"

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

        vec3 fragPos;
        mat3 TBN;
    } vsOut;

    void main()
    {
        vsOut.UV = UVAttribute.xy;
        vsOut.normal = normalsAttribute;

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

    //const float shadowsBias = 0.000003;
    const float shadowsBias = 0.0001;

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
        const in float lightMinCoeff
    )
    {
        float finalProjZ = projCoords.z;

        float shadow = 0.0;

        vec2 pixelPos = projCoords.xy / texelSize + 0.5;
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
        const in vec2 texelSize
    )
    {
        const float samplesNum = 5.0;
        const float samplesStart = (samplesNum - 1.0) / 2.0;
        const float samplesNumSquared = samplesNum * samplesNum;

        float resShadow = 0.0;

        for(float y = -samplesStart; y <= samplesStart; y += 1.0)
        {
            for(float x = -samplesStart; x <= samplesStart; x += 1.0)
            {
                vec2 offset = vec2(x, y) * texelSize;
                resShadow += calculateLinearDepth(vec3(projCoords.xy + offset, projCoords.z), shadowsCasterIdx, texelSize, lightMinCoeff);
            }
        }

        return resShadow / samplesNumSquared;
    }

    // -------------------------------------------------
    // PCSS FUNCTIONS ----------------------------------

    float calculatePCSS(
        const in vec3 normal,
        const in vec4 shadowsCasterSpaceFragPos,
        const in vec3 projCoords,
        const in int shadowsCasterIdx,
        const in vec2 texelSize,
        const in float lightMinCoeff
    )
    {
        float finalProjZ = projCoords.z;

        const int numBlockerSamples = 16;
        const int numAASamples = 16;
        const float nearPlane = 0.2;
        const float lightWorldSize = 10.0;
        const float lightFrustumWidth = 3.75;
        const float lightSizeUV = lightWorldSize / lightFrustumWidth;
        //const float lightSizeUV = lightFrustumWidth / lightWorldSize;


        //float finalBias = max(0.05 * (1.0 - dot(normal, shadowsCasters[shadowsCasterIdx].position)), shadowsBias);

        float finalBias = shadowsBias;
        //float searchWidth = lightSizeUV * (finalProjZ - nearPlane) / shadowsCasterSpaceFragPos.z;
        float searchWidth = lightSizeUV * (finalProjZ - nearPlane) / shadowsCasters[shadowsCasterIdx].position.z;

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
            //offset = rotate(offset, rotTrig);

            float occluder = texture(shadowsCastersShadowMaps[shadowsCasterIdx], projCoords.xy + offset).r;

            if(occluder < finalProjZ - finalBias)
            {
                blockers += 1.0;
                avgBlocker += occluder;
            }
        }

        avgBlocker /= blockers;

        if(blockers < 1.0)
        {
            return 1.0;
        }

        float penumbraSize = (finalProjZ - avgBlocker) / avgBlocker;
        float filterRadiusUV = penumbraSize * lightSizeUV * nearPlane / finalProjZ;
        for(int i = 0; i < numAASamples; ++i)
        {
            vec2 offset = poissonDisk[i].xy * filterRadiusUV;
            offset = rotate(offset, rotTrig);
            shadow += calculateLinearDepth(vec3(projCoords.xy + offset, finalProjZ), shadowsCasterIdx, texelSize, lightMinCoeff);
        }

        return shadow / numAASamples;
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

        //if(!gl_FrontFacing) normal *= -1;
        /**if(!gl_FrontFacing) normal *= -1;

        vec3 shadowCasterDir = normalize(shadowsCasters[shadowsCasterIdx].position -
                                         fragPos);
        float shadowFactor = dot(normal, shadowCasterDir);

        if(shadowFactor < 0.0)
        {
            return 1.0;
        }*/

        vec2 texelSize = 1.0 / textureSize(shadowsCastersShadowMaps[shadowsCasterIdx], 0);

        // PCF ------------------

        /**float pcfShadow = calculatePCF(projCoords, shadowsCasterIdx, 0.075, texelSize);

        return pcfShadow;*/

        // -----------------------

        // PCSS ------------------

        float pcssShadow = calculatePCSS(normal, shadowsCasterSpaceFragPos, projCoords, shadowsCasterIdx, texelSize, 0.075);

        return pcssShadow;

        // -----------------------
    }
    #endif

    #ifdef DIRECTIONAL_LIGHTS_NUM
        void calculateDiffuseAndSpecularColor(
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

        float finalDiffuse = max(dot(normal, lightDir), 0.0) * 16.0;

        diffuseColor = vec3(finalDiffuse) * lightColor.rgb; /// distance(fragPos, lightDir);

        float spec = pow(max(dot(normal, halfWayDir), 0.0), 32);

        specularColor = spec * lightColor.rgb;
    }
    #endif

    out vec4 fragColor;

    uniform sampler2D sgmat_emissive0;
    uniform sampler2D sgmat_ambientOcclusion1;
    uniform sampler2D sgmat_ambient2;
    uniform sampler2D sgmat_diffuseRoughness3;
    uniform sampler2D sgmat_diffuse4;
    uniform sampler2D sgmat_displacement5;
    uniform sampler2D sgmat_height6;
    uniform sampler2D sgmat_normals7;
    uniform sampler2D sgmat_baseColor8;
    uniform sampler2D sgmat_clearcoat9;
    uniform sampler2D sgmat_emissionColor10;
    uniform sampler2D sgmat_lightmap11;
    uniform sampler2D sgmat_metalness12;
    uniform sampler2D sgmat_normalCamera13;
    uniform sampler2D sgmat_opacity14;
    uniform sampler2D sgmat_relfection15;
    uniform sampler2D sgmat_sheen16;
    uniform sampler2D sgmat_shininess17;
    uniform sampler2D sgmat_specular18;
    uniform sampler2D sgmat_transmission19;

    in VSOut
    {
        vec2 UV;
        vec3 normal;

        vec3 fragPos;
        mat3 TBN;
    } vsIn;

    void main()
    {
        vec3 normalizedNormal = normalize(vsIn.normal);

        vec4 colorFromBase = vec4(1);
        vec4 colorFromDiffuse = vec4(1);
        vec4 colorFromMetalness = vec4(1);
        vec4 colorFromRoughness = vec4(1);
        vec3 colorFromNormalMap = vec3(normalizedNormal);

        vec2 finalUV = vsIn.UV;
        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vsIn.UV.y;
        #endif

        #ifdef sgmat_metalness12_DEFINED
            colorFromMetalness = texture(sgmat_metalness12, vec2(finalUV.x, finalUV.y));
        #endif

        #ifdef sgmat_baseColor8_DEFINED
            colorFromBase = texture(sgmat_baseColor8, vec2(finalUV.x, finalUV.y));
        #endif

        #ifdef sgmat_diffuse4_DEFINED
            colorFromDiffuse = texture(sgmat_diffuse4, vec2(finalUV.x, finalUV.y));
        #endif

        #ifdef sgmat_normals7_DEFINED
            colorFromNormalMap = texture(sgmat_normals7, vec2(finalUV.x, finalUV.y)).rgb;
            colorFromNormalMap = normalize(vsIn.TBN * (colorFromNormalMap * 2.0 - 1.0));
        #endif

        fragColor = colorFromBase;
        //fragColor = vec4(1.0);

        // todo: fix multiple directional lights
        #ifdef DIRECTIONAL_LIGHTS_NUM
            vec3 finalDiffuse = vec3(0.0);
            vec3 finalSpecular = vec3(0.0);

            vec3 intermediateDiffuse = vec3(0.0);
            vec3 intermediateSpecular = vec3(0.0);

            for(int i = 0; i < DIRECTIONAL_LIGHTS_NUM; i++)
            {
                calculateDiffuseAndSpecularColor(
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

            //fragColor.rgb *= vec3(ambient) + (finalDiffuse * colorFromDiffuse.rgb) + finalSpecular;
            fragColor.rgb *= vec3(ambient) + (finalDiffuse) + finalSpecular;
        #endif

        #if defined(SHADOWS_CASTERS_NUM) && defined(sgmat_shadowMap_MAX_TEXTURES_NUM)
            float shadowCoeff = 0.0;

            for(int i = 0; i < SHADOWS_CASTERS_NUM && i < sgmat_shadowMap_MAX_TEXTURES_NUM; i += 1)
            {
                fragColor.rgb *= calculateShadow(
                    shadowsCasters[i].shadowsCasterSpace * vec4(vsIn.fragPos, 1.0),
                    vsIn.fragPos,
                    colorFromNormalMap,
                    i
                );
            }
        #endif

        fragColor.rgb = ACESFilm(fragColor.rgb);
    }
#endif
