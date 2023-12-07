#define SG_NOT_INCLUDE_LIGHTS

#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"
#include "../defines.glsl"
#include "../random.glsl"
#include "../disks.glsl"
#include "../math.glsl"
#include "../primitives.glsl"

#ifdef VERTEX_SHADER
    out vec2 vs_UVAttribute;

    void main()
    {
        vec2 pos = quad2DVerticesPositions[gl_VertexID].xy;

        vs_UVAttribute = quad2DUVs[gl_VertexID];

        gl_Position = vec4(pos, 0.0, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    // VALID
    uniform bool isDepthTestPass;
    // VALID
    uniform bool isFirstBlurPass;
    // VALID
    uniform bool isHorizontalPass;
    // VALID
    uniform bool isFinalPass;

    // VALID
    uniform int currentFBIndex;
    // VALID
    uniform int currentSubPass_Idx;
    // VALID
    uniform int FBCount;

    in vec2 vs_UVAttribute;

    float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        // else FX apply

        // todo: fix
        /*vec4 currentFBColor = vec4(0.0, 0.0, 0.0, 1.0);

        float brightnessBarrier = 0.001;

        int attachmentIdx = 0;

        vec2 texOffset = 1.0 / textureSize(allFB[currentFBIndex].colorAttachments[attachmentIdx], 0);

        float multiplier = 0.7;

        if(currentSubPass_Idx == 0) // DIRECTIONAL PASS
        {
            if(!isFirstBlurPass)
            {
                attachmentIdx = 2;
            }

            if(isHorizontalPass)
            {
                for (int i = 1; i < 5; ++i)
                {
                    currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[attachmentIdx], finalUV.xy + vec2(texOffset.x * i, 0.0)).rgb * weight[i];
                    currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[attachmentIdx], finalUV.xy - vec2(texOffset.x * i, 0.0)).rgb * weight[i];
                }
            }
            else
            {
                for (int i = 1; i < 5; ++i)
                {
                    currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[attachmentIdx], finalUV.xy + vec2(0.0, texOffset.y * i)).rgb * weight[i];
                    currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[attachmentIdx], finalUV.xy - vec2(0.0, texOffset.y * i)).rgb * weight[i];
                }
            }

            gl_FragColor = currentFBColor * multiplier;
        }
        else if(currentSubPass_Idx == 1) // TEMPORAL PASS
        {
            if(isHorizontalPass)
            {
                for (int i = 1; i < 5; ++i)
                {
                    currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[1], finalUV.xy + vec2(texOffset.x * i, 0.0)).rgb * weight[i];
                    currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[1], finalUV.xy - vec2(texOffset.x * i, 0.0)).rgb * weight[i];
                }
            }
            else
            {
                for (int i = 1; i < 5; ++i)
                {
                    currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[1], finalUV.xy + vec2(0.0, texOffset.y * i)).rgb * weight[i];
                    currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[1], finalUV.xy - vec2(0.0, texOffset.y * i)).rgb * weight[i];
                }
            }

            if(isFinalPass)
            {
                vec3 baseCol = texture(allFB[currentFBIndex].colorAttachments[0], finalUV.xy).rgb;
                vec3 finalBloomCol = currentFBColor.rgb * multiplier;

                float brightness = dot(finalBloomCol, vec3(0.2126, 0.7152, 0.0722));
                if(brightness > brightnessBarrier)
                {
                    gl_FragColor = vec4(baseCol + finalBloomCol, 1.0);

                    return;
                }

                gl_FragColor = vec4(baseCol, 1.0);
            }
            else
            {
                gl_FragColor = currentFBColor * multiplier;
            }
        }*/
    }
#endif