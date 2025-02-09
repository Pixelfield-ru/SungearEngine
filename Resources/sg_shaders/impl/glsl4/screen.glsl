#subpass [_]

#vertex

#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

layout (location = 0) in vec3 positionsAttribute;
// layout (location = 1) in vec3 UVAttribute;

out VSOut
{
    vec2 UV;

    vec3 vertexPos;
} vsOut;

void main()
{
    vsOut.UV = positionsAttribute.xy;
    vsOut.vertexPos = positionsAttribute;

    gl_Position = vec4(vsOut.vertexPos, 1.0);
}

#end

#fragment

in VSOut
{
    vec2 UV;

    vec3 vertexPos;
} vsIn;

uniform sampler2D u_bufferToDisplay;

void main()
{
    // gl_FragColor = vec4(vsIn.UV, 1.0, 1.0);
    gl_FragColor = vec4(texture(u_bufferToDisplay, vec2(vsIn.UV.x - 1.0, 1.0 - vsIn.UV.y) / 2.0).xyz, 1.0);
}

#end