#subpass [UIRenderPass]

#vertex

#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "utils.glsl"

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec2 UVAttribute;
layout (location = 2) in int sliceIndexAttribute;

#include "sg_shaders/impl/glsl4/vs_attribs_utils.glsl"

// REQUIRED UNIFORM!!
// first vec2 - min, second vec2 - max
uniform vec4 u_meshAABB;

// REQUIRED UNIFORM!!
uniform vec2 u_elementSize;

// REQUIRED UNIFORM!!
uniform vec2 u_totalBorderSize;

// REQUIRED UNIFORM!!
uniform int u_layer;

out float si;

out VSOut
{
    vec2 UV;

    vec3 vertexPos;
    vec3 fragPos;
} vsOut;

void main()
{
    // si = float(sliceIndexAttribute);

    vec3 finalVertexPos = calc9SliceVertexPos(positionsAttribute, sliceIndexAttribute, u_elementSize, u_totalBorderSize);

    vsOut.UV = UVAttribute.xy;
    vsOut.vertexPos = finalVertexPos;
    // vsOut.fragPos = vec3(objectTransform.modelMatrix * vec4(positionsAttribute.xy * (meshAABBCenterOffset * 10.0), 0.0, 1.0));
    vsOut.fragPos = vec3(getCurrentInstanceModelMatrix() * vec4(finalVertexPos.xy, float(u_layer), 1.0));

    gl_Position = camera.orthographicSpaceMatrix * vec4(vsOut.fragPos, 1.0);
}

#end

#fragment

#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"

layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;

// REQUIRED UNIFORM!!
uniform vec4 u_backgroundColor;

// REQUIRED UNIFORM!!
uniform vec3 u_pickingColor;

// REQUIRED UNIFORM!!
uniform int SGPP_CurrentLayerIndex;

in float si;

in VSOut
{
    vec2 UV;

    vec3 vertexPos;
    vec3 fragPos;
} vsIn;

void main()
{
    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);
    layerColor = u_backgroundColor / 255.0;
    // layerColor = vec4(si / 9.0, 0.0, 0.0, 1.0);
    // layerColor = vec4(1.0);
}

#end