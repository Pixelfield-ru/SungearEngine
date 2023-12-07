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
    uniform bool isDepthTestPass;

    uniform int currentFBIndex;
    uniform int FBCount;
    uniform FrameBuffer allFB[MAX_PP_FB_COUNT];

    // layer 0
    uniform sampler2D frameBuffer0_ColorAttachments[3];
    // layer 1
    uniform sampler2D frameBuffer1_ColorAttachments[3];

    in vec2 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        if(isDepthTestPass)
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

        // else FX apply ----------------------------

        /*vec4 currentFBColor = vec4(0.0, 0.0, 0.0, 1.0);

        // first is depth test attachment
        currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[0], finalUV).rgb;

        gl_FragColor = currentFBColor;*/
    }
#endif