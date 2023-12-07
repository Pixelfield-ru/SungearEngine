#define SG_NOT_INCLUDE_LIGHTS

#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"
#include "../defines.glsl"
#include "../primitives.glsl"

#ifdef VERTEX_SHADER
    out vec2 vs_UVAttribute;

    void main()
    {
        vec2 pos = quad2DVerticesPositions[gl_VertexID].xy;

        vs_UVAttribute = quad2DUVs[gl_VertexID];

        gl_Position = vec4(pos, 0.0, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    #define GBUFFER_ATTACHMENTS_MAX_COUNT 6

    // combined gbuffer
    uniform sampler2D gBufferAttachments[GBUFFER_ATTACHMENTS_MAX_COUNT];

    in vec2 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

        /*for(int i = 0; i < GBUFFER_ATTACHMENTS_MAX_COUNT; ++i)
        {
            int colorAttachmentToRenderIdx = allFB[i].colorAttachmentToRenderIdx;
            finalColor.rgb += texture(allFB[i].colorAttachments[colorAttachmentToRenderIdx], finalUV).rgb;
        }*/

        finalColor.rgb = texture(gBufferAttachments[0], finalUV);

        gl_FragColor = finalColor;
        // gl_FragColor = finalColor;
        // gl_FragColor = vec4(texture(gBuffer[2], finalUV).rgb, 1.0);
        // gl_FragColor = vec4(ACESFilm(finalColor.rgb), finalColor.a);

        // -----------------------------------------------------
    }
#endif