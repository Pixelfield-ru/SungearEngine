#sg_pragma once

#sg_include "base.glsl"
#sg_include "utils.glsl"

#define SG_NOT_INCLUDE_LIGHTS

#sg_include "sg_shaders/glsl4/uniform_bufs_decl.glsl"
#sg_include "sg_shaders/glsl4/color_correction/aces.glsl"
#sg_include "sg_shaders/glsl4/defines.glsl"
#sg_include "sg_shaders/glsl4/primitives.glsl"

SGSubPass(SGLPPLayerDepthPass)
{
    SGSubShader(Fragment)
    {
        uniform int SGLPP_LayersIndices[32];
        uniform int SGLPP_LayersCount;

        // as all layers depths
        uniform sampler2D SGLPP_LayersDepthAttachments[16];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            // volumes depth test pass -------------------------------------------

            float currentNearestDepth = 1.1;

            // then sampling depth from other frame buffers and if we have closer depth then discard fragment
            for (int i = 0; i < SGLPP_LayersCount; i++)
            {
                int layerIndex = SGLPP_LayersIndices[i];

                float otherDepth = texture(SGLPP_LayersDepthAttachments[i], finalUV).r;

                if(otherDepth < currentNearestDepth)
                {
                    currentNearestDepth = otherDepth;
                    gl_FragColor = vec4(calculatePPLayerVolume(layerIndex), 1);
                }
            }
        }
    }
}

SGSubPass(SGLPPAttachmentsCombiningPass)
{
    SGSubShader(Fragment)
    {
        out vec4 fragColor;

        uniform int SGLPP_AttachmentsToCopyInCurrentTargetAttachmentCount;
        // 32 is max count of layers
        // an array of attachments that will be "copied" to the current target color attachment
        // (i.e., the attachment of the combined attachments of the postprocessing layers)
        uniform sampler2D SGLPP_AttachmentsToCopyInCurrentTargetAttachment[10];
        uniform sampler2D SGLPP_LayersColorAttachments0[10];
        uniform sampler2D SGLPP_LayersDepthAttachments[10];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            vec4 combinedColor = vec4(0.0, 0.0, 0.0, 1.0);

            // TODO: MAKE ALPHA-TEST

            // value of this must be greater than 1.0
            float currentNearestDepth = 1.1;
            float currentAlpha = 0.0;

            for(int i = 0; i < SGLPP_AttachmentsToCopyInCurrentTargetAttachmentCount; ++i)
            {
                vec4 layerDepthTestedCol = texture(SGLPP_AttachmentsToCopyInCurrentTargetAttachment[i], finalUV);
                vec4 layerCol = texture(SGLPP_LayersColorAttachments0[i], finalUV);
                float layerDepth = texture(SGLPP_LayersDepthAttachments[i], finalUV).r;

                if(layerDepth < currentNearestDepth)
                {
                    currentNearestDepth = layerDepth;
                    combinedColor.rgb = layerDepthTestedCol.rgb * layerDepthTestedCol.a + combinedColor.rgb * (1.0 - layerDepthTestedCol.a);
                }
                else if(layerDepth > currentNearestDepth)
                {
                    combinedColor.rgb += layerDepthTestedCol.rgb * (1.0 - currentAlpha);
                }

                // combinedColor.rgb += layerDepthTestedCol.rgb;

                currentAlpha += layerCol.a;
                currentAlpha = min(currentAlpha, 1.0);
            }

            // combinedColor.rgb /= float(SGLPP_AttachmentsToCopyInCurrentTargetAttachmentCount);

            fragColor = combinedColor;
            // fragColor = combinedColor;
            // gl_FragData[1] = combinedColor;
        }
    }
}