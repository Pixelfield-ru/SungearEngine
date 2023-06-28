#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;

    out VSOut
    {
        vec2 UV;
    } vsOut;

    layout(std140, binding = 0) uniform SomeU
    {
        mat4 cameraProjectionMatrix;
        mat4 cameraViewMatrix;
        mat4 objectModelMatrix;
        vec4 testColor;
    };

    void main()
    {
        vsOut.UV = UVAttribute.xy;

        gl_Position = cameraProjectionMatrix * cameraViewMatrix * objectModelMatrix * vec4(positionsAttribute.xy, positionsAttribute.z, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    out vec4 fragColor;

    layout (binding = 0) uniform sampler2D material_baseColor;
    layout (binding = 1) uniform sampler2D material_specularColor;
    layout (binding = 2) uniform sampler2D material_roughness;
    layout (binding = 3) uniform sampler2D material_normalMap;
    layout (binding = 4) uniform sampler2D material_parallaxMap;
    layout (binding = 5) uniform sampler2D material_occlusion;
    layout (binding = 6) uniform sampler2D material_emissive;

    in VSOut
    {
        vec2 UV;
    } vsIn;

    void main()
    {
        vec4 colorFromBase = vec4(1);
        vec4 colorFromSpecular = vec4(1);
        vec4 colorFromRoughness = vec4(1);

        #ifdef material_baseColor_DEFINED
            #ifdef FLIP_TEXTURES_Y
                colorFromBase = texture(material_baseColor, vec2(vsIn.UV.x, 1.0 - vsIn.UV.y));
            #else
                colorFromBase = texture(material_baseColor, vec2(vsIn.UV.x, vsIn.UV.y));
            #endif
        #endif

        #ifdef material_specularColor_DEFINED
            #ifdef FLIP_TEXTURES_Y
                colorFromSpecular = texture(material_specularColor, vec2(vsIn.UV.x, 1.0 - vsIn.UV.y));
            #else
                colorFromSpecular = texture(material_specularColor, vec2(vsIn.UV.x, vsIn.UV.y));
            #endif
        #endif

        #ifdef material_roughness_DEFINED
            #ifdef FLIP_TEXTURES_Y
                colorFromRoughness = texture(material_roughness, vec2(vsIn.UV.x, 1.0 - vsIn.UV.y));
            #else
                colorFromRoughness = texture(material_roughness, vec2(vsIn.UV.x, vsIn.UV.y));
            #endif
        #endif

        fragColor = (colorFromBase * 0.25 + colorFromSpecular * 0.55 + colorFromRoughness * 0.2) * vec4(1);
    }
#endif
