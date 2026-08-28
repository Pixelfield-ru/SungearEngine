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

struct BatchInstanceMaterial
{
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 ambientColor;
    vec4 emissionColor;
    vec4 transparentColor;
    vec3 shininessMetallicRoughness;
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

// the block keeps its name so the fragment stage's interface does not have to change
out GSOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;

    vec3 instancePosition;
    vec3 verticesIndices;

    BatchInstanceMaterial material;

    flat umat4 uvOffsets0;
    flat umat4 uvOffsets1;
    flat umat4 uvOffsets2;
} gsOut;

// transforms of instances in batch
uniform mediump samplerBuffer u_transformsTextureBuffer;
uniform mediump samplerBuffer u_materialsTextureBuffer;

// vertices of instances in batch
uniform mediump samplerBuffer u_verticesTextureBuffer;
// indices of vertices of instances in batch
uniform mediump isamplerBuffer u_indicesTextureBuffer;

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
    // 6 vec4 components
    const int materialJump = 6;

    instanceModelMatrix[0] = texelFetch(u_transformsTextureBuffer, instanceIndex * transformJump);
    instanceModelMatrix[1] = texelFetch(u_transformsTextureBuffer, instanceIndex * transformJump + 1);
    instanceModelMatrix[2] = texelFetch(u_transformsTextureBuffer, instanceIndex * transformJump + 2);
    instanceModelMatrix[3] = texelFetch(u_transformsTextureBuffer, instanceIndex * transformJump + 3);

    vec3 instancePosition = texelFetch(u_transformsTextureBuffer, instanceIndex * transformJump + 4).xyz;

    vec4 matDiffuseCol = texelFetch(u_materialsTextureBuffer, instanceIndex * materialJump);
    vec4 matSpecularCol = texelFetch(u_materialsTextureBuffer, instanceIndex * materialJump + 1);
    vec4 matAmbientCol = texelFetch(u_materialsTextureBuffer, instanceIndex * materialJump + 2);
    vec4 matEmissionCol = texelFetch(u_materialsTextureBuffer, instanceIndex * materialJump + 3);
    vec4 matTransparentCol = texelFetch(u_materialsTextureBuffer, instanceIndex * materialJump + 4);
    vec3 matShininessMetallicRoughness = texelFetch(u_materialsTextureBuffer, instanceIndex * materialJump + 5).rgb;

    gsOut.material.diffuseColor = matDiffuseCol;
    gsOut.material.specularColor = matSpecularCol;
    gsOut.material.ambientColor = matAmbientCol;
    gsOut.material.emissionColor = matEmissionCol;
    gsOut.material.transparentColor = matTransparentCol;
    gsOut.material.shininessMetallicRoughness = matShininessMetallicRoughness;

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

    gsOut.uvOffsets0 = uvOffsetsMat0;
    gsOut.uvOffsets1 = uvOffsetsMat1;
    gsOut.uvOffsets2 = uvOffsetsMat2;

    gsOut.instancePosition = instancePosition;

    // =================================================================

    // 1 position, 1 uv, 1 normal, 1 tangent, 1 bitangent
    const int vertexJump = 1 + 1 + 1 + 1 + 1;

    ivec3 verticesIndices = texelFetch(u_indicesTextureBuffer, triangleIndex).xyz;
    int vertexIndex = verticesIndices[corner];

    vec3 vertexPos = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump).xyz;
    vec3 vertexUV = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 1).xyz;
    vec3 vertexNormal = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 2).xyz;
    vec3 vertexTangent = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 3).xyz;
    vec3 vertexBitangent = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 4).xyz;

    gsOut.fragPos = vec3(instanceModelMatrix * vec4(vertexPos, 1.0));

    gsOut.UV = vertexUV.xy;
    gsOut.normal = normalize(vertexNormal);
    gsOut.worldNormal = normalize(mat3(transpose(inverse(instanceModelMatrix))) * vertexNormal);
    gsOut.vertexPos = vertexPos;
    gsOut.verticesIndices = vec3(verticesIndices);

    // 0.0 IN w COMPONENT IS CORRECT!!
    vec3 T = normalize(vec3(instanceModelMatrix * vec4(vertexTangent, 0.0)));
    vec3 B = normalize(vec3(instanceModelMatrix * vec4(vertexBitangent, 0.0)));
    vec3 N = normalize(vec3(instanceModelMatrix * vec4(gsOut.normal, 0.0)));
    gsOut.TBN = mat3(T, B, N);

    gl_Position = camera.projectionSpaceMatrix * vec4(gsOut.fragPos, 1.0);
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
layout(location = 7) out vec4 layerMaterialInfo;
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
    vec3 verticesIndices;

    BatchInstanceMaterial material;

    flat umat4 uvOffsets0;
    flat umat4 uvOffsets1;
    flat umat4 uvOffsets2;
} gsIn;

