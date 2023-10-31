// #include "../uniform_bufs_decl.glsl"

#ifdef VERTEX_SHADER
    const vec2 verticesPositions[] = vec2[]
    (
        vec2(-1.0, -1.0),
        vec2(-1.0, 1.0),
        vec2(1.0, 1.0),
        vec2(1.0, -1.0)
    );

    out vec2 vs_UVAttribute;

    void main()
    {
        vec2 pos = verticesPositions[gl_VertexID].xy;

        vs_UVAttribute = pos;

        gl_Position = vec4(vec3(pos, 0.0), 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    #ifdef sgmat_frameBufferColorAttachmentSamplers_COUNT
        uniform sampler2D sgmat_frameBufferColorAttachmentSamplers[sgmat_frameBufferColorAttachmentSamplers_COUNT];
    #endif

    in vec2 vs_UVAttribute;

    void main()
    {
        vec4 baseColor = vec4(0.0);

        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        #ifdef sgmat_frameBufferColorAttachmentSamplers_COUNT
            float mixCoeff = 1.0 / sgmat_frameBufferColorAttachmentSamplers_COUNT;
SDFSDF
            for(int i = 0; i < sgmat_frameBufferColorAttachmentSamplers_COUNT; i++)
            {
                baseColor += texture(sgmat_frameBufferColorAttachmentSamplers[i], finalUV) * mixCoeff;
            }
        #endif

        gl_FragColor = baseColor;
    }
#endif
