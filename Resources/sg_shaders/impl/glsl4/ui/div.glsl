#subpass [UIRenderPass]

#vertex

#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;
layout (location = 2) in int sliceIndexAttribute;

// REQUIRED UNIFORM!!
// first vec2 - min, second vec2 - max
uniform vec4 u_meshAABB;

// REQUIRED UNIFORM!!
uniform vec2 u_elementSize;

out VSOut
{
    vec2 UV;

    vec3 vertexPos;
    vec3 fragPos;
} vsOut;

void main()
{
    vec2 meshAABBMin = u_meshAABB.xy;
    vec2 meshAABBMax = u_meshAABB.zw;

    vec2 meshAABBCenterOffset = meshAABBMin + (meshAABBMax - meshAABBMin) / 2.0;

    vec3 transformedVertexPos = positionsAttribute;

    vec2 centralizedMeshAABBMin = meshAABBMin - meshAABBCenterOffset;
    vec2 centralizedMeshAABBMax = meshAABBMax - meshAABBCenterOffset;

    // moving vertex to center of coords by adding -meshAABBCenter
    transformedVertexPos.xy -= meshAABBCenterOffset;

    // moving vertices by forward direction and saving absolute relations between vertices
    // ==================================================================
    if(transformedVertexPos.x < 0.0)
    {
        float offsetX = abs(centralizedMeshAABBMin.x) - abs(transformedVertexPos.x);

        transformedVertexPos.x = -u_elementSize.x / 2.0 + offsetX;
    }
    else if(transformedVertexPos.x > 0.0)
    {
        float offsetX = abs(centralizedMeshAABBMax.x) - abs(transformedVertexPos.x);

        transformedVertexPos.x = u_elementSize.x / 2.0 - offsetX;
    }

    if(transformedVertexPos.y < 0.0)
    {
        float offsetY = abs(centralizedMeshAABBMin.y) - abs(transformedVertexPos.y);

        transformedVertexPos.y = -u_elementSize.y / 2.0 + offsetY;
    }
    else if(transformedVertexPos.y > 0.0)
    {
        float offsetY = abs(centralizedMeshAABBMax.y) - abs(transformedVertexPos.y);

        transformedVertexPos.y = u_elementSize.y / 2.0 - offsetY;
    }
    // ==================================================================

   /* transformedVertexPos.x = u_elementSize.x / 2.0 + positionsAttribute.x;
    transformedVertexPos.y = u_elementSize.y / 2.0 + positionsAttribute.y;*/

    // moving vertex back to actual position
    transformedVertexPos.xy += meshAABBCenterOffset * (u_elementSize);

    vsOut.UV = UVAttribute.xy;
    vsOut.vertexPos = transformedVertexPos;
    // vsOut.fragPos = vec3(objectTransform.modelMatrix * vec4(positionsAttribute.xy * (meshAABBCenterOffset * 10.0), 0.0, 1.0));
    vsOut.fragPos = vec3(objectTransform.modelMatrix * vec4(positionsAttribute.xy, 0.0, 1.0));

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

in VSOut
{
    vec2 UV;

    vec3 vertexPos;
    vec3 fragPos;
} vsIn;

void main()
{
    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);
    // layerColor = u_backgroundColor;
    layerColor = vec4(1.0, 0.0, 1.0, 1.0);
}

#end