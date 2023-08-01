struct ShadowsCaster
{
    mat4 shadowsCasterSpace;
};

#ifdef SHADOWS_CASTERS_NUM
    uniform ShadowsCaster shadowsCasters[SHADOWS_CASTERS_NUM];
    uniform sampler2D shadowsCastersShadowMaps[SHADOWS_CASTERS_NUM];
#endif


#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;

    out VSOut
    {
        vec2 UV;
        vec3 normal;

        float diffPower;
        vec3 fragPos;
    } vsOut;

    layout(std140, location = 0) uniform ObjectMatrices
    {
        mat4 objectModelMatrix;
    };

    layout(std140, location = 1) uniform ViewMatrices
    {
        mat4 projectionMatrix;
        mat4 viewMatrix;
    };

    // final yet
    vec3 lightPos = vec3(0, 20, 7);

    void main()
    {
        vsOut.UV = UVAttribute.xy;
        vsOut.normal = normalsAttribute;

        vsOut.fragPos = vec3(objectModelMatrix * vec4(positionsAttribute, 1.0));

        vec3 lightDir = normalize(lightPos - vsOut.fragPos);
        vsOut.diffPower = max(dot(normalize(vsOut.normal), lightDir), 0.0) * 5.5;

        gl_Position = projectionMatrix * viewMatrix * vec4(vsOut.fragPos, 1.0);
    }
#endif

// shadows impl
#if defined(SHADOWS_CASTERS_NUM) && defined(sgmat_shadowMap_MAX_TEXTURES_NUM)
    float rand(vec2 uv)
    {
        return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453123);
    }

    vec2 poissonDisk[4] = vec2[] (
        vec2( -0.94201624, -0.39906216 ),
        vec2( 0.94558609, -0.76890725 ),
        vec2( -0.094184101, -0.92938870 ),
        vec2( 0.34495938, 0.29387760 )
    );

    float bias = 0.0001;

    float calculateShadow(vec4 shadowsCasterSpaceFragPos, int shadowsCasterIdx)
    {
        vec3 projCoords = shadowsCasterSpaceFragPos.xyz / shadowsCasterSpaceFragPos.w;
        projCoords = projCoords * 0.5 + 0.5;

        if(projCoords.z > 1.0)
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
            int index = int(8.0 * rand(vec2(shadowsCasterSpaceFragPos.xy * i))) % 8;
            float depthFactor = texture(shadowsCastersShadowMaps[shadowsCasterIdx],
            projCoords.xy + poissonDisk[index] / 1500.0).z;
            depthFactor = depthFactor < projCoords.z - bias ?
            depthFactor : 0.0;

            shadowVisibility -= 0.2 * depthFactor;
        }

        return shadowVisibility / 1.5;
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

        float diffPower;
        vec3 fragPos;
    } vsIn;

    float ambient = 0.1;

    void main()
    {
        vec4 colorFromBase = vec4(1);
        vec4 colorFromDiffuse = vec4(1);
        vec4 colorFromMetalness = vec4(1);
        vec4 colorFromRoughness = vec4(1);
        vec3 colorFromNormalMap = vec3(1);

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
        #endif

        fragColor = vec4((vsIn.diffPower + vec3(ambient)) * colorFromBase.rgb, colorFromDiffuse.a);

        #if defined(SHADOWS_CASTERS_NUM) && defined(sgmat_shadowMap_MAX_TEXTURES_NUM)
            float shadowCoeff = 0.0;

            for(int i = 0; i < SHADOWS_CASTERS_NUM && i < sgmat_shadowMap_MAX_TEXTURES_NUM; i += 1)
            {
                fragColor.rgb *= calculateShadow(
                    shadowsCasters[i].shadowsCasterSpace * vec4(vsIn.fragPos, 1.0),
                    i
                );
            }

            //fragColor.rgb *= shadowCoeff / 1.5;
        #endif
    }
#endif
