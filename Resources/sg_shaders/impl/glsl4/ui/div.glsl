#subpass [UIRenderPass]

#vertex

#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;

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

    vec2 meshAABBCenter = (meshAABBMax - meshAABBMin) / 2.0;

    vec3 transformedVertexPos = positionsAttribute;

    vec2 centralizedMeshABBMin = meshAABBMin - meshAABBCenter;
    vec2 centralizedMeshABBMax = meshAABBMax - meshAABBCenter;

    // moving vertex to center of coords by adding -meshAABBCenter
    transformedVertexPos.xy -= meshAABBCenter;

    // moving vertices by forward direction and saving absolute relations between vertices
    // ==================================================================
    if(transformedVertexPos.x < 0.0)
    {
        float offsetX = abs(centralizedMeshABBMin.x) - abs(transformedVertexPos.x);

        transformedVertexPos.x = u_elementSize.x / 2.0 + offsetX;
    }
    else if(transformedVertexPos.x > 0.0)
    {
        float offsetX = abs(centralizedMeshABBMax.x) - abs(transformedVertexPos.x);

        transformedVertexPos.x = u_elementSize.x / 2.0 - offsetX;
    }

    if(transformedVertexPos.y < 0.0)
    {
        float offsetY = abs(centralizedMeshABBMin.y) - abs(transformedVertexPos.x);

        transformedVertexPos.y = u_elementSize.y / 2.0 + offsetY;
    }
    else if(transformedVertexPos.y > 0.0)
    {
        float offsetY = abs(centralizedMeshABBMax.y) - abs(transformedVertexPos.y);

        transformedVertexPos.y = u_elementSize.y / 2.0 - offsetY;
    }
    // ==================================================================

    // moving vertex back to actual position by adding meshAABBCenter + u_elementSize
    transformedVertexPos.xy += meshAABBCenter + u_elementSize;

    vsOut.UV = UVAttribute.xy;
    vsOut.vertexPos = transformedVertexPos;
    vsOut.fragPos = vec3(objectTransform.modelMatrix * vec4(transformedVertexPos, 1.0));

    gl_Position = camera.orthographicSpaceMatrix * vec4(vsOut.fragPos, 1.0);
}

#end

#fragment

#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"

layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;

// REQUIRED UNIFORM!!
uniform vec4 u_elementColor;

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
    layerColor = u_elementColor;
}

#end