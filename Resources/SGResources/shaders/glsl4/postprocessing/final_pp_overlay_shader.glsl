#define SG_NOT_INCLUDE_LIGHTS

#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"
#include "../defines.glsl"

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
    uniform int FBCount;
    uniform FrameBuffer allFB[MAX_PP_FB_COUNT];

    in vec2 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

        for(int i = 0; i < FBCount; ++i)
        {
            int colorAttachmentToRenderIdx = allFB[i].colorAttachmentToRenderIdx;
            finalColor.rgb += texture(allFB[i].colorAttachments[colorAttachmentToRenderIdx], finalUV).rgb;
        }

        gl_FragColor = finalColor;
        // gl_FragColor = vec4(ACESFilm(finalColor.rgb), finalColor.a);

        // -----------------------------------------------------
    }
#endif