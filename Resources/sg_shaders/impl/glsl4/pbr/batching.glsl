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
    vec3 instancePosition;
    vec3 instanceRotation;
    vec3 instanceScale;

    BatchInstanceMaterial material;

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

    vec3 instancePosition = texelFetch(u_transformsTextureBuffer, vsOut.instanceIndex * transformJump + 4).xyz;

    vec3 instanceRotation = texelFetch(u_transformsTextureBuffer, vsOut.instanceIndex * transformJump + 5).xyz;

    vec3 instanceScale = texelFetch(u_transformsTextureBuffer, vsOut.instanceIndex * transformJump + 6).xyz;

    vsOut.instanceModelMatrix = instanceModelMatrix;
    vsOut.instancePosition = instancePosition;
    vsOut.instanceRotation = instanceRotation;
    vsOut.instanceScale = instanceScale;

    vec4 matDiffuseCol = texelFetch(u_materialsTextureBuffer, vsOut.instanceIndex * materialJump);
    vec4 matSpecularCol = texelFetch(u_materialsTextureBuffer, vsOut.instanceIndex * materialJump + 1);
    vec4 matAmbientCol = texelFetch(u_materialsTextureBuffer, vsOut.instanceIndex * materialJump + 2);
    vec4 matEmissionCol = texelFetch(u_materialsTextureBuffer, vsOut.instanceIndex * materialJump + 3);
    vec4 matTransparentCol = texelFetch(u_materialsTextureBuffer, vsOut.instanceIndex * materialJump + 4);
    vec3 matShininessMetallicRoughness = texelFetch(u_materialsTextureBuffer, vsOut.instanceIndex * materialJump + 5).rgb;

    vsOut.material.diffuseColor = matDiffuseCol;
    vsOut.material.specularColor = matSpecularCol;
    vsOut.material.ambientColor = matAmbientCol;
    vsOut.material.emissionColor = matEmissionCol;
    vsOut.material.transparentColor = matTransparentCol;
    vsOut.material.shininessMetallicRoughness = matShininessMetallicRoughness;

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

in VSOut
{
    int instanceIndex;
    int triangleIndex;

    mat4 instanceModelMatrix;
    vec3 instancePosition;
    vec3 instanceRotation;
    vec3 instanceScale;

    BatchInstanceMaterial material;

    flat umat4 uvOffsets0;
    flat umat4 uvOffsets1;
    flat umat4 uvOffsets2;
} vsIn[];

// vertices of instances in batch
uniform samplerBuffer u_verticesTextureBuffer;
// indices of vertices of instances in batch
uniform isamplerBuffer u_indicesTextureBuffer;

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
        vec3 vertexNormal = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 2).xyz;
        vec3 vertexTangent = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 3).xyz;
        vec3 vertexBitangent = texelFetch(u_verticesTextureBuffer, vertexIndex * vertexJump + 4).xyz;

        gsOut.UV = vertexUV.xy;
        gsOut.normal = normalize(vertexNormal);
        gsOut.worldNormal = normalize(mat3(transpose(inverse(vsIn[0].instanceModelMatrix))) * vertexNormal);
        gsOut.vertexPos = vertexPos;
        gsOut.fragPos = vec3(vsIn[0].instanceModelMatrix * vec4(vertexPos, 1.0));
        gsOut.verticesIndices = verticesIndices;

        // 0.0 IN w COMPONENT IS CORRECT!!
        vec3 T = normalize(vec3(vsIn[0].instanceModelMatrix * vec4(vertexTangent, 0.0)));
        vec3 B = normalize(vec3(vsIn[0].instanceModelMatrix * vec4(vertexBitangent, 0.0)));
        vec3 N = normalize(vec3(vsIn[0].instanceModelMatrix * vec4(gsOut.normal, 0.0)));
        gsOut.TBN = mat3(T, B, N);

        gsOut.material = vsIn[0].material;

        gsOut.uvOffsets0 = vsIn[0].uvOffsets0;
        gsOut.uvOffsets1 = vsIn[0].uvOffsets1;
        gsOut.uvOffsets2 = vsIn[0].uvOffsets2;

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
    vec3 verticesIndices;

    BatchInstanceMaterial material;

    flat umat4 uvOffsets0;
    flat umat4 uvOffsets1;
    flat umat4 uvOffsets2;
} gsIn;

