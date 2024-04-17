#sg_pragma once

#sg_include "layered_pp_base.glsl"

SGSubPass(PostProcessLayerFXPass)
{
    SGSubShader(Fragment)
    {
        uniform int SGLPP_CurrentSubPassIndex;
        uniform int SGLPP_CurrentLayerIndex;
        uniform int SGLPP_LayersCount;

        // ===================================================
        // !! - VALID NAMINGS FOR ATTACHMENTS ARRAYS
        // SGLPP_Layer{LayerIndex}_ColorAttachments[{ArrayIndex}]
        // SGLPP_Layer{LayerIndex}_DepthAttachments[{ArrayIndex}]
        // SGLPP_Layer{LayerIndex}_DepthStencilAttachments[{ArrayIndex}]
        // SGLPP_Layer{LayerIndex}_RenderAttachments[{ArrayIndex}]
        //
        // SGLPP_{LayerName}_ColorAttachments[{ArrayIndex}]
        // SGLPP_{LayerName}_DepthAttachments[{ArrayIndex}]
        // SGLPP_{LayerName}_DepthStencilAttachments[{ArrayIndex}]
        // SGLPP_{LayerName}_RenderAttachments[{ArrayIndex}]
        //
        // !! - TO GET ATTACHMENTS COUNT USE POSTFIX 'Count'
        // EXAMPLE: 'SGLPP_Layer0_ColorAttachments_Count' OR 'SGLPP_{LayerName}_ColorAttachments_Count'
        //
        // !! - VALID NAMES FOR COMBINED FRAME BUFFER ATTACHMENTS
        // !! - YOU`LL BE ABLE TO USE RESULTS FROM LAST FRAME IN COMBINED FRAME BUFFER
        // SGLPP_CombinedFrameBuffer_ColorAttachments[{ArrayIndex}]
        // SGLPP_CombinedFrameBuffer_DepthAttachments[{ArrayIndex}]
        // SGLPP_CombinedFrameBuffer_DepthStencilAttachments[{ArrayIndex}]
        // SGLPP_CombinedFrameBuffer_RenderAttachments[{ArrayIndex}]
        //
        // !! - TO GET ATTACHMENTS COUNT USE POSTFIX 'Count'
        // EXAMPLE: 'SGLPP_CombinedFrameBuffer_ColorAttachments_Count'
        //
        // !! - TO GET INDICES OF ALL LAYERS USE UNIFORM 'SGLPP_LayersIndices'
        // !! - TO GET LAYERS COUNT USE UNIFORM 'SGLPP_LayersCount'
        // !! - TO GET CURRENT LAYER INDEX USE UNIFORM 'SGLPP_CurrentLayerIndex'. !! WARNING - THE INDEXES OF THE LAYERS ARE NOT NECESSARILY SEQUENTIAL
        // ===================================================

        uniform sampler2D SGLPP_test_pp_layer_ColorAttachments[3];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            if(SGLPP_CurrentLayerIndex == 2 && SGLPP_CurrentSubPassIndex == 1)
            {
                vec4 col = texture(SGLPP_test_pp_layer_ColorAttachments[1], finalUV);
                if(col != SG_MAKE_NOT_THIS_LAYER_COLOR(SGLPP_CurrentLayerIndex))
                {
                    // gl_FragColor = col;
                    gl_FragColor = vec4(1, 0, 0, 1);
                }
            }

            // just do nothing
        }
    }
}