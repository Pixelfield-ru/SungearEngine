#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/math.glsl"
#include "sg_shaders/impl/glsl4/defines.glsl"

#subpass [OutlinePass]

#vertex

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;
layout (location = 2) in vec3 normalsAttribute;

uniform float u_outlineThickness;
uniform int u_isFirstPass;

void main()
{
    if(u_isFirstPass == 1)
    {
        gl_Position = camera.projectionSpaceMatrix * objectTransform.modelMatrix * vec4(positionsAttribute, 1.0);
    }
    else
    {
        // gl_Position = camera.projectionSpaceMatrix * objectTransform.modelMatrix * vec4(positionsAttribute + normalize(min(normalsAttribute * u_outlineThickness, vec3(0.01, 0.01, 0.01))), 1.0);
        gl_Position = camera.projectionSpaceMatrix * objectTransform.modelMatrix * vec4(positionsAttribute + normalsAttribute * u_outlineThickness, 1.0);
    }
}

#end

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

// NOT USED!
// layout(location = 0) out vec4 layerVolume;
// layout(location = 1) out vec4 layerColor;

uniform vec4 u_outlineColor;

void main()
{
    gl_FragColor = u_outlineColor;
}

#end