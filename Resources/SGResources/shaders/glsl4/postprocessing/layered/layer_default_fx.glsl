#sg_pragma once

#sg_include "vertex_base.glsl"

SGSubPass(PostProcessLayerFXPass)
{
    SGSubShader(Fragment)
    {
        // valid
        uniform int currentSubPass_Idx;
        // valid
        uniform int currentFBIndex;
        // valid
        uniform int FBCount;

        // valid
        uniform int frameBuffer0_colorAttachmentsCount;
        // valid
        uniform int frameBuffer1_colorAttachmentsCount;
        // layer 0 (valid)
        uniform sampler2D frameBuffer0_colorAttachments[3];
        // layer 1 (valid)
        uniform sampler2D frameBuffer1_colorAttachments[3];

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