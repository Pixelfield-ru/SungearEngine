#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;

    layout(std140) uniform ObjectMatrices
    {
        mat4 objectModelMatrix;
    };

    layout(std140) uniform ViewMatrices
    {
        mat4 projectionMatrix;
        mat4 viewMatrix;
        vec3 viewDirection;
    };

    out vec3 vs_UVAttribute;

    void main()
    {
        vs_UVAttribute = positionsAttribute;

        gl_Position = projectionMatrix * mat4(mat3(viewMatrix)) * objectModelMatrix * vec4(positionsAttribute, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    out vec4 fragColor;

    uniform samplerCube sgmat_skybox25;

    in vec3 vs_UVAttribute;

    void main()
    {
        //fragColor = vec4(1.0);
        #ifdef sgmat_skybox25_DEFINED
            fragColor = texture(sgmat_skybox25, vs_UVAttribute.xyz);
        #else
            fragColor = vec4(1.0);
        #endif
    }
#endif
