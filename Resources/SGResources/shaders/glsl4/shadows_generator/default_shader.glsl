#include "../uniform_bufs_decl.glsl"

#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;

    void main()
    {
        gl_Position = projectionMatrix * viewMatrix * objectModelMatrix * vec4(positionsAttribute, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    void main()
    {
        gl_FragColor = vec4(gl_FragCoord.z);
    }
#endif
