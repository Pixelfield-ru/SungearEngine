#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;

    layout(std140, location = 0) uniform ObjectMatrices
    {
        mat4 objectModelMatrix;
    };

    layout(std140, location = 1) uniform CameraMatrices
    {
        mat4 cameraProjectionMatrix;
        mat4 cameraViewMatrix;
    };

    void main()
    {
        gl_Position = cameraProjectionMatrix * cameraViewMatrix * objectModelMatrix * vec4(positionsAttribute.xy, positionsAttribute.z, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    //out vec4 fragColor;

    void main()
    {
        //fragColor = vec4(1.0);
    }
#endif