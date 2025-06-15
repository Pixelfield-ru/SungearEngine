#subpass [BatchingPass]

#vertex

#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

layout (location = 0) in ivec2 instanceTriangle;

out vec3 vs_UVAttribute;
flat out int vs_InstanceID;

out VSOut
{
    int instanceIndex;
    int triangleIndex;

    mat4 instanceModelMatrix;
    vec3 instancePosition;
    vec3 instanceRotation;
    vec3 instanceScale;
} vsOut;

void main()
{
    vsOut.instanceIndex = instanceTriangle.x;
    vsOut.triangleIndex = instanceTriangle.y;

    // =================================================================

    mat4 instanceModelMatrix = mat4(1.0);

    // 4 columns of model matrix, 1 position, 1 rotation, 1 scale
    const int transformJump = 4 + 1 + 1 + 1;

    instanceModelMatrix[0] = texelFetch(u_transformsTextureBuffer, vsIn[0].instanceIndex * transformJump);
    instanceModelMatrix[1] = texelFetch(u_transformsTextureBuffer, vsIn[0].instanceIndex * transformJump + 1);
    instanceModelMatrix[2] = texelFetch(u_transformsTextureBuffer, vsIn[0].instanceIndex * transformJump + 2);
    instanceModelMatrix[3] = texelFetch(u_transformsTextureBuffer, vsIn[0].instanceIndex * transformJump + 3);

    vec3 instancePosition = texelFetch(u_transformsTextureBuffer, vsIn[0].instanceIndex * transformJump + 4).xyz;

    vec3 instanceRotation = texelFetch(u_transformsTextureBuffer, vsIn[0].instanceIndex * transformJump + 5).xyz;

    vec3 instanceScale = texelFetch(u_transformsTextureBuffer, vsIn[0].instanceIndex * transformJump + 6).xyz;

    gsOut.instancePosition = instancePosition;

    // =================================================================

    gl_Position = vec4(float(instanceTriangle.y), 1.0, 0.0, 1.0);
}

#end

// =========================================================================
// =========================================================================
// =========================================================================

#geometry

layout (points) in;
layout (triangles, max_vertices = 3) out;

out GSOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;

    vec3 instancePosition;
} gsOut;

in VSOut
{
    int instanceIndex;
    int triangleIndex;
    mat4 instanceModelMatrix;
} vsIn[];

// transforms of instances in batch
uniform samplerBuffer u_transformsTextureBuffer;
// vertices of instances in batch
uniform samplerBuffer u_verticesTextureBuffer;
// indices of vertices of instances in batch
uniform samplerBuffer u_indicesTextureBuffer;

void main()
{
    // 1 position, 1 uv, 1 normal, 1 tangent, 1 bitangent
    const int vertexJump = 1 + 1 + 1 + 1 + 1;

    vec3 verticesIndices = texelFetch(u_indicesTextureBuffer, vsIn[0].triangleIndex).xyz;

    for(int i = 0; i < 3; ++i)
    {
        int vertexIndex = int(verticesIndices[i]);

        vec3 vertexPos = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump).xyz;
        vec3 vertexUV = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 1).xyz;
        vec3 vertexNormal = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 2).xyz;
        vec3 vertexTangent = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 3).xyz;
        vec3 vertexBitangent = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 4).xyz;

        gsOut.UV = vertexUV;
        gsOut.normal = vertexNormal;
        gsOut.worldNormal = normalize(mat3(transpose(inverse(instanceModelMatrix))) * vertexNormal);
        gsOut.vertexPos = vertexPos;
        gsOut.fragPos = vec3(instanceModelMatrix * vec4(vertexPos, 1.0));

        vec3 T = normalize(vec3(instanceModelMatrix * vec4(vertexTangent, 0.0)));
        vec3 B = normalize(vec3(instanceModelMatrix * vec4(vertexBitangent, 0.0)));
        vec3 N = normalize(vec3(instanceModelMatrix * vec4(vertexNormal, 0.0)));
        gsOut.TBN = mat3(T, B, N);

        gl_Position = camera.projectionSpaceMatrix * vec4(gsOut.fragPos, 1.0);

        EmitVertex();
    }

    EndPrimitive();
}

#end

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

// REQUIRED COLORS!!! ===========
layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;
// COLOR FOR STOCHASTIC TRANSPARNCY
layout(location = 3) out vec4 layerSTColor;
layout(location = 4) out vec3 layerWorldPosColor;
layout(location = 5) out vec3 layerFragmentNormalColor;
layout(location = 6) out vec3 layerVertexNormalColor;
// REQUIRED COLORS!!! ===========

in GSOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;

    vec3 instancePosition;
} gsIn;

void main()
{
    vec2 finalUV = gsIn.UV.xy;
    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - gsIn.UV.y;
    #endif

    fragColor = vec4(finalUV, 0.0f, 1.0);
}

#end