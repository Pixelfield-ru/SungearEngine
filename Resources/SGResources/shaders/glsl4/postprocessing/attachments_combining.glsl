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
#endif