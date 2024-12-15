#include "sg_shaders/glsl4/uniform_bufs_decl.glsl"

#subpass [BatchedLinesPass]

#vertex

layout (location = 0) in vec3 linesPositionsAttribute;
layout (location = 1) in vec4 linesColorsAttribute;

out vec4 fs_lineCol;

void main()
{
    fs_lineCol = linesColorsAttribute;

    gl_Position = camera.projectionSpaceMatrix * vec4(linesPositionsAttribute, 1.0);
}

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

out vec4 fragColor;

in vec4 fs_lineCol;

void main()
{
    fragColor = fs_lineCol;
}