#include "utils.glsl"

#subpass [SGPPLayerFXPass]

// includes vertex pass
#include "base.glsl"

#fragment

#include "sg_shaders/impl/glsl4/color_correction/aces.glsl"
#include "sg_shaders/impl/glsl4/color_correction/lottes.glsl"
#include "sg_shaders/impl/glsl4/color_correction/reinhard.glsl"
#include "sg_shaders/impl/glsl4/color_correction/filmic.glsl"
#include "sg_shaders/impl/glsl4/color_correction/neutral.glsl"
#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/random.glsl"

// ===================================================
// !! - TO GET INDICES OF ALL LAYERS USE UNIFORM ARRAY 'SGPP_LayersIndices' (UNIFORM INT)
// !! - TO GET LAYERS COUNT USE UNIFORM INT 'SGPP_LayersCount'
// !! - TO GET CURRENT LAYER INDEX USE UNIFORM INT 'SGPP_CurrentLayerIndex'. !! WARNING - THE INDEXES OF THE LAYERS ARE NOT NECESSARILY SEQUENTIAL
// !! - TO GET CURRENT LAYER CURRENT SUB PASS INDEX USE UNIFORM INT 'SGPP_CurrentFXSubPassSeqIndex'
// ===================================================

// =========== HOW TO WRITE/READ COLORS ==============
// !! USE LOCATION 7 TO WRITE IN DEFAULT ATTACHMENT THAT CONTAINS EFFECTS
// !! USE UNIFORM SAMPLER2D 'SGPP_LayersVolumes' TO GET LAYERS VOLUMES
// !! USE UNIFORM SAMPLER2D 'SGPP_LayersColors' TO GET LAYERS COLORS WITHOUT POSTPROCESS EFFECTS
// !! USE UNIFORM SAMPLER2D 'SGPP_LayersColorFX' TO GET LAYERS COLORS WITH POSTPROCESS EFFECTS
// !! YOU CAN ADD YOUR CUSTOM INTERMEDIATE ATTACHMENTS TO STORE EFFECTS
// ===================================================

in vec2 vs_UVAttribute;

uniform sampler2D SGPP_LayersVolumes;
uniform sampler2D SGPP_LayersColors;
uniform sampler2D SGPP_LayersColorsFX;
// ST - stochastic transparency
uniform sampler2D SGPP_LayersSTColor;
uniform int SGPP_CurrentLayerIndex;
uniform int SGPP_CurrentFXSubPassSeqIndex;

uniform sampler2D u_GBufferWorldPos;
uniform sampler2D u_GBufferFragmentNormal;

uniform int SG_SSAO_samplesCount;
uniform vec3 SG_SSAO_samples[256];

uniform sampler2D SG_SSAO_noise;
uniform sampler2D SG_SSAO_occlusion;
uniform sampler2D SG_SSAO_final;
// uniform sampler2D SG_SSAO_occlusionBlurredTexture;
uniform int SG_SSAO_ENABLED;

out vec4 fragColor;

void main()
{
    vec2 finalUV = vs_UVAttribute.xy;

    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - vs_UVAttribute.y;
    #endif

    if(SG_SSAO_ENABLED == 0) return;

    // just do nothing
    if(texture(SGPP_LayersVolumes, finalUV).rgb != calculatePPLayerVolume(SGPP_CurrentLayerIndex).rgb) return;

    vec2 noiseTexSize = vec2(textureSize(SG_SSAO_noise, 0));
    vec2 noiseScale = vec2(programData.primaryMonitorSize.x / noiseTexSize.x, programData.primaryMonitorSize.y / noiseTexSize.y);

    vec3 fragPos = vec3(camera.viewMatrix * texture(u_GBufferWorldPos, finalUV));

    // ssao occlusion pass.
    // writes in SG_SSAO_occlusion
    // reads only GBUFFER attachments
    if(SGPP_CurrentFXSubPassSeqIndex == 0)
    {
        vec3 normal = normalize(mat3(camera.viewMatrix) * texture(u_GBufferFragmentNormal, finalUV).xyz);
        vec3 randomVec = texture(SG_SSAO_noise, finalUV * noiseScale).xyz;

        vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
        vec3 bitangent = cross(normal, tangent);
        mat3 TBN = mat3(tangent, bitangent, normal);

        float occlusion = 0.0;
        float radius = 0.5;
        float bias = 0.025;

        for (int i = 0; i < SG_SSAO_samplesCount; ++i)
        {
            vec3 smp = TBN * SG_SSAO_samples[i];
            smp = fragPos + smp * radius;

            vec4 offset = vec4(smp, 1.0);
            offset = camera.projectionMatrix * offset; // переход из видового в клиповое
            offset.xyz /= offset.w; // перспективное деление
            offset.xyz = offset.xyz * 0.5 + 0.5;

            offset.x = clamp(offset.x, 0.001, 0.999);
            offset.y = clamp(offset.y, 0.001, 0.999);

            float sampleDepth = vec3(camera.viewMatrix * texture(u_GBufferWorldPos, offset.xy)).z;
            float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
            occlusion += (sampleDepth >= smp.z + bias ? 1.0 : 0.0) * rangeCheck;
        }

        occlusion = 1.0 - (occlusion / float(SG_SSAO_samplesCount));

        fragColor = vec4(vec3(occlusion), 1.0);
    }
    // ssao blur pass. takes color from final FX attachment (attachment 7 - SGPP_LayersColorsFX)
    // writes in SG_SSAO_final
    // reads SG_SSAO_occlusion and SGPP_LayersColorsFX
    else if(SGPP_CurrentFXSubPassSeqIndex == 1)
    {
        vec2 texelSize = 1.0 / vec2(textureSize(SG_SSAO_occlusion, 0));
        float result = 0.0;

        float iterationsCount = 0.0;

        for (float x = -2.0; x < 2.0; x += 1.0)
        {
            for (float y = -2.0; y < 2.0; y += 1.0)
            {
                vec2 offset = vec2(x, y) * texelSize;

                result += texture(SG_SSAO_occlusion, finalUV + offset).r;

                iterationsCount += 1.0;
            }
        }

        vec4 albedo = texture(SGPP_LayersColorsFX, finalUV);
        float ssaoCoeff = result / iterationsCount;

        fragColor = vec4(albedo.rgb * ssaoCoeff, albedo.a);
    }
    // ssao write pass
    // writes in SGPP_LayersColorsFX
    // reads SG_SSAO_final
    else if(SGPP_CurrentFXSubPassSeqIndex == 2)
    {
        vec4 albedo = texture(SG_SSAO_final, finalUV);
        fragColor = albedo;
    }
}

#end