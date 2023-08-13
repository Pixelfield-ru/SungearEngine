// TODO: make optimization

in int gl_FrontFacing;
in vec4 gl_FragCoord;

layout(std140, location = 0) uniform ObjectMatrices
{
    mat4 objectModelMatrix;
};

layout(std140, location = 1) uniform ViewMatrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    vec3 viewDirection;
};

struct ShadowsCaster
{
    mat4 shadowsCasterSpace;
    vec3 position;
};

struct DirectionalLight
{
    vec3 position;
    vec4 color;
};

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

// shadows impl
#if defined(SHADOWS_CASTERS_NUM) && defined(sgmat_shadowMap_MAX_TEXTURES_NUM)
    vec2 poissonDisk[4] = vec2[] (
        vec2( -0.94201624, -0.39906216 ),
        vec2( 0.94558609, -0.76890725 ),
        vec2( -0.094184101, -0.92938870 ),
        vec2( 0.34495938, 0.29387760 )
    );

    float bias = 0.00003;

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

        if(gl_FrontFacing == 0) normal *= -1;

        vec3 shadowCasterDir = normalize(shadowsCasters[shadowsCasterIdx].position -
        fragPos);
        float shadowFactor = dot(normal, shadowCasterDir);

        if(shadowFactor < 0.0)
        {
            return 1.0;
        }

        /*float closestDepth = texture(shadowsCastersShadowMaps[shadowsCasterIdx], projCoords.xy).r;
        float currentDepth = projCoords.z;
        float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
        return 1.0 - shadow / 1.5;*/

        float shadowVisibility = 1.0;

        for(int i = 0; i < 4; i++)
        {
            // todo: make random poisson
            float depthFactor = texture(shadowsCastersShadowMaps[shadowsCasterIdx],
            projCoords.xy + poissonDisk[i] / 3000.0).z;
            depthFactor = depthFactor < projCoords.z - bias ?
            depthFactor : 0.0;

            shadowVisibility -= 0.2 * depthFactor;
        }

        return shadowVisibility / 1.5;
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
        float finalDiffuse = max(dot(normal, lightDir), 0.0) * 5;

        diffuseColor = vec3(finalDiffuse) * lightColor.rgb;

        vec3 viewDir = normalize(viewDirection - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);

        specularColor = 4.0 * spec * lightColor.rgb;
    }
#endif

#ifdef FRAGMENT_SHADER
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
            colorFromNormalMap = normalize(colorFromNormalMap * 2.0 - 1.0);
            colorFromNormalMap = normalize(vsIn.TBN * colorFromNormalMap);
        #endif

        fragColor = colorFromBase;

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

            fragColor.rgb *= vec3(ambient) + (finalDiffuse * colorFromDiffuse.rgb) + finalSpecular;
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
    }
#endif
