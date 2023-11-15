#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"
#include "../defines.glsl"

#define MAX_FB_COUNT 5

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
    uniform int FBCount;
    uniform FrameBuffer allFB[MAX_FB_COUNT];

    in vec2 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        float mixCoeff = 0.0;

        for(int i = 0; i < FBCount; ++i)
        {
            mixCoeff = 1.0 / allFB[i].colorAttachmentsCount;

            for (int k = 0; k < allFB[i].colorAttachmentsCount; ++k)
            {
                // finalColor.rgb += vec3(1.0);
                finalColor.rgb += texture(allFB[i].colorAttachments[k], finalUV).rgb * mixCoeff;
            }
        }

        gl_FragColor = finalColor;

        // -----------------------------------------------------
    }
#endif