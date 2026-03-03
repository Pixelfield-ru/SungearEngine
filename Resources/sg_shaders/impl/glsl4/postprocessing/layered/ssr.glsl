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

uniform sampler2D SG_SSR_raw;
uniform sampler2D SG_SSR_final;

float SG_SSR_smoothness = 1.0;
float SG_SSR_radius = 1000.0;

out vec4 fragColor;

vec2 getUV(vec3 position)
{
    vec4 pVP = camera.projectionSpaceMatrix * vec4(position, 1.0f);
    pVP.xy = 0.5 + 0.5 * pVP.xy / pVP.w;
    return pVP.xy;
}

vec2 worldToUV(vec3 worldPos)
{
    vec4 viewPos = camera.viewMatrix * vec4(worldPos, 1.0);
    vec4 clipPos = camera.projectionMatrix * viewPos;
    vec3 ndc = clipPos.xyz / clipPos.w;
    return ndc.xy * 0.5 + 0.5;
}

void main()
{
    vec2 finalUV = vs_UVAttribute.xy;

    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - vs_UVAttribute.y;
    #endif

    // ssr calculate pass.
    // writes in SG_SSR_raw
    // reads SGPP_LayersColorsFX
    if(SGPP_CurrentFXSubPassSeqIndex == 0)
    {
        vec3 texelPos = texture(u_GBufferWorldPos, finalUV).xyz;
        vec3 texelNormal = normalize(texture(u_GBufferFragmentNormal, finalUV).xyz);
        vec4 albedo = texture(SGPP_LayersColorsFX, finalUV);

        vec3 viewDir = normalize(texelPos - camera.position);

        vec3 reflectedDir = normalize(reflect(viewDir, texelNormal));

        vec3 currentRayPos = vec3(0.0);
        vec3 currentSamplePos = vec3(0.0);

        const int MAX_STEPS = 10;
        float stepSize = 5.0; // initial step size

        vec3 ssrColor = vec3(0.0);
        float ssrCoeff = 0.0;

        for(int i = 0; i < MAX_STEPS; ++i)
        {
            currentRayPos = texelPos + reflectedDir * stepSize;

            vec2 rayUV = getUV(currentRayPos);

            if(rayUV.x < 0.0 || rayUV.x > 1.0 || rayUV.y < 0.0 || rayUV.y > 1.0)
            {
                ssrColor = albedo.rgb;
                break;
            }

            currentSamplePos = texture(u_GBufferWorldPos, rayUV).xyz;

            float distToSample = length(currentRayPos - currentSamplePos);
            float rayLength = length(currentRayPos - texelPos);

            if(distToSample < 0.3)
            {
                ssrColor = texture(SGPP_LayersColorsFX, rayUV).rgb;

                float rayLength = length(currentRayPos - texelPos);

                ssrCoeff = clamp((1.0 / (rayLength * rayLength)) * 25000.0, 0.0, 1.0);

                break;
            }

            stepSize = length(texelPos - currentSamplePos);
        }

        fragColor = vec4(albedo.rgb + ssrColor * ssrCoeff, albedo.a);
    }
    // SSR blur pass.
    // writes in SG_SSR_final.
    // reads SG_SSR_raw.
    else if(SGPP_CurrentFXSubPassSeqIndex == 1)
    {
        // vec4 albedo = texture(SG_SSR_final, finalUV);

        vec2 texelSize = 1.0 / vec2(textureSize(SG_SSR_raw, 0));

        vec4 result = vec4(0.0);

        float iterationsCount = 0.0;

        for (float x = -2.0; x < 2.0; x += 1.0)
        {
            for (float y = -2.0; y < 2.0; y += 1.0)
            {
                vec2 offset = vec2(x, y) * texelSize;

                result += texture(SG_SSR_raw, finalUV + offset);

                iterationsCount += 1.0;
            }
        }

        result /= iterationsCount;

        fragColor = result;
    }
    // SSR write pass.
    // writes in SGPP_LayersColorsFX.
    // reads SG_SSR_final.
    else if(SGPP_CurrentFXSubPassSeqIndex == 2)
    {
        vec4 albedo = texture(SG_SSR_final, finalUV);
        fragColor = albedo;
    }
}

#end