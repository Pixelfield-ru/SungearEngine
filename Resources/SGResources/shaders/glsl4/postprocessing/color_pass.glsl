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

    void main()
    {
        vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);

        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        float mixCoeff = 1.0 / sgpp_defaultFB.colorAttachmentsCount;

        for (int i = 0; i < sgpp_defaultFB.colorAttachmentsCount; i++)
        {
            finalColor += texture(sgpp_defaultFB.colorAttachments[i], finalUV) * mixCoeff;
        }

        mixCoeff = 1.0 / blurPPLayer.colorAttachmentsCount;

        vec4 blurCol = vec4(0.0);

        for (int i = 0; i < blurPPLayer.colorAttachmentsCount; i++)
        {
            int s = 0;

            for(int x = -9; x < 9; x++)
            {
                for(int y = -9; y < 9; y++)
                {
                    blurCol += texture(blurPPLayer.colorAttachments[i], finalUV + vec2(x, y) * 0.001) * mixCoeff;
                    s++;
                }
            }

            blurCol /= s;
        }

        /*for (int i = 0; i < blurPPLayer.colorAttachmentsCount; i++)
        {
            finalColor += texture(blurPPLayer.colorAttachments[i], finalUV) * mixCoeff;
        }*/

        finalColor += blurCol * 100.0;

        finalColor.rgb = ACESFilm(finalColor.rgb);

        gl_FragColor = finalColor;
    }
#endif
