#define SG_NOT_INCLUDE_LIGHTS

#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"
#include "../defines.glsl"
#include "../random.glsl"
#include "../disks.glsl"
#include "../math.glsl"

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
    uniform bool depthTestPass;

    uniform int currentFBIndex;
    uniform int FBCount;
    uniform FrameBuffer allFB[MAX_PP_FB_COUNT];

    in vec2 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
        finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        if(depthTestPass)
        {
            // depth test pass -------------------------------------------

            // first - sampling depth from current frame buffer
            float mixCoeff = 1.0 / allFB[currentFBIndex].depthAttachmentsCount;
            float currentFBDepth = 0.0;

            for (int i = 0; i < allFB[currentFBIndex].depthAttachmentsCount; i++)
            {
                currentFBDepth += texture(allFB[currentFBIndex].depthAttachments[i], finalUV).r * mixCoeff;
            }

            // then sampling depth from other frame buffers and if we have closer depth then discard fragment
            for (int i = 0; i < FBCount; i++)
            {
                if (currentFBIndex == i) continue;

                mixCoeff = 1.0 / allFB[i].depthAttachmentsCount;

                float otherDepth = 0.0;

                for (int k = 0; k < allFB[i].depthAttachmentsCount; k++)
                {
                    otherDepth += texture(allFB[i].depthAttachments[k], finalUV).r * mixCoeff;
                }

                // discard fragment
                if (otherDepth < currentFBDepth)
                {
                    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

                    return;
                }
            }

            // ----------------------------------------------------
            // applying color (if depth test passed) --------------

            // then if depth of current frame buffer is closest we render pixel of this frame buffer

            /*vec4 currentFBColor = vec4(0.0, 0.0, 0.0, 1.0);
            mixCoeff = 1.0 / allFB[currentFBIndex].colorAttachmentsCount;

            //for (int i = 0; i < allFB[currentFBIndex].colorAttachmentsCount; i++)
            {
                currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[0], finalUV).rgb * mixCoeff;
            }

            gl_FragColor = currentFBColor;*/

            // -----------------------------------------------------

            return;
        }

        // else FX apply

        float mixCoeff = 1.0 / (allFB[currentFBIndex].colorAttachmentsCount - 1);

        vec4 currentFBColor = vec4(0.0, 0.0, 0.0, 1.0);

        for (int i = 1; i < allFB[currentFBIndex].colorAttachmentsCount; i++)
        {
            int s = 0;

            /*for(int x = -4; x < 4; ++x)
            {
                for(int y = -4; y < 4; ++y)
                {
                    vec2 fUV = finalUV + vec2(x, y) * 0.003;

                    currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[0], fUV).rgb * mixCoeff;

                    ++s;
                }
            }*/

            const float shadowsMinCoeff = 0.55;
            const int samplesNum = 32;

            float visibility = 1.0;
            const float downstep = (1.0 - shadowsMinCoeff) / samplesNum;

            float rand = random(finalUV.xy);
            // rand = mad(rand, 2.0, -1.0);
            float rotAngle = rand * PI;
            vec2 rotTrig = vec2(cos(rotAngle), sin(rotAngle));

            for(int i = 0; i < samplesNum; i++)
            {
                currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[0], finalUV.xy + rotate(poissonDisk[i], rotTrig) / 100.0).rgb;
            }

            currentFBColor.rgb /= samplesNum;
        }

        gl_FragColor = currentFBColor * 100.0;
    }
#endif