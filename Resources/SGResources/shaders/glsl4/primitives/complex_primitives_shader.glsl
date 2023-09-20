#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"

#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;

    uniform vec3 offset;

    void main()
    {
        gl_Position = projectionMatrix * viewMatrix * objectModelMatrix * vec4(offset + positionsAttribute, 1.0);
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
