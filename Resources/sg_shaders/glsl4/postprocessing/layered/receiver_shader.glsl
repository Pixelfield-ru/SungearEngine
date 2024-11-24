#sg_pragma once

#sg_include "base.glsl"

#define SG_NOT_INCLUDE_LIGHTS

#sg_include "sg_shaders/glsl4/uniform_bufs_decl.glsl"
#sg_include "sg_shaders/glsl4/color_correction/aces.glsl"
#sg_include "sg_shaders/glsl4/defines.glsl"
#sg_include "sg_shaders/glsl4/primitives.glsl"

// this subpass compares data from the depth attachment of the current layer and data from the depth attachments of other layers.
// if a nearer pixel is found, then the color of the attachment that passes the depth check using this subpass is set to vec4(0, 0, 0, 1) (black)
SGSubPass(SGLPPLayerDepthPass)
{
    SGSubShader(Fragment)
    {
        uniform int SGLPP_CurrentLayerSeqIndex;
        uniform int SGLPP_CurrentLayerIndex;
        uniform int SGLPP_LayersIndices[32];
        uniform int SGLPP_LayersCount;

        // as all layers depths
        uniform sampler2D SGLPP_LayersDepthAttachments[16];
        // as layers color attachments that are needed to be depth tested
        uniform sampler2D SGLPP_LayersColorAttachments0[16];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            // depth test pass -------------------------------------------

            float currentFBDepth = texture(SGLPP_LayersDepthAttachments[SGLPP_CurrentLayerSeqIndex], finalUV).r;
            vec4 currentColor0 = texture(SGLPP_LayersColorAttachments0[SGLPP_CurrentLayerSeqIndex], finalUV);

            vec3 finalCol = currentColor0.rgb;

            // then sampling depth from other frame buffers and if we have closer depth then discard fragment
            for (int i = 0; i < SGLPP_LayersCount; i++)
            {
                if (SGLPP_CurrentLayerIndex == SGLPP_LayersIndices[i]) continue;

                float otherDepth = texture(SGLPP_LayersDepthAttachments[i], finalUV).r;
                vec4 otherColor0 = texture(SGLPP_LayersColorAttachments0[i], finalUV);

                if(otherDepth < currentFBDepth)
                {
                    gl_FragColor = vec4(0, 0, 0, 1);
                    return;
                }
            }

            // gl_FragColor = vec4(finalCol, 1.0);

            // gl_FragColor = vec4(currentFBDepth, currentFBDepth, currentFBDepth, 1.0);
            // gl_FragColor.a = 1.0;
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

            // TODO: MAKE ALPHA-TEST

            for(int i = 0; i < SGLPP_AttachmentsToCopyInCurrentTargetAttachmentCount; ++i)
            {
                vec4 layerCol = texture(SGLPP_AttachmentsToCopyInCurrentTargetAttachment[i], finalUV);
                // layerCol.a = 1.0 - layerCol.a;
                combinedColor.rgb += layerCol.rgb * layerCol.a;
            }

            // combinedColor.rgb /= float(SGLPP_AttachmentsToCopyInCurrentTargetAttachmentCount);

            gl_FragColor = combinedColor;
            // fragColor = combinedColor;
            // gl_FragData[1] = combinedColor;
        }
    }
}