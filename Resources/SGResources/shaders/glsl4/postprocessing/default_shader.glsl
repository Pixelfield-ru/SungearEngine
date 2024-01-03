#sg_include "vertex_base.glsl"

#sg_define SG_NOT_INCLUDE_LIGHTS

#sg_include "../uniform_bufs_decl.glsl"
#sg_include "../color_correction/aces.glsl"
#sg_include "../defines.glsl"
#sg_include "../primitives.glsl"

SGSubPass(PostProcessLayerDepthPass)
{
    SGSubShader(Fragment)
    {
        uniform int currentFBIndex;

        // as layers count
        uniform int allFrameBuffersDepthAttachmentCount;
        // as all layers depths
        uniform sampler2D allFrameBuffersDepthAttachments[32];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            // depth test pass -------------------------------------------

            float currentFBDepth = texture(allFrameBuffersDepthAttachments[currentFBIndex], finalUV).r;

            // then sampling depth from other frame buffers and if we have closer depth then discard fragment
            for (int i = 0; i < allFrameBuffersDepthAttachmentCount; i++)
            {
                if (currentFBIndex == i) continue;

                float otherDepth = texture(allFrameBuffersDepthAttachments[i], finalUV).r;

                // discard fragment
                if (otherDepth < currentFBDepth)
                {
                    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

                    return;
                }
            }
        }
    }
}

// todo: отдельный шейдер
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

SGSubPass(PostProcessAttachmentsCombiningPass)
{
    SGSubShader(Fragment)
    {
        // out vec4 fragColor;

        uniform int layersAttachmentNCount;
        // 32 is max count of layers
        uniform sampler2D layersAttachmentN[32];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            vec4 combinedColor = vec4(0.0, 0.0, 0.0, 1.0);

            for(int i = 0; i < layersAttachmentNCount; ++i)
            {
                combinedColor.rgb += texture(layersAttachmentN[i], finalUV).rgb;
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
        uniform int combinedBufferAttachmentsCount;
        uniform sampler2D combinedBuffer[32];

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

            finalColor.rgb = texture(combinedBuffer[0], finalUV).rgb;

            gl_FragColor = finalColor;

            // -----------------------------------------------------
        }
    }
}
