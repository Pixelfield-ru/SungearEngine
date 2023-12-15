#define SG_NOT_INCLUDE_LIGHTS

#include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"
#include "../defines.glsl"
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
    uniform int currentFBIndex;

    // as layers count
    uniform int allFBDepthAttachments_COUNT;
    // as all layers depths
    uniform sampler2D allFBDepthAttachments[32];

    in vec2 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        // depth test pass -------------------------------------------

        float currentFBDepth = texture(allFBDepthAttachments[currentFBIndex], finalUV).r;

        // then sampling depth from other frame buffers and if we have closer depth then discard fragment
        for (int i = 0; i < allFBDepthAttachments_COUNT; i++)
        {
            if (currentFBIndex == i) continue;

            float otherDepth = texture(allFBDepthAttachments[i], finalUV).r;

            // discard fragment
            if (otherDepth < currentFBDepth)
            {
                gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

                return;
            }
        }
    }
#endif