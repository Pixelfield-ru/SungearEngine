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

    layout(std140, binding = 0) uniform Matrices
    {
        mat4 cameraProjectionMatrix;
        mat4 cameraViewMatrix;
        mat4 objectModelMatrix;
        vec4 testColor;
    };

    // final yet
    vec3 lightPos = vec3(0, 10, 20);

    void main()
    {
        vsOut.UV = UVAttribute.xy;
        vsOut.normal = normalsAttribute;

        vsOut.fragPos = vec3(objectModelMatrix * vec4(positionsAttribute, 1.0));

        vec3 lightDir = normalize(lightPos - vsOut.fragPos);
        vsOut.diffPower = max(dot(normalize(vsOut.normal), lightDir), 0.0);

        gl_Position = cameraProjectionMatrix * cameraViewMatrix * objectModelMatrix * vec4(positionsAttribute.xy, positionsAttribute.z, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    out vec4 fragColor;

    layout (binding = 0) uniform sampler2D material_baseColor0;
    layout (binding = 1) uniform sampler2D material_specularColor;
    layout (binding = 2) uniform sampler2D material_roughness;
    layout (binding = 3) uniform sampler2D material_normalMap0;
    layout (binding = 4) uniform sampler2D material_parallaxMap;
    layout (binding = 5) uniform sampler2D material_occlusion;
    layout (binding = 6) uniform sampler2D material_emissive;

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
        vec4 colorFromSpecular = vec4(1);
        vec4 colorFromRoughness = vec4(1);
        vec4 colorFromNormalMap = vec4(1);

        #ifdef material_baseColor0_DEFINED
            #ifdef FLIP_TEXTURES_Y
                colorFromBase = texture(material_baseColor0, vec2(vsIn.UV.x, 1.0 - vsIn.UV.y));
            #else
                colorFromBase = texture(material_baseColor0, vec2(vsIn.UV.x, vsIn.UV.y));
            #endif
        #endif

        #ifdef material_specularColor0_DEFINED
            #ifdef FLIP_TEXTURES_Y
                colorFromSpecular = texture(material_specularColor, vec2(vsIn.UV.x, 1.0 - vsIn.UV.y));
            #else
                colorFromSpecular = texture(material_specularColor, vec2(vsIn.UV.x, vsIn.UV.y));
            #endif
        #endif

        #ifdef material_roughness0_DEFINED
            #ifdef FLIP_TEXTURES_Y
                colorFromRoughness = texture(material_roughness, vec2(vsIn.UV.x, 1.0 - vsIn.UV.y));
            #else
                colorFromRoughness = texture(material_roughness, vec2(vsIn.UV.x, vsIn.UV.y));
            #endif
        #endif

        #ifdef material_normalMap0_DEFINED
            #ifdef FLIP_TEXTURES_Y
                colorFromNormalMap = texture(material_normalMap0, vec2(vsIn.UV.x, 1.0 - vsIn.UV.y));
            #else
                colorFromNormalMap = texture(material_normalMap0, vec2(vsIn.UV.x, vsIn.UV.y));
            #endif
        #endif

        fragColor = vec4((vsIn.diffPower + vec3(ambient)) * colorFromBase.xyz, 1.0);
    }
#endif
