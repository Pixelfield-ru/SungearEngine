#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

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

#end

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;

#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"

in vec4 fs_lineCol;

uniform int SGPP_CurrentLayerIndex;

void main()
{
    layerColor = vec4(fs_lineCol.rgb, 1.0);
    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);
}

#end