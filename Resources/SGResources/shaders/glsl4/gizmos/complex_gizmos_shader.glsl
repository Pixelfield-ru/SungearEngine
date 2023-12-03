#include "../uniform_bufs_decl.glsl"

#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;

    uniform vec3 u_offset;

    uniform mat4 u_primitiveModelMatrix;

    void main()
    {
        gl_Position = camera.spaceMatrix * u_primitiveModelMatrix * vec4(u_offset + positionsAttribute, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    out vec4 fragColor;

    uniform vec4 u_color;

    void main()
    {
        fragColor = vec4(u_color.rgb, u_color.a);
    }
#endif
