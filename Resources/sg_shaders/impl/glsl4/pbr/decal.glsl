#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"

#subpass [GeometryPass]

#attribute [Culling] [true]

#vertex

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;
layout (location = 2) in vec3 normalsAttribute;
layout (location = 3) in vec3 tangentsAttribute;
layout (location = 4) in vec3 bitangentsAttribute;

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

void main()
{
    vsOut.UV = UVAttribute.xy;
    vsOut.normal = normalize(normalsAttribute);
    vsOut.worldNormal = normalize(mat3(transpose(inverse(objectTransform.modelMatrix))) * vsOut.normal);

    vsOut.vertexPos = positionsAttribute.xyz;
    vsOut.fragPos = vec3(objectTransform.modelMatrix * vec4(positionsAttribute, 1.0));

    vec3 T = normalize(vec3(objectTransform.modelMatrix * vec4(tangentsAttribute, 0.0)));
    vec3 B = normalize(vec3(objectTransform.modelMatrix * vec4(bitangentsAttribute, 0.0)));
    vec3 N = normalize(vec3(objectTransform.modelMatrix * vec4(vsOut.normal, 0.0)));
    vsOut.TBN = mat3(T, B, N);

    vsOut.invModelMatrix = inverse(objectTransform.modelMatrix);

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

uniform sampler2D u_GBufferWorldPos;

uniform sampler2D mat_diffuseSamplers[3];
uniform int mat_diffuseSamplers_CURRENT_COUNT;

// REQUIRED UNIFORM!!
uniform vec3 u_pickingColor;

uniform int SGPP_CurrentLayerIndex;

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

void main()
{
    // vec2 uv = gl_FragCoord.xy / vec2(2560, 1440);
    vec2 uv = gl_FragCoord.xy / programData.primaryMonitorSize;

    vec3 gBufWorldPos = texture(u_GBufferWorldPos, uv).xyz;

    vec3 localPos = (vsIn.invModelMatrix * vec4(gBufWorldPos, 1.0)).xyz;

    if (any(lessThan(localPos, vec3(-0.5))) || any(greaterThan(localPos, vec3(0.5)))) discard;

    vec2 decalUV = localPos.xz + 0.5;

    // Читаем текстуру декали
    vec4 decalColor = texture(mat_diffuseSamplers[0], decalUV);

    layerColor = decalColor;
    // layerColor = vec4(decalUV, 0.0, 1.0);
    // layerColor = vec4(gBufWorldPos, 1.0);
    // layerColor = vec4(1.0, 1.0, 0.0, 1.0);
    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);
    pickingColor = vec3(u_pickingColor);
}

#end