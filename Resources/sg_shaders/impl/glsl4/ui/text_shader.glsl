#subpass [TextRenderPass]

#vertex

#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

layout (location = 0) in mat4 characterModelMatrix;
layout (location = 4) in vec4 characterColor;
layout (location = 5) in vec2 characterUV;
layout (location = 6) in vec3 characterVertexPosition;

out vec2 vs_UVAttribute;
out vec4 vs_characterColor;

uniform vec2 u_maxCharacterSize;

void main()
{
    vs_UVAttribute = characterUV;
    vs_characterColor = characterColor;

    vec3 charVPos = characterVertexPosition;
    // charVPos.y = (charVPos.y) + u_maxCharacterSize.y / 4.0;

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
uniform sampler2D u_fontSpecializationAtlasSDF;

// REQUIRED UNIFORM!!
uniform int SGPP_CurrentLayerIndex;

const float threshold = .01;

float f( vec2 x )
{
    float r = length(x);
    float a = atan(x.y,x.x);
    return r - 1.0 + 0.5*sin(3.0*a+2.0*r*r);
}

vec2 grad( vec2 x )
{
    vec2 h = vec2( 0.01, 0.0 );
    return vec2( f(x+h.xy) - f(x-h.xy),
    f(x+h.yx) - f(x-h.yx) )/(2.0*h.x);
}

float color( vec2 x )
{
    float v = f( x );
    vec2  g = grad( x );
    float de = abs(v)/length(g);
    float eps = 0.05;
    return smoothstep( 1.0*eps, 2.0*eps, de );
}

void main()
{
    vec4 charCol = vec4(0, 0, 0, 1);

    vec2 finalUV = vs_UVAttribute.xy;

    float distance = texture(u_fontSpecializationAtlasSDF, vec2(finalUV.x, finalUV.y)).r;
    float alpha = smoothstep(0.5 - 0.01, 0.5 + 0.01, distance);



    charCol = texture(u_fontSpecializationAtlas, vec2(finalUV.x, finalUV.y));

    vec3 c = texture2D(u_fontSpecializationAtlasSDF, vec2(finalUV.x, finalUV.y)).rgb;
    // vec3 res = smoothstep(.5-threshold, .5+threshold, c);

    float coll = color(c.rg);

    // gl_FragColor = vec4((c-0.5)*contrast,1.0);

    // fragColor = charCol;
    layerColor = vec4(charCol.rgb * vs_characterColor.rgb, vs_characterColor.a);
    // layerColor = vec4(1.0);
    // fragColor = vec4(1.0);
}

#end