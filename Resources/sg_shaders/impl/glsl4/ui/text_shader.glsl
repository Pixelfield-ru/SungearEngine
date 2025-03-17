#subpass [TextRenderPass]

#vertex

#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

layout (location = 0) in mat4 characterModelMatrix;
layout (location = 4) in vec4 characterColor;
layout (location = 5) in vec2 characterUV;
layout (location = 6) in vec3 characterVertexPosition;

out vec2 vs_UVAttribute;
out vec4 vs_characterColor;

void main()
{
    vs_UVAttribute = characterUV;
    vs_characterColor = characterColor;

    vec3 charVPos = characterVertexPosition;
    charVPos.y = 1.0 - charVPos.y;

    gl_Position = camera.orthographicSpaceMatrix * characterModelMatrix * vec4(charVPos, 1.0);
}

#end

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;

in vec2 vs_UVAttribute;
in vec4 vs_characterColor;

uniform sampler2D u_fontSpecializationAtlas;

// REQUIRED UNIFORM!!
uniform int SGPP_CurrentLayerIndex;

void main()
{
    vec4 charCol = vec4(0, 0, 0, 1);

    vec2 finalUV = vs_UVAttribute.xy;

    charCol = texture(u_fontSpecializationAtlas, vec2(finalUV.x, finalUV.y));

    // fragColor = charCol;
    layerColor = vec4(vec3(charCol.r) * vs_characterColor.rgb, charCol.r * vs_characterColor.a);
    // fragColor = vec4(1.0);
}

#end