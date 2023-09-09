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
        //vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(positionsAttribute, 1.0);
        //gl_Position = vec4(pos.xy, pos.w, pos.w);
    }
#endif

#ifdef FRAGMENT_SHADER
    vec3 saturate(vec3 a) { return clamp(a, vec3(0), vec3(1)); }

    vec3 ACESFilm(vec3 x)
    {
        float a = 2.51f;
        float b = 0.03f;
        float c = 2.43f;
        float d = 0.59f;
        float e = 0.14f;
        return saturate((x*(a*x+b))/(x*(c*x+d)+e));
    }

    out vec4 fragColor;

    uniform samplerCube sgmat_skybox25;

    in vec3 vs_UVAttribute;

    void main()
    {
        //fragColor = vec4(1.0);
        #ifdef sgmat_skybox25_DEFINED
            vec4 skyboxCol = texture(sgmat_skybox25, vs_UVAttribute.xyz);
            fragColor = vec4(ACESFilm(skyboxCol.rgb), skyboxCol.a);
        #else
            fragColor = vec4(1.0);
        #endif
    }
#endif
