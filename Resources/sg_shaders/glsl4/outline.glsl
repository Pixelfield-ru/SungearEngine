#include "sg_shaders/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/glsl4/math.glsl"
#include "sg_shaders/glsl4/defines.glsl"

#subpass [OutlinePass]

#vertex

layout (location = 0) in vec3 positionsAttribute;

void main()
{
    gl_Position = camera.projectionSpaceMatrix * objectTransform.modelMatrix * vec4(positionsAttribute, 1.0);
}

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

// NOT USED!
// layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;

uniform vec4 u_outlineColor;

void main()
{
    layerColor = u_outlineColor;
}
