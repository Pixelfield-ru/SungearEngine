#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

#subpass [TextRenderPass]

#vertex

layout (location = 0) in mat4 characterModelMatrix;
layout (location = 4) in vec4 characterColor;
layout (location = 5) in vec2 characterUV;
layout (location = 6) in vec3 characterVertexPosition;
// x - layer, y - font scale
layout (location = 7) in vec2 characterAdditionalParams;

out vec2 vs_UVAttribute;
out vec4 vs_characterColor;
out float vs_fontScale;

void main()
{
    vs_UVAttribute = characterUV;
    vs_characterColor = characterColor;
    vs_fontScale = characterAdditionalParams.y;

    vec3 charVPos = characterVertexPosition * 1.0;
    // charVPos.y = (charVPos.y) + u_maxCharacterSize.y / 4.0;

    charVPos.z = characterAdditionalParams.x;

    gl_Position = camera.orthographicSpaceMatrix * characterModelMatrix * vec4(charVPos, 1.0);
}

#end

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;

in vec2 vs_UVAttribute;
in vec4 vs_characterColor;

uniform sampler2D u_fontSpecializationAtlas;

// REQUIRED UNIFORM!!
uniform int SGPP_CurrentLayerIndex;

const float threshold = 0.01;

uniform float pxRange = 6.0;
uniform vec4 bgColor = vec4(0.0);
uniform vec4 fgColor = vec4(0.0, 0.0, 0.0, 1.0);

in float vs_fontScale;

float textWeight = 0.0;
float distanceRange = 10.0;

#define DIST_RANGE 8.0

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}
float getDistance()
{
    vec3 msdf = texture(u_fontSpecializationAtlas, vs_UVAttribute).rgb;
    return median(msdf.r, msdf.g, msdf.b);
}

void main()
{
    vec2 finalUV = vs_UVAttribute.xy;

    vec2 dxdy = fwidth(vs_UVAttribute) * textureSize(u_fontSpecializationAtlas, 0);
    float dist = getDistance() + min(textWeight, 0.5 - 1.0 / DIST_RANGE) - 0.5;
    float opacity = clamp(dist * distanceRange / length(dxdy) + 0.5, 0.0, 1.0);

    // if(opacity < 0.2) discard;

    // layerColor = mix(bgColor, fgColor, opacity);
    layerColor = vec4(fgColor.rgb, opacity);
    // layerColor = vec4(msd, 1.0);
    // fragColor = charCol;
    // layerColor = vec4(charCol.rgb * vs_characterColor.rgb, vs_characterColor.a);
    // layerColor = vec4(1.0);
    // fragColor = vec4(1.0);
}

#end