#define FRAGMENT_SHADER

#define PI 3.14

#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"

#ifdef VERTEX_SHADER
    uniform vec3 offset;
    uniform vec3 verticesPositions[2];

    void main()
    {
        gl_Position = projectionMatrix * viewMatrix * objectModelMatrix * vec4(offset + verticesPositions[gl_VertexID], 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    out vec4 fragColor;

    uniform vec4 color;

    void main()
    {
        fragColor = vec4(ACESFilm(color.rgb), color.a);
    }
#endif
