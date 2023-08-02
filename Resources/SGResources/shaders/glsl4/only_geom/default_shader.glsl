#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;

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

    void main()
    {
        gl_Position = projectionMatrix * viewMatrix * objectModelMatrix * vec4(positionsAttribute, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    //out vec4 fragColor;

    void main()
    {
        gl_FragColor = vec4(gl_FragCoord.z);
    }
#endif