#include "sg_shaders/impl/glsl4/pbr_base.glsl"
#include "sg_shaders/impl/glsl4/bit_utils.glsl"
#include "sg_shaders/impl/glsl4/disks.glsl"
#include "sg_shaders/impl/glsl4/shadows_sampling/csm.glsl"
#include "lighting_calc.glsl"

/*uniform sampler2D mat_diffuseSamplers[1];
uniform vec2 mat_diffuseSamplersSizes[1];
uniform int mat_diffuseSamplers_CURRENT_COUNT;

uniform sampler2D mat_metalnessSamplers[1];
uniform vec2 mat_metalnessSamplersSizes[1];
uniform int mat_metalnessSamplers_CURRENT_COUNT;

uniform sampler2D mat_specularSamplers[1];
uniform vec2 mat_specularSamplersSizes[1];
uniform int mat_specularSamplers_CURRENT_COUNT;

uniform sampler2D mat_normalsSamplers[1];
uniform vec2 mat_normalsSamplersSizes[1];
uniform int mat_normalsSamplers_CURRENT_COUNT;

uniform sampler2D mat_lightmapSamplers[1];
uniform vec2 mat_lightmapSamplersSizes[1];
uniform int mat_lightmapSamplers_CURRENT_COUNT;

uniform sampler2D mat_diffuseRoughnessSamplers[1];
uniform vec2 mat_diffuseRoughnessSamplersSizes[1];
uniform int mat_diffuseRoughnessSamplers_CURRENT_COUNT;*/

uniform sampler2D batchAtlas;
uniform vec2 batchAtlasSize;

