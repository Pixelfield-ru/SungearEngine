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

    // valid
    uniform int frameBuffer0_colorAttachmentsCount;
    // valid
    uniform int frameBuffer1_colorAttachmentsCount;
    // layer 1 (valid)
    uniform sampler2D frameBuffer0_colorAttachments[1];
    uniform sampler2D frameBuffer1_colorAttachments[9];
    uniform sampler2D frameBuffer0_depthAttachments[1];
    uniform sampler2D frameBuffer1_depthAttachments[1];

    in vec2 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        // else FX apply

        // todo: fix
        vec4 currentFBColor = vec4(0.0, 0.0, 0.0, 1.0);

        // reverse depth pass
        if(currentSubPass_Idx == 0)
        {
            // finalUV.y += sin(finalUV.x) * 0.01;

            float currentFBDepth = texture(frameBuffer1_depthAttachments[0], finalUV).r;

            float otherDepth0 = texture(frameBuffer0_depthAttachments[0], finalUV).r;

            if(otherDepth0 > currentFBDepth)
            {
                /*vec2 wsz = windowSize;
                vec2 clickPos = finalUV;

                float waveOffset = currentTime / 2.5;
                float waveRadius = currentTime / 5.0;

                vec2 resClickPos = clickPos / wsz;
                vec2 waveDir = resClickPos - finalUV;
                waveDir = waveDir * vec2(wsz.x / wsz.y, 1.0);
                float len = length(waveDir);

                float sinFactor = sin(len * 100.0 - currentTime * 5.0) * 0.1;
                float discardFactor = clamp(waveRadius - abs(waveOffset - len), 0.0, 1.0);

                vec2 offset = normalize(waveDir) * sinFactor;

                currentFBColor.rgb = texture(frameBuffer0_colorAttachments[0], finalUV + offset).rgb;*/

                /*float zoom = 2.5;
                float halfWidth = (1.0 / zoom) / 2.0;

                vec2 texSize = textureSize(frameBuffer0_colorAttachments[0], 0);
                float attitude = texSize.x / texSize.y;

                vec2 distortionUV = 0.5 - halfWidth + finalUV / zoom;*/

                // vec2 distortionUV = vec2(finalUV.x + sin(finalUV.y * 1500.0) * 0.001, finalUV.y + sin(finalUV.x * 1500.0) * 0.005);
                vec2 normalCol = texture(frameBuffer1_colorAttachments[3], finalUV).rg;
                vec2 barrier = vec2(1.0, 1.0);
                float NdotB = dot(normalCol, barrier);

                vec3 baseCol = texture(frameBuffer1_colorAttachments[1], finalUV).rgb;

                vec2 distortionUV = finalUV * NdotB;

                /*
                float zoom = 1.5;
                float halfWidth = (1.0 / zoom) / 2.0;
                vec4 col = texture(iChannel0, 0.5 - halfWidth + uv / zoom);*/

                // finalUV.y += sin(finalUV.x * currentTime * 5.0) * 0.01;
                currentFBColor.rgb = texture(frameBuffer0_colorAttachments[0], distortionUV).rgb;
            }

            gl_FragColor = currentFBColor;

            return;
        }
    }
#endif
