#sg_pragma once

#sg_include "base.glsl"

SGSubPass(SGLPPFinalFXPass)
{
    SGSubShader(Fragment)
    {
        // !! - BY DEFAULT, ONE TEXTURE BINDING IS ADDED FOR THIS SHADER NAMED SGLPP_CombinedAttachments[0]
        //      AND CONTANING THE ZERO COLOR ATTACHMENT OF THE LayeredFrameReceiver COMBINED FRAMEBUFFER
        uniform sampler2D SGLPP_CombinedAttachments[1];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

            finalColor.rgb = texture(SGLPP_CombinedAttachments[0], finalUV).rgb;

            gl_FragColor = finalColor;
        }
    }
}