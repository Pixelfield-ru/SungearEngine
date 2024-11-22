#define SG_NOT_INCLUDE_LIHTS

#sg_include "sg_shaders/glsl4/uniform_bufs_decl.glsl"
#sg_include "sg_shaders/glsl4/defines.glsl"

#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;

    out vec3 vs_UVAttribute;

    void main()
    {
        vs_UVAttribute = UVAttribute;

        gl_Position = camera.spaceMatrix * objectModelMatrix * vec4(positionsAttribute, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    uniform int sgmat_diffuseSamplers_COUNT;
    uniform sampler2D sgmat_diffuseSamplers[SGMAT_SAMPLERS_OF_TYPE_MAX];

    in vec3 vs_UVAttribute;

    void main()
    {
        float depth = gl_FragCoord.z;

        vec2 finalUV = vs_UVAttribute.xy;
        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        /*float a = 1.0;
        float mixCoeff = 1.0 / sgmat_diffuseSamplers_COUNT;

        for (int i = 0; i < sgmat_diffuseSamplers_COUNT; i++)
        {
            a += texture(sgmat_diffuseSamplers[i], finalUV).a * mixCoeff;
        }*/

        // todo: make blending for shadows
        /*if (a < 0.2)
        {
            discard;
        }*/

        // vsm

        /*depth = depth * 0.5 + 0.5;

        float dx = dFdx(depth);
        float dy = dFdy(depth);

        float moment1 = depth;
        float moment2 = depth * depth - 0.25 * (dx * dx + dy * dy);

        gl_FragColor = vec4(moment1, moment2, 0.0, 1.0);*/
        gl_FragColor = vec4(depth, 0.0, 0.0, 1.0);
        //gl_FragColor = vec4(0.0);
    }
#endif
