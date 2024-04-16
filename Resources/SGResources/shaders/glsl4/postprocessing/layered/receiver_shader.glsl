#sg_pragma once

#sg_include "vertex_base.glsl"

#define SG_NOT_INCLUDE_LIGHTS

#sg_include "../uniform_bufs_decl.glsl"
#sg_include "../color_correction/aces.glsl"
#sg_include "../defines.glsl"
#sg_include "../primitives.glsl"

// this subpass compares data from the depth attachment of the current layer and data from the depth attachments of other layers.
// if a nearer pixel is found, then the color of the attachment that passes the depth check using this subpass is set to vec4(0, 0, 0, 1) (black)
SGSubPass(PostProcessLayerDepthPass)
{
    SGSubShader(Fragment)
    {
        uniform int currentFBIndex;

        // as layers count
        uniform int layersDepthAttachmentsCount;
        // as all layers depths
        uniform sampler2D layersDepthAttachments[32];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            // depth test pass -------------------------------------------

            float currentFBDepth = texture(layersDepthAttachments[currentFBIndex], finalUV).r;

            // then sampling depth from other frame buffers and if we have closer depth then discard fragment
            for (int i = 0; i < layersDepthAttachmentsCount; i++)
            {
                if (currentFBIndex == i) continue;

                float otherDepth = texture(layersDepthAttachments[i], finalUV).r;

                // discard fragment
                if (otherDepth < currentFBDepth)
                {
                    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

                    return;
                }

                // else do nothing and save the pixel color
            }
        }
    }
}

SGSubPass(PostProcessAttachmentsCombiningPass)
{
    SGSubShader(Fragment)
    {
        // out vec4 fragColor;

        uniform int attachmentsToCopyInCurrentTargetAttachmentCount;
        // 32 is max count of layers
        // an array of attachments that will be "copied" to the current target color attachment
        // (i.e., the attachment of the combined attachments of the postprocessing layers)
        uniform sampler2D attachmentsToCopyInCurrentTargetAttachment[32];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            vec4 combinedColor = vec4(0.0, 0.0, 0.0, 1.0);

            for(int i = 0; i < attachmentsToCopyInCurrentTargetAttachmentCount; ++i)
            {
                combinedColor.rgb += texture(attachmentsToCopyInCurrentTargetAttachment[i], finalUV).rgb;
            }

            gl_FragColor = combinedColor;
            // fragColor = combinedColor;
            // gl_FragData[1] = combinedColor;
        }
    }
}

SGSubPass(PostProcessFinalFXPass)
{
    SGSubShader(Fragment)
    {
        uniform int combinedAttachmentsCount;
        uniform sampler2D combinedAttachments[32];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

            finalColor.rgb = texture(combinedAttachments[0], finalUV).rgb;

            gl_FragColor = finalColor;

            // -----------------------------------------------------
        }
    }
}
