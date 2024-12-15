#include "sg_shaders/impl/glsl4/primitives.glsl"

#vertex

out vec2 vs_UVAttribute;

void main()
{
    vec2 pos = quad2DVerticesPositions[gl_VertexID].xy;

    vs_UVAttribute = quad2DUVs[gl_VertexID];

    gl_Position = vec4(pos, 0.0, 1.0);
}

#end
