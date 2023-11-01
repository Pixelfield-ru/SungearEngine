// #include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"

#ifdef VERTEX_SHADER
    const vec2 verticesPositions[] = vec2[]
    (
        vec2(-1.0, -1.0),
        vec2(-1.0, 1.0),
        vec2(1.0, 1.0),
        vec2(1.0, -1.0)
    );

    const vec2 uvs[] = vec2[]
    (
        vec2(0.0, 0.0),
        vec2(0.0, 1.0),
        vec2(1.0, 1.0),
        vec2(1.0, 0.0)
    );

    out vec2 vs_UVAttribute;

    void main()
    {
        vec2 pos = verticesPositions[gl_VertexID].xy;

        vs_UVAttribute = uvs[gl_VertexID];

        gl_Position = vec4(pos, 0.0, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    uniform int sgmat_frameBufferColorAttachment_UNIFORM_COUNT;

    #ifdef sgmat_frameBufferColorAttachment_MAX_TEXTURES_NUM
        uniform sampler2D sgmat_frameBufferColorAttachmentSamplers[sgmat_frameBufferColorAttachment_MAX_TEXTURES_NUM];
    #endif

    in vec2 vs_UVAttribute;

    void main()
    {
        vec4 baseColor = vec4(0.0);

        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        #ifdef sgmat_frameBufferColorAttachment_MAX_TEXTURES_NUM
            float mixCoeff = 1.0 / sgmat_frameBufferColorAttachment_UNIFORM_COUNT;

            for(int i = 0; i < sgmat_frameBufferColorAttachment_UNIFORM_COUNT; i++)
            {
                baseColor += texture(sgmat_frameBufferColorAttachmentSamplers[i], finalUV) * mixCoeff;
            }
        #endif

        baseColor.rgb = ACESFilm(baseColor.rgb);

        gl_FragColor = baseColor;
    }
#endif
