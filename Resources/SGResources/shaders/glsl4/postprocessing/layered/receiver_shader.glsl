#sg_pragma once

#sg_include "base.glsl"

#define SG_NOT_INCLUDE_LIGHTS

#sg_include "../uniform_bufs_decl.glsl"
#sg_include "../color_correction/aces.glsl"
#sg_include "../defines.glsl"
#sg_include "../primitives.glsl"

// this subpass compares data from the depth attachment of the current layer and data from the depth attachments of other layers.
// if a nearer pixel is found, then the color of the attachment that passes the depth check using this subpass is set to vec4(0, 0, 0, 1) (black)
SGSubPass(SGLPPLayerDepthPass)
{
    SGSubShader(Fragment)
    {
        uniform int SGLPP_CurrentLayerSeqIndex;
        uniform int SGLPP_CurrentLayerIndex;
        uniform int SGLPP_LayersCount;

        // as layers count
        uniform int SGLPP_LayersDepthAttachmentsCount;
        // as all layers depths
        uniform sampler2D SGLPP_LayersDepthAttachments[32];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            // depth test pass -------------------------------------------

            float currentFBDepth = texture(SGLPP_LayersDepthAttachments[SGLPP_CurrentLayerSeqIndex], finalUV).r;

            // then sampling depth from other frame buffers and if we have closer depth then discard fragment
            for (int i = 0; i < SGLPP_LayersCount; i++)
            {
                if (SGLPP_CurrentLayerSeqIndex == i) continue;

                float otherDepth = texture(SGLPP_LayersDepthAttachments[i], finalUV).r;

                // discard fragment
                if (otherDepth < currentFBDepth)
                {
                    gl_FragColor = vec4(0, 0, 0, 1);

                    return;
                }

                // else do nothing and save the pixel color
            }
        }
    }
}

SGSubPass(SGLPPAttachmentsCombiningPass)
{
    SGSubShader(Fragment)
    {
        // out vec4 fragColor;

        uniform int SGLPP_AttachmentsToCopyInCurrentTargetAttachmentCount;
        // 32 is max count of layers
        // an array of attachments that will be "copied" to the current target color attachment
        // (i.e., the attachment of the combined attachments of the postprocessing layers)
        uniform sampler2D SGLPP_AttachmentsToCopyInCurrentTargetAttachment[32];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            vec4 combinedColor = vec4(0.0, 0.0, 0.0, 1.0);

            for(int i = 0; i < SGLPP_AttachmentsToCopyInCurrentTargetAttachmentCount; ++i)
            {
                combinedColor.rgb += texture(SGLPP_AttachmentsToCopyInCurrentTargetAttachment[i], finalUV).rgb;
            }

            gl_FragColor = combinedColor;
            // fragColor = combinedColor;
            // gl_FragData[1] = combinedColor;
        }
    }
}