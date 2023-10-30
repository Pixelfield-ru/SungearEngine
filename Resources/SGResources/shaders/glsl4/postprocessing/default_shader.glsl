#include "../uniform_bufs_decl.glsl"

#ifdef VERTEX_SHADER
layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;
layout (location = 2) in vec3 normalsAttribute;

out vec3 vs_UVAttribute;

void main()
{
    vs_UVAttribute = UVAttribute;

    gl_Position = projectionMatrix * viewMatrix * objectModelMatrix * vec4(positionsAttribute, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
    uniform sampler2D sgmat_baseColor0;
    uniform sampler2D sgmat_baseColor1;
    uniform sampler2D sgmat_baseColor2;

    in vec3 vs_UVAttribute;

    void main()
    {
        vec4 baseColor0 = vec4(1.0);
        vec4 baseColor1 = vec4(1.0);
        vec4 baseColor2 = vec4(1.0);
        vec4 baseColor3 = vec4(1.0);
        vec4 baseColor4 = vec4(1.0);
        vec4 baseColor5 = vec4(1.0);
        vec4 baseColor6 = vec4(1.0);
        vec4 baseColor7 = vec4(1.0);
        vec4 baseColor8 = vec4(1.0);
        vec4 baseColor9 = vec4(1.0);

        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        #ifdef sgmat_baseColor1_DEFINED
            float a = texture(sgmat_baseColor8, finalUV).a;
        #endif

        gl_FragColor = vec4(depth, 0.0, 0.0, 1.0);
    }
#endif
