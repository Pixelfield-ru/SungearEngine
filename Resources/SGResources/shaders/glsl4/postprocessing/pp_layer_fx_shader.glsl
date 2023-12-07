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

        // else FX apply ----------------------------

        /*vec4 currentFBColor = vec4(0.0, 0.0, 0.0, 1.0);

        // first is depth test attachment
        currentFBColor.rgb += texture(allFB[currentFBIndex].colorAttachments[0], finalUV).rgb;

        gl_FragColor = currentFBColor;*/
    }
#endif