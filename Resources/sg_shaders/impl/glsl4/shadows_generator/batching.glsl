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

out VSOut
{
    int instanceIndex;
    int triangleIndex;

    mat4 instanceModelMatrix;

    flat umat4 uvOffsets0;
    flat umat4 uvOffsets1;
    flat umat4 uvOffsets2;
} vsOut;

// transforms of instances in batch
uniform samplerBuffer u_transformsTextureBuffer;
uniform samplerBuffer u_materialsTextureBuffer;

void main()
{
    vsOut.instanceIndex = instanceTriangle.x;
    vsOut.triangleIndex = instanceTriangle.y;

    // =================================================================

    mat4 instanceModelMatrix = mat4(1.0);

    // 4 columns of model matrix, 1 position, 1 rotation, 1 scale
    const int transformJump = 4 + 1 + 1 + 1;
    // 6 vec4 components
    const int materialJump = 6;

    instanceModelMatrix[0] = texelFetch(u_transformsTextureBuffer, vsOut.instanceIndex * transformJump);
    instanceModelMatrix[1] = texelFetch(u_transformsTextureBuffer, vsOut.instanceIndex * transformJump + 1);
    instanceModelMatrix[2] = texelFetch(u_transformsTextureBuffer, vsOut.instanceIndex * transformJump + 2);
    instanceModelMatrix[3] = texelFetch(u_transformsTextureBuffer, vsOut.instanceIndex * transformJump + 3);

    vsOut.instanceModelMatrix = instanceModelMatrix;

    umat4 uvOffsetsMat0;
    uvOffsetsMat0.r0 = uvOffsets0;
    uvOffsetsMat0.r1 = uvOffsets1;
    uvOffsetsMat0.r2 = uvOffsets2;
    uvOffsetsMat0.r3 = uvOffsets3;

    umat4 uvOffsetsMat1;
    uvOffsetsMat1.r0 = uvOffsets4;
    uvOffsetsMat1.r1 = uvOffsets5;
    uvOffsetsMat1.r2 = uvOffsets6;
    uvOffsetsMat1.r3 = uvOffsets7;

    umat4 uvOffsetsMat2;
    uvOffsetsMat2.r0 = uvOffsets8;
    uvOffsetsMat2.r1 = uvOffsets9;
    uvOffsetsMat2.r2 = uvOffsets10;
    uvOffsetsMat2.r3 = uvOffsets11;

    vsOut.uvOffsets0 = uvOffsetsMat0;
    vsOut.uvOffsets1 = uvOffsetsMat1;
    vsOut.uvOffsets2 = uvOffsetsMat2;

    // =================================================================

    gl_Position = vec4(float(instanceTriangle.y), 1.0, 0.0, 1.0);
}

#end

// =========================================================================
// =========================================================================
// =========================================================================

#geometry

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

out GSOut
{
    vec2 UV;

    vec4 fragPos;

    flat umat4 uvOffsets0;
    flat umat4 uvOffsets1;
    flat umat4 uvOffsets2;
} gsOut;

in VSOut
{
    int instanceIndex;
    int triangleIndex;

    mat4 instanceModelMatrix;

    flat umat4 uvOffsets0;
    flat umat4 uvOffsets1;
    flat umat4 uvOffsets2;
} vsIn[];

// vertices of instances in batch
uniform samplerBuffer u_verticesTextureBuffer;
// indices of vertices of instances in batch
uniform isamplerBuffer u_indicesTextureBuffer;

/*uniform mat4 CSMLightSpaceMatricies[16];
uniform int CSMCascadesCount;*/

uniform mat4 CSMLightSpaceMatrix;

void main()
{
    // 1 position, 1 uv, 1 normal, 1 tangent, 1 bitangent
    const int vertexJump = 1 + 1 + 1 + 1 + 1;

    ivec3 verticesIndices = texelFetch(u_indicesTextureBuffer, vsIn[0].triangleIndex).xyz;

    for(int i = 0; i < 3; ++i)
    {
        int vertexIndex = verticesIndices[i];

        vec3 vertexPos = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump).xyz;
        vec3 vertexUV = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 1).xyz;

        gsOut.UV = vertexUV.xy;

        vec4 fragPos = vsIn[0].instanceModelMatrix * vec4(vertexPos, 1.0);

        gsOut.fragPos = fragPos;

        gsOut.uvOffsets0 = vsIn[0].uvOffsets0;
        gsOut.uvOffsets1 = vsIn[0].uvOffsets1;
        gsOut.uvOffsets2 = vsIn[0].uvOffsets2;

        // gl_Position = camera.projectionSpaceMatrix * vec4(fragPos, 1.0);
        gl_Position = CSMLightSpaceMatrix * vec4(fragPos.xyz, 1.0);

        EmitVertex();
    }

    EndPrimitive();
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
    flat umat4 uvOffsets1;
    flat umat4 uvOffsets2;
} gsIn;

#include "sg_shaders/impl/glsl4/bit_utils.glsl"

// layout(location = 0) out float fragmentDepth;

uniform sampler2D batchAtlas;
uniform vec2 batchAtlasSize;

void main()
{
    vec2 finalUV = gsIn.UV.xy;
    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - finalUV.y;
    #endif

    // ===============================================================================================
    // ===============================        load textures       ====================================
    // ===============================================================================================

    vec2 texUVOffset = unpackU32ToU16Vec2(gsIn.uvOffsets0.r2.x);
    vec2 texSize = unpackU32ToU16Vec2(gsIn.uvOffsets0.r2.y);

    vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);

    if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y)
    {
		vec2 uv = (texUVOffset + fract(finalUV) * texSize) / batchAtlasSize;
	
		highp vec2 dfdx = dFdx(uv) / batchAtlasSize;
		highp vec2 dfdy = dFdy(uv) / batchAtlasSize;
	
        diffuseColor = textureGrad(batchAtlas, uv, dfdx, dfdy);
    }

    if(diffuseColor.a < 0.05)
    {
        discard;
    }

    // gl_FragDepth = diffuseColor.a;
}

#end