#include "sg_shaders/impl/glsl4/pbr_base.glsl"
#include "sg_shaders/impl/glsl4/bit_utils.glsl"

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

uniform mat4 CSMLightSpaceMatrix[6];
uniform float CSMCascadesPlanesDistances[6];
uniform sampler2D CSMShadowMaps[6];
uniform int CSMCascadesCount;

float getCSMShadow(vec3 lightDir)
{
    // select cascade layer
    vec4 fragPosViewSpace = camera.viewMatrix * vec4(gsIn.fragPos, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for(int i = 0; i < CSMCascadesCount; ++i)
    {
        if (depthValue < CSMCascadesPlanesDistances[i])
        {
            layer = i;
            break;
        }
    }

    if(layer == -1)
    {
        layer = CSMCascadesCount;
    }
    // layer = 0;


    vec4 fragPosLightSpace = CSMLightSpaceMatrix[layer] * vec4(gsIn.fragPos, 1.0);

    // ===========================================

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth > 1.0)
    {
        return 0.0;
    }

    // todo: pass from cpu side
    const float cameraFarPlane = 2500.0f;

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(gsIn.normal);
    // float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.005);
    float biases[4] = float[] ( 0.002f, 0.002f, 0.008f, 0.05f );
    float bias = biases[layer];
    if (layer == CSMCascadesCount)
    {
        bias *= 1 / (cameraFarPlane * 0.5f);
    }
    else
    {
        bias *= 1 / (CSMCascadesPlanesDistances[layer] * 0.5f);
    }

    // ====================================== algo
    // PCF

    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(CSMShadowMaps[layer], 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(
                CSMShadowMaps[layer],
                vec2(projCoords.xy + vec2(x, y) * texelSize)
            ).r;

            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }

    return shadow;
}

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

    {
        vec2 texUVOffset = unpackU32ToU16Vec2(gsIn.uvOffsets0.r2.x);
        vec2 texSize = unpackU32ToU16Vec2(gsIn.uvOffsets0.r2.y);

        diffuseColor.rgba = vec4(0.0, 0.0, 0.0, 0.0);

        highp vec2 dfdx = dFdx(finalUV) / batchAtlasSize;
        highp vec2 dfdy = dFdy(finalUV) / batchAtlasSize;

        diffuseColor += textureGrad(batchAtlas, (texUVOffset + fract(finalUV) * texSize) / batchAtlasSize, dfdx, dfdy);
    }

    if(diffuseColor.a < 0.05) discard;

    {
        {
            vec2 texUVOffset = unpackU32ToU16Vec2(gsIn.uvOffsets1.r1.z);
            vec2 texSize = unpackU32ToU16Vec2(gsIn.uvOffsets1.r1.w);

            if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y)
            {
                aoRoughnessMetallic.r = 0.0;

                highp vec2 dfdx = dFdx(finalUV) / batchAtlasSize;
                highp vec2 dfdy = dFdy(finalUV) / batchAtlasSize;

                aoRoughnessMetallic.r += textureGrad(batchAtlas, (texUVOffset + fract(finalUV) * texSize) / batchAtlasSize, dfdx, dfdy).r;
            }
        }

        {
            vec2 texUVOffset = unpackU32ToU16Vec2(gsIn.uvOffsets0.r1.z);
            vec2 texSize = unpackU32ToU16Vec2(gsIn.uvOffsets0.r1.w);

            if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y)
            {
                aoRoughnessMetallic.g = 0.0;

                highp vec2 dfdx = dFdx(finalUV) / batchAtlasSize;
                highp vec2 dfdy = dFdy(finalUV) / batchAtlasSize;

                aoRoughnessMetallic.g += textureGrad(batchAtlas, (texUVOffset + fract(finalUV) * texSize) / batchAtlasSize, dfdx, dfdy).g;

                aoRoughnessMetallic.g *= 1.0;
            }
        }

        {
            vec2 texUVOffset = unpackU32ToU16Vec2(gsIn.uvOffsets1.r2.x);
            vec2 texSize = unpackU32ToU16Vec2(gsIn.uvOffsets1.r2.y);

            if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y)
            {
                aoRoughnessMetallic.b = 0.0;

                highp vec2 dfdx = dFdx(finalUV) / batchAtlasSize;
                highp vec2 dfdy = dFdy(finalUV) / batchAtlasSize;

                aoRoughnessMetallic.b += textureGrad(batchAtlas, (texUVOffset + fract(finalUV) * texSize) / batchAtlasSize, dfdx, dfdy).b;

                aoRoughnessMetallic.b *= 1.0;
            }
        }
    }

    {
        // if(mat_normalsSamplers_CURRENT_COUNT > 0)
        // {
        vec2 texUVOffset = unpackU32ToU16Vec2(gsIn.uvOffsets0.r3.z);
        vec2 texSize = unpackU32ToU16Vec2(gsIn.uvOffsets0.r3.w);

        if(texSize.x < batchAtlasSize.x && texSize.y < batchAtlasSize.y)
        {
            highp vec2 dfdx = dFdx(finalUV) / batchAtlasSize;
            highp vec2 dfdy = dFdy(finalUV) / batchAtlasSize;

            normalMapColor += textureGrad(batchAtlas, (texUVOffset + fract(finalUV) * texSize) / batchAtlasSize, dfdx, dfdy).rgb;

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

    // для формулы Шлика-Френеля
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metalness);

    vec3 dirLightsShadowCoeff = vec3(0.0);

    vec3 ambient = vec3(0.0);
    vec3 lo = vec3(0.0);

    float shadow = (-getCSMShadow(atmosphere.sunPosition)) * 0.1;

    // calculating sun
    {
        // ambient += atmosphere.sunAmbient;

        vec3 lightDir = normalize(atmosphere.sunPosition);// TRUE
        vec3 halfWayDir = normalize(lightDir + viewDir);// TRUE

        // energy brightness coeff (коэфф. энергетической яркости)
        float NdotL = saturate(dot(finalNormal, lightDir));
        float NdotVD = abs(dot(finalNormal, viewDir)) + 1e-5f;

        // ===================        shadows calc        =====================

        /*dirLightsShadowCoeff += calcDirLightShadow(
            directionalLights[i],
            gsIn.fragPos,
            finalNormal,
            sgmat_shadowMap2DSamplers[i]
        ) * finalRadiance;*/

        // ====================================================================

        // cooktorrance func: DFG /

        // NDF (normal distribution func)
        float D = GGXTR(
            finalNormal,
            halfWayDir,
            roughness * (1.0 - specularCoeff)
        );// TRUE

        float cosTheta = saturate(dot(halfWayDir, viewDir));

        // это по сути зеркальная часть (kS)
        vec3 F = SchlickFresnel(cosTheta, F0);// kS
        // geometry function
        float G = GeometrySmith(NdotVD, NdotL, roughness * (1.0 - specularCoeff));// TRUE

        vec3 diffuse = vec3(1.0) - F;
        diffuse *= (1.0 - metalness);// check diffuse color higher

        vec3 ctNumerator = D * F * G;
        float ctDenominator = 1.0 * NdotVD * NdotL;
        // vec3 specular = (ctNumerator / max(ctDenominator, 0.001)) * u_materialSpecularCol.r;
        vec3 specular = (ctNumerator / max(ctDenominator, 0.001)) * 0.5;

        lo += ((diffuse * albedo.rgb / PI + specular) + shadow) * max(atmosphere.sunColor.rgb, vec3(0, 0, 0)) * NdotL * 1.0;
    }

    ambient = albedo.rgb * ao;
    vec3 finalCol = ambient * vec3(1.0) * materialAmbientFactor + lo + ambient;

    layerColor = vec4(finalCol, 1.0);
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