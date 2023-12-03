#include "../uniform_bufs_decl.glsl"

#ifdef VERTEX_SHADER
    uniform vec3 u_offset;
    uniform vec3 u_verticesPositions[2];

    uniform mat4 u_primitiveModelMatrix;

    void main()
    {
        gl_Position = camera.spaceMatrix * u_primitiveModelMatrix * vec4(u_offset + u_verticesPositions[gl_VertexID], 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    out vec4 fragColor;

    uniform vec4 u_color;

    void main()
    {
        fragColor = u_color;
    }
#endif
