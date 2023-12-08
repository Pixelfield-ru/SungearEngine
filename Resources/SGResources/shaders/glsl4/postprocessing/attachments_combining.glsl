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
    uniform layersAttachmentNCount;
    // 32 is max count of layers
    uniform sampler2D layersAttachmentN[32];

    out vec4 combinedAttachment;

    in vec2 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        for(int i = 0; i < layersAttachmentNCount; ++i)
        {
            combinedAttachment += texture(layersAttachmentN[i], finalUV);
        }
    }
#endif