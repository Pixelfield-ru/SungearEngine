#ifdef SHADOWS_CASTERS_NUM
    uniform sampler2D shadowMaps[SHADOWS_CASTERS_NUM];
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

    layout(std140, location = 1) uniform CameraMatrices
    {
        mat4 cameraProjectionMatrix;
        mat4 cameraViewMatrix;
    };

    // final yet
    vec3 lightPos = vec3(0, 25, 15);

    void main()
    {
        vsOut.UV = UVAttribute.xy;
        vsOut.normal = normalsAttribute;

        vsOut.fragPos = vec3(objectModelMatrix * vec4(positionsAttribute, 1.0));

        vec3 lightDir = normalize(lightPos - vsOut.fragPos);
        vsOut.diffPower = max(dot(normalize(vsOut.normal), lightDir), 0.0) * 5.5;

        gl_Position = cameraProjectionMatrix * cameraViewMatrix * objectModelMatrix * vec4(positionsAttribute.xy, positionsAttribute.z, 1.0);
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

        //fragColor = vec4(1.0);
        fragColor = vec4((vsIn.diffPower + vec3(ambient)) * colorFromBase.rgb, colorFromDiffuse.a);
    }
#endif
