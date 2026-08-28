#subpass [GeometryPass]

#include "sg_shaders/impl/glsl4/color_correction/aces.glsl"
#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/math.glsl"
#include "sg_shaders/impl/glsl4/defines.glsl"

struct umat4
{
    uvec4 r0;
    uvec4 r1;
    uvec4 r2;
    uvec4 r3;
};

#vertex

// One instance per triangle of the batch, three vertices each: the batch is drawn with
// renderArrayInstanced(vertexArray, state, 3, 0, trianglesCount) and every attribute below carries a
// divisor of 1. This used to be a point per triangle expanded by a geometry stage; the expansion is
// the same, it just happens here now, because spirv-cross cannot translate a geometry stage to HLSL
// and because a geometry stage is the slowest way to do this on every backend anyway.
layout (location = 0) in ivec2 instanceTriangle;

layout (location = 1) in uvec4 uvOffsets0;
layout (location = 2) in uvec4 uvOffsets1;
layout (location = 3) in uvec4 uvOffsets2;
layout (location = 4) in uvec4 uvOffsets3;

layout (location = 5) in uvec4 uvOffsets4;
layout (location = 6) in uvec4 uvOffsets5;
layout (location = 7) in uvec4 uvOffsets6;
layout (location = 8) in uvec4 uvOffsets7;

layout (location = 9) in uvec4 uvOffsets8;
layout (location = 10) in uvec4 uvOffsets9;
layout (location = 11) in uvec4 uvOffsets10;
layout (location = 12) in uvec4 uvOffsets11;

out GSOut
{
    vec2 UV;

    vec4 fragPos;

    flat umat4 uvOffsets0;
} gsOut;

// transforms of instances in batch
uniform mediump samplerBuffer u_transformsTextureBuffer;
uniform mediump samplerBuffer u_materialsTextureBuffer;

// vertices of instances in batch
uniform mediump samplerBuffer u_verticesTextureBuffer;
// indices of vertices of instances in batch
uniform mediump isamplerBuffer u_indicesTextureBuffer;

/*uniform mat4 CSMLightSpaceMatricies[16];
uniform int CSMCascadesCount;*/

uniform mat4 CSMLightSpaceMatrix;

void main()
{
    int instanceIndex = instanceTriangle.x;
    int triangleIndex = instanceTriangle.y;

    // which corner of the triangle this invocation is: the draw issues exactly three vertices
    int corner = gl_VertexID;

    // =================================================================

    mat4 instanceModelMatrix = mat4(1.0);

    // 4 columns of model matrix, 1 position, 1 rotation, 1 scale
    const int transformJump = 4 + 1 + 1 + 1;

    instanceModelMatrix[0] = texelFetch(u_transformsTextureBuffer, instanceIndex * transformJump);
    instanceModelMatrix[1] = texelFetch(u_transformsTextureBuffer, instanceIndex * transformJump + 1);
    instanceModelMatrix[2] = texelFetch(u_transformsTextureBuffer, instanceIndex * transformJump + 2);
    instanceModelMatrix[3] = texelFetch(u_transformsTextureBuffer, instanceIndex * transformJump + 3);

    umat4 uvOffsetsMat0;
    uvOffsetsMat0.r0 = uvOffsets0;
    uvOffsetsMat0.r1 = uvOffsets1;
    uvOffsetsMat0.r2 = uvOffsets2;
    uvOffsetsMat0.r3 = uvOffsets3;

    // =================================================================

    // 1 position, 1 uv, 1 normal, 1 tangent, 1 bitangent
    const int vertexJump = 1 + 1 + 1 + 1 + 1;

    ivec3 verticesIndices = texelFetch(u_indicesTextureBuffer, triangleIndex).xyz;
    int vertexIndex = verticesIndices[corner];

    vec3 vertexPos = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump).xyz;
    vec3 vertexUV = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 1).xyz;

    vec4 fragPos = instanceModelMatrix * vec4(vertexPos, 1.0);

    gsOut.UV = vertexUV.xy;
    gsOut.fragPos = fragPos;
    gsOut.uvOffsets0 = uvOffsetsMat0;

    // gl_Position = camera.projectionSpaceMatrix * vec4(fragPos.xyz, 1.0);
    gl_Position = CSMLightSpaceMatrix * fragPos;
}

#end

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

in GSOut
{
    vec2 UV;

    vec4 fragPos;

    flat umat4 uvOffsets0;
} gsIn;

#include "sg_shaders/impl/glsl4/bit_utils.glsl"

// layout(early_fragment_tests) in;
// layout (location = 0) out float fragmentDepth;

uniform sampler2D batchAtlas;
uniform vec2 batchAtlasSize;

/*void main() { }*/

void main()
{
    vec2 finalUV = gsIn.UV.xy;
    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - finalUV.y;
    #endif

    vec2 texUVOffset = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets0.r2.x));
    vec2 texSize = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets0.r2.y));

    if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y && texUVOffset.x < batchAtlasSize.x && texUVOffset.y < batchAtlasSize.y)
    {
        vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);

		vec2 uv = (texUVOffset + fract(finalUV) * texSize) / batchAtlasSize;

        highp vec2 dfdx = dFdx(uv) / batchAtlasSize;
        highp vec2 dfdy = dFdy(uv) / batchAtlasSize;

        diffuseColor = textureGrad(batchAtlas, uv, dfdx, dfdy);
        // diffuseColor = textureLod(batchAtlas, uv, 0.0);

        if(diffuseColor.a < 0.05)
        {
            discard;
        }
    }
}

#end
