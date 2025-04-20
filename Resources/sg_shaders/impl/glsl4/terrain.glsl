#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/color_correction/aces.glsl"
#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"
#include "sg_shaders/impl/glsl4/defines.glsl"

#subpass [GeometryPass]

#attribute [Culling] [true]

#vertex

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;
layout (location = 2) in vec3 normalsAttribute;

out VSOut
{
    vec3 vertexPos;
    vec3 fragPos;
} vsOut;

void main()
{
    /*vsOut.UV = UVAttribute.xy;
    vsOut.normal = normalize(normalsAttribute);
    vsOut.worldNormal = normalize(mat3(transpose(inverse(objectTransform.modelMatrix))) * normalsAttribute);*/

    vsOut.vertexPos = positionsAttribute;
    vsOut.fragPos = vec3(objectTransform.modelMatrix * vec4(positionsAttribute, 1.0));

    gl_Position = objectTransform.modelMatrix * vec4(positionsAttribute.xyz, 1.0);
}

#end

// =====================================================================================================================

#tess_control

layout(vertices = 3) out;

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    const int MIN_TESS_LEVEL = 5;
    const int MAX_TESS_LEVEL = 70;
    const float MIN_DISTANCE = 0;
    const float MAX_DISTANCE = 800;

    if(gl_InvocationID == 0)
    {
        vec4 eye0 = camera.viewMatrix * objectTransform.modelMatrix * gl_in[0].gl_Position;
        vec4 eye1 = camera.viewMatrix * objectTransform.modelMatrix * gl_in[1].gl_Position;
        vec4 eye2 = camera.viewMatrix * objectTransform.modelMatrix * gl_in[2].gl_Position;

        eye0.z = int(eye0.z / 100.0) * 100.0;
        eye1.z = int(eye1.z / 100.0) * 100.0;
        eye2.z = int(eye2.z / 100.0) * 100.0;

        float dist0 = clamp((abs(eye0.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float dist1 = clamp((abs(eye1.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float dist2 = clamp((abs(eye2.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);

        float tessLevel0 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(dist0, dist1));
        float tessLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(dist1, dist2));
        float tessLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(dist2, dist0));

        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;

        gl_TessLevelInner[0] = max(max(tessLevel0, tessLevel1), tessLevel2);
    }
}

#end

// =====================================================================================================================

#tess_eval

layout(triangles, fractional_odd_spacing, ccw) in;

out TessEvalOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;
} tessEvalOut;

uniform sampler2D mat_heightSamplers[1];
uniform int mat_heightSamplers_CURRENT_COUNT;

float getHeight(vec2 xz)
{
    return texture(mat_heightSamplers[0], xz).r * 128.0 - 16.0;
}

void main()
{
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;

    // Интерполяция с помощью барицентрических координат
    vec3 pos = gl_TessCoord.x * p0 +
               gl_TessCoord.y * p1 +
               gl_TessCoord.z * p2;

    // generating normal
    /*float eps = 0.05;
    float hL = getHeight(pos.xz - vec2(eps, 0.0));
    float hR = getHeight(pos.xz + vec2(eps, 0.0));
    float hD = getHeight(pos.xz - vec2(0.0, eps));
    float hU = getHeight(pos.xz + vec2(0.0, eps));

    vec3 normal;
    normal.x = hL - hR;
    normal.y = 2.0 * eps;
    normal.z = hD - hU;*/

    tessEvalOut.vertexPos = pos;
    tessEvalOut.fragPos = vec3(objectTransform.modelMatrix * vec4(pos, 1.0));

    float eps = 0.01;

    // Позиции сдвинутые по x и z
    vec2 offsetX = vec2(eps, 0.0);
    vec2 offsetZ = vec2(0.0, eps);

    // Высоты вокруг текущей точки
    float hL = getHeight(pos.xz - offsetX);
    float hR = getHeight(pos.xz + offsetX);
    float hD = getHeight(pos.xz - offsetZ);
    float hU = getHeight(pos.xz + offsetZ);

    // Позиции с высотой
    vec3 pL = vec3(pos.x - eps, hL, pos.z);
    vec3 pR = vec3(pos.x + eps, hR, pos.z);
    vec3 pD = vec3(pos.x, hD, pos.z - eps);
    vec3 pU = vec3(pos.x, hU, pos.z + eps);

    // Векторы касаний поверхности
    vec3 dx = pR - pL;
    vec3 dz = pU - pD;

    // Крестовое произведение — нормаль
    vec3 normal = normalize(cross(dz, dx));

    // tessEvalOut.UV = UVAttribute.xy;
    tessEvalOut.normal = normalize(normal);
    tessEvalOut.worldNormal = normalize(mat3(transpose(inverse(objectTransform.modelMatrix))) * tessEvalOut.normal);
    // tessEvalOut.UV = (pos.xz * 0.001 + 20.0) / 2.0;
    tessEvalOut.UV = (pos.xz * 0.001 - 1.5) / 2.0;

    /*vec3 T = normalize(vec3(objectTransform.modelMatrix * vec4(tangentsAttribute, 0.0)));
    vec3 B = normalize(vec3(objectTransform.modelMatrix * vec4(bitangentsAttribute, 0.0)));
    vec3 N = normalize(vec3(objectTransform.modelMatrix * vec4(vsOut.normal, 0.0)));
    vsOut.TBN = mat3(T, B, N);*/

    // pos.y += getHeight(pos.xz);
    pos.y = getHeight(tessEvalOut.UV);

    gl_Position = camera.projectionSpaceMatrix *vec4(pos, 1.0);
}

#end

// =====================================================================================================================

#fragment

layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;

#include "sg_shaders/impl/glsl4/math.glsl"
#include "sg_shaders/impl/glsl4/pbr_base.glsl"

uniform sampler2D mat_diffuseSamplers[3];
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

// REQUIRED UNIFORM!!
uniform vec3 u_pickingColor;

uniform int SGPP_CurrentLayerIndex;

in VSOut
{
    vec3 vertexPos;
    vec3 fragPos;
} vsIn;

in TessEvalOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;
} tessEvalIn;

void main()
{
    vec3 normalizedNormal = tessEvalIn.normal;

    vec4 diffuseColor = vec4(u_materialDiffuseCol);
    vec4 aoRoughnessMetallic = vec4(materialAmbientFactor, u_materialRoughnessFactor, u_materialMetallicFactor, 1);
    float specularCoeff = 0.0f;
    vec3 normalMapColor = vec3(0);
    vec3 finalNormal = vec3(0);

    vec2 finalUV = tessEvalIn.UV;
    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - tessEvalIn.UV.y;
    #endif

    // ===============================================================================================
    // ===============================        loading textures       =================================
    // ===============================================================================================

    {
        if(mat_diffuseSamplers_CURRENT_COUNT > 0)
        {
            float mixCoeff = 1.0 / mat_diffuseSamplers_CURRENT_COUNT;

            diffuseColor.rgba = vec4(0.0, 0.0, 0.0, 0.0);

            for (int i = 0; i < mat_diffuseSamplers_CURRENT_COUNT; ++i)
            {
                diffuseColor += texture(mat_diffuseSamplers[i], finalUV) * mixCoeff;
            }
        }
    }

    if(diffuseColor.a < 0.05) discard;

    {
        if(mat_specularSamplers_CURRENT_COUNT > 0)
        {
            float mixCoeff = 1.0 / mat_specularSamplers_CURRENT_COUNT;

            specularCoeff = 0.0;

            for (int i = 0; i < mat_specularSamplers_CURRENT_COUNT; ++i)
            {
                specularCoeff += texture(mat_specularSamplers[i], finalUV).r * mixCoeff;
            }
        }
    }


    /*if(diffuseColor.a < 0.1)
    {
        discard;
    }*/

    {
        {
            if(mat_lightmapSamplers_CURRENT_COUNT > 0)
            {
                float mixCoeff = 1.0 / mat_lightmapSamplers_CURRENT_COUNT;

                aoRoughnessMetallic.r = 0.0;

                for (int i = 0; i < mat_lightmapSamplers_CURRENT_COUNT; ++i)
                {
                    aoRoughnessMetallic.r += texture(mat_lightmapSamplers[i], finalUV).r * mixCoeff;
                }
            }
        }

        {
            if(mat_diffuseRoughnessSamplers_CURRENT_COUNT > 0)
            {
                float mixCoeff = 1.0 / mat_diffuseRoughnessSamplers_CURRENT_COUNT;

                aoRoughnessMetallic.g = 0.0;

                for (int i = 0; i < mat_diffuseRoughnessSamplers_CURRENT_COUNT; ++i)
                {
                    aoRoughnessMetallic.g += texture(mat_diffuseRoughnessSamplers[i], finalUV).g * mixCoeff;
                }

                aoRoughnessMetallic.g *= 1.0;
            }
        }

        {
            if(mat_metalnessSamplers_CURRENT_COUNT > 0)
            {
                float mixCoeff = 1.0 / mat_metalnessSamplers_CURRENT_COUNT;

                aoRoughnessMetallic.b = 0.0;

                for (int i = 0; i < mat_metalnessSamplers_CURRENT_COUNT; ++i)
                {
                    aoRoughnessMetallic.b += texture(mat_metalnessSamplers[i], finalUV).b * mixCoeff;
                }

                aoRoughnessMetallic.b *= 1.0;
            }
        }
    }

    if(mat_normalsSamplers_CURRENT_COUNT > 0)
    {
        float mixCoeff = 1.0 / mat_normalsSamplers_CURRENT_COUNT;

        for (int i = 0; i < mat_normalsSamplers_CURRENT_COUNT; ++i)
        {
            normalMapColor += texture(mat_normalsSamplers[i], finalUV).rgb * mixCoeff;
        }

        finalNormal = normalize(tessEvalIn.TBN * (normalMapColor * 2.0 - 1.0));
    }
    else
    {
        finalNormal = tessEvalIn.worldNormal;
    }

    vec3 viewDir = normalize(camera.position - tessEvalIn.fragPos);

    // colorFromRoughness.r = AO MAP
    // colorFromRoughness.g = ROUGHNESS
    // colorFromRoughness.b = METALNESS

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
            vsIn.fragPos,
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
        vec3 specular = (ctNumerator / max(ctDenominator, 0.001)) * u_materialSpecularCol.r;

        lo += (diffuse * albedo.rgb / PI + specular) * max(atmosphere.sunColor.rgb, vec3(0, 0, 0)) * NdotL * 1.0;
    }

    // ambient = ambient * albedo.rgb * ao;
    ambient = albedo.rgb * ao;
    vec3 finalCol = ambient * u_materialAmbientCol.rgb * materialAmbientFactor + lo + ambient;
    float exposure = 0.7;
    finalCol = ACESTonemap(finalCol, exposure);
    // finalCol = vec3(tessEvalIn.UV, 0.0);

    layerColor = vec4(finalCol, 1.0);
    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);
    pickingColor = vec3(u_pickingColor);
}

#end

/*#geometry

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in TessEvalOut
{
    vec2 UV;
    vec3 normal;
    vec3 worldNormal;

    vec3 vertexPos;
    vec3 fragPos;
    mat3 TBN;
} tessEvalIn[];

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        int j = (i + 1) % 3;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
        gl_Position = gl_in[j].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
}

#end*/
