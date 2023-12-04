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
    uniform bool depthTestPass;

    uniform int currentFBIndex;
    uniform int FBCount;
    uniform FrameBuffer allFB[MAX_PP_FB_COUNT];

    in vec2 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        if(depthTestPass)
        {
            // depth test pass -------------------------------------------

            // first - sampling depth from current frame buffer
            float mixCoeff = 1.0 / allFB[currentFBIndex].depthAttachmentsCount;
            float currentFBDepth = 0.0;

            for (int i = 0; i < allFB[currentFBIndex].depthAttachmentsCount; i++)
            {
                currentFBDepth += texture(allFB[currentFBIndex].depthAttachments[i], finalUV).r * mixCoeff;
            }

            // then sampling depth from other frame buffers and if we have closer depth then discard fragment
            for (int i = 0; i < FBCount; i++)
            {
                if (currentFBIndex == i) continue;

                mixCoeff = 1.0 / allFB[i].depthAttachmentsCount;

                float otherDepth = 0.0;

                for (int k = 0; k < allFB[i].depthAttachmentsCount; k++)
                {
                    otherDepth += texture(allFB[i].depthAttachments[k], finalUV).r * mixCoeff;
                }

                // discard fragment
                if (otherDepth < currentFBDepth)
                {
                    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

                    return;
                }
            }

            return;
        }

        // else FX apply

        float mixCoeff = 1.0 / (allFB[currentFBIndex].colorAttachmentsCount - 1);

        vec4 currentFBColor = vec4(0.0, 0.0, 0.0, 1.0);

        // first is depth test attachment
        for (int i = 1; i < allFB[currentFBIndex].colorAttachmentsCount; i++)
        {
            currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[0], finalUV).rgb * mixCoeff;
        }

        gl_FragColor = currentFBColor;
    }
#endif