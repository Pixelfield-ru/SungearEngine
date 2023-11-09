#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"
#include "../defines.glsl"

#ifdef VERTEX_SHADER
    const vec2 verticesPositions[] = vec2[]
    (
        vec2(-1.0, -1.0),
        vec2(-1.0, 1.0),
        vec2(1.0, 1.0),
        vec2(1.0, -1.0)
    );

    const vec2 uvs[] = vec2[]
    (
        vec2(0.0, 0.0),
        vec2(0.0, 1.0),
        vec2(1.0, 1.0),
        vec2(1.0, 0.0)
    );

    out vec2 vs_UVAttribute;

    void main()
    {
        vec2 pos = verticesPositions[gl_VertexID].xy;

        vs_UVAttribute = uvs[gl_VertexID];

        gl_Position = vec4(pos, 0.0, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    uniform FrameBuffer sgpp_defaultFB;
    uniform FrameBuffer blurPPLayer;

    in vec2 vs_UVAttribute;

    float curDepth = 1.0;

    int curFBIdx = 0;

    void main()
    {
        vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);

        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        {
            float depth = 0.0;
            float mixCoeff = 1.0 / sgpp_defaultFB.depthAttachmentsCount;

            for (int i = 0; i < sgpp_defaultFB.depthAttachmentsCount; i++)
            {
                depth += texture(sgpp_defaultFB.depthAttachments[i], finalUV).r * mixCoeff;
            }

            if(depth < curDepth)
            {
                curDepth = depth;
                curFBIdx = 0;
            }
        }

        {
            float depth = 0.0;
            float mixCoeff = 1.0 / blurPPLayer.depthAttachmentsCount;

            for (int i = 0; i < blurPPLayer.depthAttachmentsCount; i++)
            {
                int s = 0;

                for(int x = -3; x < 3; x++)
                {
                    for (int y = -3; y < 3; y++)
                    {
                        depth += texture(blurPPLayer.depthAttachments[i], finalUV + vec2(x, y) * 0.001).r * mixCoeff;

                        s++;
                    }
                }

                depth /= s;
            }

            if(depth < curDepth)
            {
                curDepth = depth;
                curFBIdx = 1;
            }
        }

        if (curFBIdx == 0)
        {
            float mixCoeff = 1.0 / sgpp_defaultFB.colorAttachmentsCount;

            for (int i = 0; i < sgpp_defaultFB.colorAttachmentsCount; i++)
            {
                finalColor += texture(sgpp_defaultFB.colorAttachments[i], finalUV) * mixCoeff;
            }
        }
        else if (curFBIdx == 1)
        {
            float mixCoeff = 1.0 / blurPPLayer.colorAttachmentsCount;

            for (int i = 0; i < blurPPLayer.colorAttachmentsCount; i++)
            {
                /*int s = 0;

                for(int x = -3; x < 3; x++)
                {
                    for(int y = -3; y < 3; y++)
                    {
                        finalColor += texture(blurPPLayer.colorAttachments[i], finalUV + vec2(x, y) * 0.001) * mixCoeff;

                        s++;
                    }
                }

                finalColor /= s;*/

                finalColor += texture(blurPPLayer.colorAttachments[i], finalUV) * mixCoeff;
            }

            // finalColor *= vec4(0.5, 1.0, 1.0, 1.0);
        }

        finalColor.rgb = ACESFilm(finalColor.rgb);

        gl_FragColor = finalColor;
    }
#endif
