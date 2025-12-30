#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/math.glsl"
#include "sg_shaders/impl/glsl4/defines.glsl"
#include "sg_shaders/impl/glsl4/random.glsl"

#subpass [GeometryPass]

#attribute [Culling] [true]

float ambient = 0.1;

#vertex

#include "sg_shaders/impl/glsl4/instancing.glsl"

layout (location = SG_VS_POSITIONS_ATTRIBUTE_LOC) in vec3 positionsAttribute;
layout (location = SG_VS_UV_ATTRIBUTE_LOC) in vec3 UVAttribute;
layout (location = SG_VS_NORMALS_ATTRIBUTE_LOC) in vec3 normalsAttribute;
layout (location = SG_VS_TANGENTS_ATTRIBUTE_LOC) in vec3 tangentsAttribute;
layout (location = SG_VS_BITANGENTS_ATTRIBUTE_LOC) in vec3 bitangentsAttribute;
layout (location = SG_VS_BONES_IDS_ATTRIBUTE0_LOC) in ivec4 bonesIDsAttribute0;
layout (location = SG_VS_BONES_IDS_ATTRIBUTE1_LOC) in ivec4 bonesIDsAttribute1;
layout (location = SG_VS_BONES_WEIGHTS_ATTRIBUTE0_LOC) in vec4 bonesWeightsAttribute0;
layout (location = SG_VS_BONES_WEIGHTS_ATTRIBUTE1_LOC) in vec4 bonesWeightsAttribute1;
#if SG_VS_VERTEX_COLOR_ATTRIBUTE0_LOC < SG_VS_MAX_ATTRIBS_COUNT
layout (location = SG_VS_VERTEX_COLOR_ATTRIBUTE0_LOC) in vec4 vertexColor0Attribute;
#endif
#if SG_VS_VERTEX_COLOR_ATTRIBUTE1_LOC < SG_VS_MAX_ATTRIBS_COUNT
layout (location = SG_VS_VERTEX_COLOR_ATTRIBUTE1_LOC) in vec4 vertexColor1Attribute;
#endif

#include "sg_shaders/impl/glsl4/animation/bones_calculation.glsl"
#include "sg_shaders/impl/glsl4/vs_attribs_utils.glsl"

out VSOut
{
    vec2 UV;

    vec4 vertexColor0;
    vec4 vertexColor1;
} vsOut;

void main()
{
    vec4 totalPosition = vec4(0.0);
    vec3 totalNormal = vec3(0.0);

    mat4 finalModelMatrix = getCurrentInstanceModelMatrix();

    bool isMeshAffectedByBones = calculateVertexPosAndNormal(positionsAttribute, normalsAttribute, totalPosition, totalNormal);

    /*totalPosition.xyz = positionsAttribute;
    totalPosition.w = 1.0;
    totalNormal = normalsAttribute;*/

    if(isMeshAffectedByBones)
    {
        finalModelMatrix = mat4(1.0);
    }

    vsOut.UV = UVAttribute.xy;

    vec3 fragPos = vec3(finalModelMatrix * totalPosition);

    vsOut.vertexColor0 = getVertexColor0();
    vsOut.vertexColor1 = getVertexColor1();

    gl_Position = camera.projectionSpaceMatrix * vec4(fragPos, 1.0);
    // gl_Position = camera.projectionSpaceMatrix * objectTransform.modelMatrix * vec4(positionsAttribute, 1.0);
}

#end

// ============================================================================
// ============================================================================
// ============================================================================

#fragment

uniform sampler2D mat_diffuseSamplers[3];
uniform int mat_diffuseSamplers_CURRENT_COUNT;

// REQUIRED UNIFORM!!
uniform int u_verticesColorsAttributesCount;

// REQUIRED UNIFORM!!
uniform int u_isTransparentPass;

in VSOut
{
    vec2 UV;

    vec4 vertexColor0;
    vec4 vertexColor1;
} vsIn;

void main()
{
    vec4 diffuseColor = vec4(u_materialDiffuseCol);

    vec2 finalUV = vsIn.UV;
    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - vsIn.UV.y;
    #endif

    // ===============================================================================================
    // ===============================        load textures       ====================================
    // ===============================================================================================

    if(mat_diffuseSamplers_CURRENT_COUNT > 0)
    {
        float mixCoeff = 1.0 / float(mat_diffuseSamplers_CURRENT_COUNT);

        diffuseColor.rgba = vec4(0.0, 0.0, 0.0, 0.0);

        for (int i = 0; i < mat_diffuseSamplers_CURRENT_COUNT; ++i)
        {
            diffuseColor += texture(mat_diffuseSamplers[i], finalUV) * mixCoeff;
        }
    }

    if(diffuseColor.a < 0.1)
    {
        discard;
    }

    if(u_verticesColorsAttributesCount > 0 && vsIn.vertexColor0.a < 0.1)
    {
        discard;
    }
}

#end