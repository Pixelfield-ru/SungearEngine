#sg_pragma once

#sg_include "base.glsl"

SGSubPass(SGLPPLayerFXPass)
{
    SGSubShader(Fragment)
    {
        // ===================================================
        // !! - TO GET INDICES OF ALL LAYERS USE UNIFORM ARRAY 'SGLPP_LayersIndices'
        // !! - TO GET LAYERS COUNT USE UNIFORM INT 'SGLPP_LayersCount'
        // !! - TO GET CURRENT LAYER INDEX USE UNIFORM INT 'SGLPP_CurrentLayerIndex'. !! WARNING - THE INDEXES OF THE LAYERS ARE NOT NECESSARILY SEQUENTIAL
        // !! - TO GET CURRENT LAYER SEQUENTIAL INDEX USE UNIFORM INT 'SGLPP_CurrentLayerSeqIndex'
        // !! - TO GET CURRENT LAYER CURRENT SUB PASS INDEX USE UNIFORM INT 'SGLPP_CurrentSubPassIndex'
        // ===================================================

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            // just do nothing
        }
    }
}