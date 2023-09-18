#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"

#ifdef VERTEX_SHADER
    uniform vec3 offset;
    uniform vec3 verticesPositions[360];

    void main()
    {
        gl_Position = projectionMatrix * viewMatrix * objectModelMatrix * vec4(offset * verticesPositions[gl_VertexID], 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    out vec4 fragColor;

    uniform vec4 color;

    in vec3 vs_UVAttribute;

    void main()
    {
        fragColor = vec4(ACESFilm(color.rgb), color.a);
    }
#endif
