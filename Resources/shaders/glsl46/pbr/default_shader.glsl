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

    layout(std140, binding = 0) uniform ObjectMatrices
    {
        mat4 objectModelMatrix;
    };

    layout(std140, location = 1) uniform ViewMatrices
    {
        mat4 projectionMatrix;
        mat4 viewMatrix;
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

        gl_Position = projectionMatrix * viewMatrix * objectModelMatrix * vec4(positionsAttribute.xy, positionsAttribute.z, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    out vec4 fragColor;

    layout (binding = 0) uniform sampler2D sgmat_emissive0;
    layout (binding = 1) uniform sampler2D sgmat_ambientOcclusion1;
    layout (binding = 2) uniform sampler2D sgmat_ambient2;
    layout (binding = 3) uniform sampler2D sgmat_diffuseRoughness3;
    layout (binding = 4) uniform sampler2D sgmat_diffuse4;
    layout (binding = 5) uniform sampler2D sgmat_displacement5;
    layout (binding = 6) uniform sampler2D sgmat_height6;
    layout (binding = 7) uniform sampler2D sgmat_normals7;
    layout (binding = 8) uniform sampler2D sgmat_baseColor8;
    layout (binding = 9) uniform sampler2D sgmat_clearcoat9;
    layout (binding = 10) uniform sampler2D sgmat_emissionColor10;
    layout (binding = 11) uniform sampler2D sgmat_lightmap11;
    layout (binding = 12) uniform sampler2D sgmat_metalness12;
    layout (binding = 13) uniform sampler2D sgmat_normalCamera13;
    layout (binding = 14) uniform sampler2D sgmat_opacity14;
    layout (binding = 15) uniform sampler2D sgmat_relfection15;
    layout (binding = 16) uniform sampler2D sgmat_sheen16;
    layout (binding = 17) uniform sampler2D sgmat_shininess17;
    layout (binding = 18) uniform sampler2D sgmat_specular18;
    layout (binding = 19) uniform sampler2D sgmat_transmission19;

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

        fragColor = vec4((vsIn.diffPower + vec3(ambient)) * colorFromBase.rgb * colorFromDiffuse.rgb, colorFromDiffuse.a);
    }
#endif
