#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"

#subpass [VolumetricsPass]

#attribute [Culling] [true]

#vertex

#include "sg_shaders/impl/glsl4/instancing.glsl"

layout (location = SG_VS_POSITIONS_ATTRIBUTE_LOC) in vec3 positionsAttribute;
layout (location = SG_VS_UV_ATTRIBUTE_LOC) in vec3 UVAttribute;
layout (location = SG_VS_NORMALS_ATTRIBUTE_LOC) in vec3 normalsAttribute;
layout (location = SG_VS_TANGENTS_ATTRIBUTE_LOC) in vec3 tangentsAttribute;
layout (location = SG_VS_BITANGENTS_ATTRIBUTE_LOC) in vec3 bitangentsAttribute;
#if SG_VS_VERTEX_COLOR_ATTRIBUTE0_LOC < SG_VS_MAX_ATTRIBS_COUNT
layout (location = SG_VS_VERTEX_COLOR_ATTRIBUTE0_LOC) in vec4 vertexColor0Attribute;
#endif
#if SG_VS_VERTEX_COLOR_ATTRIBUTE1_LOC < SG_VS_MAX_ATTRIBS_COUNT
layout (location = SG_VS_VERTEX_COLOR_ATTRIBUTE1_LOC) in vec4 vertexColor1Attribute;
#endif

out VSOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;
    mat4 invModelMatrix;
} vsOut;

#include "sg_shaders/impl/glsl4/vs_attribs_utils.glsl"

void main()
{
    mat4 instanceModelMatrix = getCurrentInstanceModelMatrix();

    vsOut.UV = UVAttribute.xy;
    vsOut.normal = normalize(normalsAttribute);
    vsOut.worldNormal = normalize(mat3(transpose(inverse(instanceModelMatrix))) * vsOut.normal);

    vsOut.vertexPos = positionsAttribute.xyz;
    vsOut.fragPos = vec3(instanceModelMatrix * vec4(positionsAttribute, 1.0));

    vec3 T = normalize(vec3(instanceModelMatrix * vec4(tangentsAttribute, 0.0)));
    vec3 B = normalize(vec3(instanceModelMatrix * vec4(bitangentsAttribute, 0.0)));
    vec3 N = normalize(vec3(instanceModelMatrix * vec4(vsOut.normal, 0.0)));
    vsOut.TBN = mat3(T, B, N);

    vsOut.invModelMatrix = inverse(instanceModelMatrix);

    gl_Position = camera.projectionSpaceMatrix * vec4(vsOut.fragPos, 1.0);
}

#end

#fragment

// REQUIRED COLORS!!! ===========
layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;
// COLOR FOR STOCHASTIC TRANSPARNCY
layout(location = 3) out vec4 layerSTColor;
// REQUIRED COLORS!!! ===========

#include "sg_shaders/impl/glsl4/math.glsl"
#include "sg_shaders/impl/glsl4/pbr_base.glsl"
#include "sg_shaders/impl/glsl4/color_correction/aces.glsl"
#include "sg_shaders/impl/glsl4/shadows_sampling/csm.glsl"
#include "pbr/lighting_calc.glsl"

in VSOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;
    mat4 invModelMatrix;
} vsIn;

// REQUIRED UNIFORM!!
uniform vec3 u_pickingColor;

uniform int SGPP_CurrentLayerIndex;

uniform sampler2D mat_diffuseSamplers[1];
uniform int mat_diffuseSamplers_CURRENT_COUNT;

uniform sampler2D mat_metalnessSamplers[1];
uniform int mat_metalnessSamplers_CURRENT_COUNT;

uniform sampler2D mat_specularSamplers[1];
uniform int mat_specularSamplers_CURRENT_COUNT;

uniform sampler2D mat_normalsSamplers[1];
uniform int mat_normalsSamplers_CURRENT_COUNT;

uniform sampler2D mat_lightmapSamplers[1];
uniform int mat_lightmapSamplers_CURRENT_COUNT;

uniform sampler2D mat_diffuseRoughnessSamplers[1];
uniform int mat_diffuseRoughnessSamplers_CURRENT_COUNT;

uniform sampler2D mat_heightSamplers[1];
uniform int mat_heightSamplers_CURRENT_COUNT;

uniform float u_sgVolumetricCoverage;
uniform float u_sgVolumetricDensity;
uniform vec3 u_sgMeshAABBMin;
uniform vec3 u_sgMeshAABBMax;

void main()
{
    layerColor = vec4(1.0);
}

#end