void main()
{
    vec3 normalizedNormal = gsIn.normal;

    vec4 diffuseColor = gsIn.material.diffuseColor;
    vec4 aoRoughnessMetallic = vec4(materialAmbientFactor, gsIn.material.shininessMetallicRoughness.bg, 1.0);
    float specularCoeff = 0.0f;
    vec3 normalMapColor = vec3(0);
    vec3 finalNormal = vec3(0);

    vec2 finalUV = gsIn.UV.xy;
    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - finalUV.y;
    #endif

    // ===============================================================================================
    // ===============================        load textures       ====================================
    // ===============================================================================================

	vec2 fractUV = fract(finalUV);

    {
        vec2 texUVOffset = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets0.r2.x));
        vec2 texSize = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets0.r2.y));

        // A mesh with no diffuse texture in the atlas is an ordinary case, and its color comes from
        // the instance material — which is what diffuseColor was already initialised with above.
        // Zeroing it here and then discarding on alpha threw away every fragment of every untextured
        // batch, so nothing a batch contained was ever drawn unless it happened to be textured.
        if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y)
        {
            vec2 uv = (texUVOffset + fractUV * texSize) / batchAtlasSize;
            vec2 dfdx = dFdx(uv) / batchAtlasSize;
            vec2 dfdy = dFdy(uv) / batchAtlasSize;

            diffuseColor = textureGrad(batchAtlas, uv, dfdx, dfdy);

            // alpha cutout, and only where a texture actually supplied that alpha
            if(diffuseColor.a < 0.05) discard;
        }
    }

    {
        {
            vec2 texUVOffset = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets1.r1.z));
            vec2 texSize = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets1.r1.w));

            if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y)
            {
				vec2 uv = (texUVOffset + fractUV * texSize) / batchAtlasSize;
				vec2 dfdx = dFdx(uv) / batchAtlasSize;
				vec2 dfdy = dFdy(uv) / batchAtlasSize;
			
                aoRoughnessMetallic.r = 0.0;

                aoRoughnessMetallic.r += textureGrad(batchAtlas, uv, dfdx, dfdy).r;
            }
        }

        {
            vec2 texUVOffset = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets0.r1.z));
            vec2 texSize = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets0.r1.w));

            if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y)
            {
				vec2 uv = (texUVOffset + fractUV * texSize) / batchAtlasSize;
				vec2 dfdx = dFdx(uv) / batchAtlasSize;
				vec2 dfdy = dFdy(uv) / batchAtlasSize;
			
                aoRoughnessMetallic.g = 0.0;

                aoRoughnessMetallic.g += textureGrad(batchAtlas, uv, dfdx, dfdy).g;

                aoRoughnessMetallic.g *= 1.0;
            }
        }

        {
            vec2 texUVOffset = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets1.r2.x));
            vec2 texSize = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets1.r2.y));

            if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y)
            {
				vec2 uv = (texUVOffset + fractUV * texSize) / batchAtlasSize;
				vec2 dfdx = dFdx(uv) / batchAtlasSize;
				vec2 dfdy = dFdy(uv) / batchAtlasSize;
			
                aoRoughnessMetallic.b = 0.0;

                aoRoughnessMetallic.b += textureGrad(batchAtlas, uv, dfdx, dfdy).b;

                aoRoughnessMetallic.b *= 1.0;
            }
        }
    }

    {
        vec2 texUVOffset = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets0.r3.z));
        vec2 texSize = vec2(unpackU32ToU16Vec2(gsIn.uvOffsets0.r3.w));

        if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y)
        {
			vec2 uv = (texUVOffset + fractUV * texSize) / batchAtlasSize;
			vec2 dfdx = dFdx(uv) / batchAtlasSize;
			vec2 dfdy = dFdy(uv) / batchAtlasSize;
			
            normalMapColor += textureGrad(batchAtlas, uv, dfdx, dfdy).rgb;

            finalNormal = normalize(gsIn.TBN * (normalMapColor * 2.0 - 1.0));
        }
        else
        {
            finalNormal = gsIn.worldNormal;
        }
    }

    vec3 viewDir = normalize(camera.position - gsIn.fragPos);

    vec3 albedo         = diffuseColor.rgb;
    float ao            = aoRoughnessMetallic.r;
    float roughness     = aoRoughnessMetallic.g;
    float metalness     = aoRoughnessMetallic.b;

    vec3 finalCol = calculateLight(albedo, gsIn.fragPos, viewDir, finalNormal, roughness, specularCoeff, metalness, ao);

    layerColor = vec4(finalCol, 1.0);
    layerWorldPosColor = gsIn.fragPos;
    layerFragmentNormalColor = finalNormal;
    layerVertexNormalColor = gsIn.worldNormal;
    layerMaterialInfo = vec4(roughness, metalness, specularCoeff, ao);
    // layerColor = diffuseColor;
    // layerColor = vec4(diffuseTexUVOffset, 1.0, 1.0);
    // layerColor = vec4(gsIn.worldNormal, 1.0);
    // layerColor = vec4(finalNormal, 1.0);
    // layerColor = vec4(normalMapColor, 1.0);
    // layerColor = vec4(1.0, aoRoughnessMetallic.g, aoRoughnessMetallic.b, 1.0);

    // layerColor = vec4(finalUV, 0.0f, 1.0);

    // debug
    // layerColor = vec4(gsIn.worldNormal, 1.0f);
}

#end