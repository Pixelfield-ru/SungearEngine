/// This file must be included in vertex shader after layout declaration.

#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/defines.glsl"

/**
 * Requires vertex attribute: ivec4 bonesIDsAttribute0
 * Requires vertex attribute: ivec4 bonesIDsAttribute1
 * Requires vertex attribute: vec4 bonesWeightsAttribute0
 * Requires vertex attribute: vec4 bonesWeightsAttribute1
 * Returns is mesh affected by any bone.
 */
bool calculateVertexPosAndNormal(vec3 inVertexPos, vec3 inVertexNormal, out vec4 outVertexPos, out vec3 outVertexNormal)
{
    outVertexPos = vec4(0.0);
    outVertexNormal = vec3(0.0);

    int bonesCount = int(round(texelFetch(u_bonesMatricesUniformBuffer, 0).r));

    if(u_isAnimatedMesh == 0 || bonesCount == 0)
    {
        outVertexPos = vec4(inVertexPos, 1.0);
        outVertexNormal = inVertexNormal;
    }

    bool isVertexAffected = false;

    for(int i = 0; i < SG_MAX_BONES_PER_VERTEX && i < bonesCount; ++i)
    {
        int curBoneID = -1;
        float curBoneWeight = 0.0f;

        if(i < 4)
        {
            curBoneID = bonesIDsAttribute0[i];
            curBoneWeight = bonesWeightsAttribute0[i];
        }
        else
        {
            curBoneID = bonesIDsAttribute1[i - 4];
            curBoneWeight = bonesWeightsAttribute1[i - 4];
        }

        mat4 boneMatrix = mat4(1.0);

        boneMatrix[0] = texelFetch(u_bonesMatricesUniformBuffer, curBoneID * 16 + 1);
        boneMatrix[1] = texelFetch(u_bonesMatricesUniformBuffer, curBoneID * 16 + 2);
        boneMatrix[2] = texelFetch(u_bonesMatricesUniformBuffer, curBoneID * 16 + 3);
        boneMatrix[3] = texelFetch(u_bonesMatricesUniformBuffer, curBoneID * 16 + 4);

        if(curBoneID == -1)
        {
            continue;
        }

        isVertexAffected = true;

        if(curBoneID >= SG_MAX_BONES_PER_MESH)
        {
            outVertexPos = vec4(inVertexPos, 1.0);
            break;
        }

        vec4 localPosition = boneMatrix * vec4(inVertexPos, 1.0);
        outVertexPos += localPosition * curBoneWeight;
        vec3 localNormal = mat3(boneMatrix) * inVertexNormal;
        outVertexNormal += localNormal * curBoneWeight;
    }

    if(!isVertexAffected)
    {
        outVertexPos = vec4(inVertexPos, 1.0);
        outVertexNormal = inVertexNormal;
    }

    return isVertexAffected;
}