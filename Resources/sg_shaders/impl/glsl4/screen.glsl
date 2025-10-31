#subpass [_]

#vertex

#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;

out VSOut
{
    vec2 UV;

    vec3 vertexPos;
} vsOut;

void main()
{
    vsOut.UV = UVAttribute.xy;
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
// bool
uniform bool u_flipOutput;

out vec4 fragColor;

void main()
{
    // gl_FragColor = vec4(vsIn.UV, 1.0, 1.0);
    float uvY = vsIn.UV.y;
    if(u_flipOutput)
    {
        uvY = 1.0 - uvY;
    }

    vec4 col = vec4(texture(u_bufferToDisplay, vec2(vsIn.UV.x, uvY)));
    // if(col.a < 0.05) discard;

    // gl_FragColor = vec4(texture(u_bufferToDisplay, vec2(vsIn.UV.x - 1.0, uvY) / 2.0).xyz, 1.0);
    // gl_FragColor = vec4(col.r, col.r, col.r, 1.0);
    // gl_FragColor = col;
    fragColor = col;
}

#end