#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"

#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;

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
    out vec4 fragColor;

    uniform samplerCube sgmat_skybox25;

    in vec3 vs_UVAttribute;

    void main()
    {
        #ifdef sgmat_skybox25_DEFINED
            vec4 skyboxCol = texture(sgmat_skybox25, vs_UVAttribute.xyz);
            fragColor = vec4(ACESFilm(skyboxCol.rgb), skyboxCol.a);
        #else
            fragColor = vec4(1.0);
        #endif
    }
#endif
