// TODO: make optimization

#sg_include "../light_structs_decl.glsl"
#sg_include "../uniform_bufs_decl.glsl"

/*in int gl_FrontFacing;
in vec4 gl_FragCoord;*/
#define PI 3.1415926

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

        //if(!gl_FrontFacing) vsOut.normal *= -1.0;

        vsOut.fragPos = vec3(objectModelMatrix * vec4(positionsAttribute, 1.0));

        vec3 T = normalize(vec3(objectModelMatrix * vec4(tangentsAttribute,   0.0)));
        vec3 B = normalize(vec3(objectModelMatrix * vec4(bitangentsAttribute, 0.0)));
        vec3 N = normalize(vec3(objectModelMatrix * vec4(normalsAttribute,    0.0)));
        vsOut.TBN = mat3(T, B, N);

        gl_Position = projectionMatrix * viewMatrix * vec4(vsOut.fragPos, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    float saturate(float a) { return clamp(a, 0.0, 1.0); }
    vec3 saturate(vec3 a) { return clamp(a, vec3(0.0), vec3(1.0)); }

    vec3 ACESFilm(vec3 x)
    {
        float a = 2.51f;
        float b = 0.03f;
        float c = 2.43f;
        float d = 0.59f;
        float e = 0.14f;
        return saturate((x*(a*x+b))/(x*(c*x+d)+e));
    }

    // shadows impl
    #if defined(SHADOWS_CASTERS_NUM) && defined(sgmat_shadowMap_MAX_TEXTURES_NUM)
/**const vec2 poissonDisk[32] = vec2[] (
    vec2(0.06407013, 0.05409927),
    vec2(0.7366577, 0.5789394),
    vec2(-0.6270542, -0.5320278),
    vec2(-0.4096107, 0.8411095),
    vec2(0.6849564, -0.4990818),
    vec2(-0.874181, -0.04579735),
    vec2(0.9989998, 0.0009880066),
    vec2(-0.004920578, -0.9151649),
    vec2(0.1805763, 0.9747483),
    vec2(-0.2138451, 0.2635818),
    vec2(0.109845, 0.3884785),
    vec2(0.06876755, -0.3581074),
    vec2(0.374073, -0.7661266),
    vec2(0.3079132, -0.1216763),
    vec2(-0.3794335, -0.8271583),
    vec2(-0.203878, -0.07715034),
    vec2(0.5912697, 0.1469799),
    vec2(-0.88069, 0.3031784),
    vec2(0.5040108, 0.8283722),
    vec2(-0.5844124, 0.5494877),
    vec2(0.6017799, -0.1726654),
    vec2(-0.5554981, 0.1559997),
    vec2(-0.3016369, -0.3900928),
    vec2(-0.5550632, -0.1723762),
    vec2(0.925029, 0.2995041),
    vec2(-0.2473137, 0.5538505),
    vec2(0.9183037, -0.2862392),
    vec2(0.2469421, 0.6718712),
    vec2(0.3916397, -0.4328209),
    vec2(-0.03576927, -0.6220032),
    vec2(-0.04661255, 0.7995201),
    vec2(0.4402924, 0.3640312)
);*/

/**const vec2 poissonDisk[16] = vec2[] (
    vec2(0.5912697, 0.1469799),
    vec2(-0.88069, 0.3031784),
    vec2(0.5040108, 0.8283722),
    vec2(-0.5844124, 0.5494877),
    vec2(0.6017799, -0.1726654),
    vec2(-0.5554981, 0.1559997),
    vec2(-0.3016369, -0.3900928),
    vec2(-0.5550632, -0.1723762),
    vec2(0.925029, 0.2995041),
    vec2(-0.2473137, 0.5538505),
    vec2(0.9183037, -0.2862392),
    vec2(0.2469421, 0.6718712),
    vec2(0.3916397, -0.4328209),
    vec2(-0.03576927, -0.6220032),
    vec2(-0.04661255, 0.7995201),
    vec2(0.4402924, 0.3640312)
);*/

/**vec2 poissonDisk[59] = vec2[](
    vec2(0.01020043f, 0.3103616f),
    vec2(-0.4121873f, -0.1701329f),
    vec2(0.4333374f, 0.6148015f),
    vec2(0.1092096f, -0.2437763f),
    vec2(0.6641068f, -0.1210794f),
    vec2(-0.1726627f, 0.8724736f),
    vec2(-0.8549297f, 0.2836411f),
    vec2(0.5146544f, -0.6802685f),
    vec2(0.04769185f, -0.879628f),
    vec2(-0.9373617f, -0.2187589f),
    vec2(-0.69226f, -0.6652822f),
    vec2(0.9230682f, 0.3181772f),
    vec2(-0.1565961f, 0.8773971f),
    vec2(-0.5258075f, 0.3916658f),
    vec2(0.515902f, 0.3077986f),
    vec2(-0.006838934f, 0.2577735f),
    vec2(-0.9315282f, -0.04518054f),
    vec2(-0.3417063f, -0.1195169f),
    vec2(-0.3221133f, -0.8118886f),
    vec2(0.425082f, -0.3786222f),
    vec2(0.3917231f, 0.9194779f),
    vec2(0.8819267f, -0.1306234f),
    vec2(-0.7906089f, -0.5639677f),
    vec2(0.2073919f, -0.9611396f),
    vec2(-0.05151585f, 0.3436534f),
    vec2(0.3648908f, 0.2827295f),
    vec2(-0.2478754f, 0.186921f),
    vec2(0.1171809f, 0.1482293f),
    vec2(-0.1496224f, 0.6990415f),
    vec2(-0.456594f, 0.378567f),
    vec2(-0.4242465f, -0.001935145f),
    vec2(-0.1889321f, -0.2015685f),
    vec2(0.1480272f, 0.6432338f),
    vec2(-0.5046303f, 0.8245607f),
    vec2(0.001617888f, 0.9789896f),
    vec2(-0.6228038f, 0.5963655f),
    vec2(0.4185582f, 0.7959766f),
    vec2(0.06965782f, -0.1184023f),
    vec2(-0.8310863f, 0.2197417f),
    vec2(-0.869589f, 0.4893173f),
    vec2(-0.6366982f, -0.357598f),
    vec2(-0.2509329f, -0.5531961f),
    vec2(-0.03994134f, -0.4170877f),
    vec2(-0.675245f, -0.0009701257f),
    vec2(0.3373009f, -0.4531572f),
    vec2(0.3022793f, -0.02336982f),
    vec2(0.6078352f, 0.5235748f),
    vec2(-0.9277961f, -0.05385896f),
    vec2(0.3847639f, -0.7718652f),
    vec2(0.5278201f, -0.168486f),
    vec2(0.1269102f, -0.8461399f),
    vec2(0.7260014f, -0.4588331f),
    vec2(-0.8775687f, -0.450681f),
    vec2(-0.574103f, -0.7766181f),
    vec2(0.6930821f, 0.2592674f),
    vec2(-0.3360346f, -0.8594083f),
    vec2(-0.2591985f, 0.9300818f),
    vec2(0.939391f, -0.2374034f),
    vec2(0.8332635f, 0.01952092f)

);*/

        const vec2 poissonDisk[32] = vec2[] (
            vec2( -0.94201624, -0.39906216 ),
            vec2( 0.94558609, -0.76890725 ),
            vec2( -0.094184101, -0.92938870 ),
            vec2( 0.34495938, 0.29387760 ),
            vec2( -0.91588581, 0.45771432 ),
            vec2( -0.81544232, -0.87912464 ),
            vec2( -0.38277543, 0.27676845 ),
            vec2( 0.97484398, 0.75648379 ),
            vec2( 0.44323325, -0.97511554 ),
            vec2( 0.53742981, -0.47373420 ),
            vec2( -0.26496911, -0.41893023 ),
            vec2( 0.79197514, 0.19090188 ),
            vec2( -0.24188840, 0.99706507 ),
            vec2( -0.81409955, 0.91437590 ),
            vec2( 0.19984126, 0.78641367 ),
            vec2( 0.14383161, -0.14100790 ),


            vec2(-0.8682281, -0.1990303),
            vec2(-0.01973724, 0.6478714),
            vec2(-0.3897587, -0.4665619),
            vec2(-0.7416366, -0.4377831),
            vec2(-0.5523247, 0.4272514),
            vec2(-0.5325066, 0.8410385),
            vec2(0.3085465, -0.7842533),
            vec2(0.8400612, -0.200119),
            vec2(0.6632416, 0.3067062),
            vec2(-0.4462856, -0.04265022),
            vec2(0.06892014, 0.812484),
            vec2(0.5149567, -0.7502338),
            vec2(0.6464897, -0.4666451),
            vec2(-0.159861, 0.1038342),
            vec2(0.6455986, 0.04419327),
            vec2(-0.7445076, 0.5035095)
        );

    float random(vec2 uv)
    {
        return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453123);
    }

    float mad(const in float mval, const in float aval, const in float bval)
    {
        return mval * aval + bval;
    }

    //const float shadowsBias = 0.000003;
    const float shadowsBias = 0.000035;

    float sampleShadowMap(
        const in vec2 coords,
        const in int shadowsCasterIdx,
        const in float compare,
        const in float lightMinCoeff
    )
    {
        return texture(shadowsCastersShadowMaps[shadowsCasterIdx], coords).z < compare ? lightMinCoeff : 1.0;
        //return step(compare, texture2D(shadowsCastersShadowMaps[shadowsCasterIdx], coords).z);
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

    vec2 rotate(vec2 pos, vec2 rotTrig)
    {
        return vec2(pos.x * rotTrig.x - pos.y * rotTrig.y, pos.y * rotTrig.x + pos.x * rotTrig.y);
    }

    // -------------------------------------------------
    // PCSS FUNCTIONS ----------------------------------

    vec2 vogelDiskSample(int sampleIndex, int samplesCount, float phi)
    {
        float GoldenAngle = 2.4f;

        float r = sqrt(float(sampleIndex) + 0.5f) / sqrt(float(samplesCount));
        float theta = sampleIndex * GoldenAngle + phi;

        float sine = sin(theta);
        float cosine = cos(theta);

        return vec2(r * cosine, r * sine);
    }

    float interleavedGradientNoise(vec2 position_screen)
    {
        vec3 magic = vec3(0.06711056f, 0.00583715f, 52.9829189f);
        return fract(magic.z * fract(dot(position_screen, magic.xy)));
    }

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

        float finalBias = max(0.05 * (1.0 - dot(normal, shadowsCasters[shadowsCasterIdx].position)), shadowsBias);

        float searchWidth = lightSizeUV * (finalProjZ - nearPlane) / shadowsCasters[shadowsCasterIdx].position.z;

        float blockers = 0.0;
        float avgBlocker = 0.0;

        float shadow = 0.0;

        float rand = random(projCoords.xy);
        rand = mad(rand, 2.0, -1.0);
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

        /**float pcfShadow = calculatePCF(projCoords, shadowsCasterIdx, 0.75, texelSize);

        return pcfShadow;*/
        // -----------------------

        // PCSS ------------------

        float pcssShadow = calculatePCSS(normal, shadowsCasterSpaceFragPos, projCoords, shadowsCasterIdx, texelSize, 0.55);

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

        diffuseColor = vec3(finalDiffuse) * lightColor.rgb / distance(fragPos, lightDir);

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
