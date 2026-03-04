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
uniform sampler2D u_GBufferMaterialsInfo;

uniform sampler2D SG_SSR_raw;
uniform sampler2D SG_SSR_final;

uniform float SG_SSR_blur_radius;
uniform int SG_SSR_max_steps;
uniform float SG_SSR_initial_step_size;
uniform float SG_SSR_min_step_size;
uniform float SG_SSR_intensity;

out vec4 fragColor;

vec3 getUV(vec3 position)
{
    vec4 pVP = camera.projectionSpaceMatrix * vec4(position, 1.0f);
    pVP.xyz = 0.5 + 0.5 * pVP.xyz / pVP.w;
    return pVP.xyz;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

#define Scale vec3(.8, .8, .8)
#define K 19.19

vec3 hash(vec3 a)
{
    a = fract(a * Scale);
    a += dot(a, a.yxz + K);
    return fract((a.xxy + a.yxx)*a.zyx);
}

void main()
{
    vec2 finalUV = vs_UVAttribute.xy;

    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - vs_UVAttribute.y;
    #endif

    vec4 albedo = texture(SGPP_LayersColorsFX, finalUV);

    vec3 roughnessMetallicSpecular = texture(u_GBufferMaterialsInfo, finalUV).rgb;
    float roughness = roughnessMetallicSpecular.r;
    float metallic = roughnessMetallicSpecular.g;
    float specular = roughnessMetallicSpecular.b;

    if(metallic < 0.01)
    {
        fragColor = albedo;
        return;
    }

    // ssr calculate pass.
    // writes in SG_SSR_raw
    // reads SGPP_LayersColorsFX
    if(SGPP_CurrentFXSubPassSeqIndex == 0)
    {
        vec3 texelPos = textureLod(u_GBufferWorldPos, finalUV, 2).xyz;
        vec3 texelNormal = textureLod(u_GBufferFragmentNormal, finalUV, 2).xyz;

        /*vec3 F0 = vec3(0.04);
        F0 = mix(F0, albedo.rgb, metallic);
        vec3 fresnel = fresnelSchlick(max(dot(texelNormal, texelPos), 0.0), F0);*/

        // vec3 jittering = mix(vec3(0.0), vec3(hash(texelPos)), specular);

        vec3 viewDir = normalize(texelPos - camera.position);

        vec3 reflectedDir = normalize(reflect(viewDir, texelNormal));

        vec3 jittering = mix(vec3(0.0), vec3(hash((vec4(texelPos, 1.0) * inverse(camera.viewMatrix)).xyz)), roughness * (1.0 - specular));

        vec3 currentRayPos = vec3(0.0);
        vec3 currentSamplePos = vec3(0.0);

        float stepSize = SG_SSR_initial_step_size; // initial step size

        vec3 ssrColor = vec3(0.0);
        float ssrCoeff = 0.0;

        for(int i = 0; i < SG_SSR_max_steps; ++i)
        {
            currentRayPos = texelPos + reflectedDir * stepSize + jittering;

            vec3 rayUV = getUV(currentRayPos);

            currentSamplePos = textureLod(u_GBufferWorldPos, rayUV.xy, 2).xyz;

            float distToSample = length(currentRayPos - currentSamplePos);
            float rayLength = length(currentRayPos - texelPos);

            if(distToSample < 0.3)
            {
                vec2 dCoords = smoothstep(vec2(0.2), vec2(0.6), abs(vec2(0.5) - rayUV.xy));

                ssrColor = textureLod(SGPP_LayersColorsFX, rayUV.xy, 2).rgb;

                float rayLength = length(currentRayPos - texelPos);

                float screenEdgeFactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);

                ssrCoeff = clamp((1.0 / (rayLength * rayLength)) * SG_SSR_intensity, 0.0, 1.0) * screenEdgeFactor;
                // ssrCoeff = screenEdgeFactor;

                break;
            }

            stepSize = max(SG_SSR_min_step_size, length(texelPos - currentSamplePos));
        }

        fragColor = vec4(ssrColor * ssrCoeff /* * fresnel */, albedo.a);
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
                vec2 offset = vec2(x, y) * texelSize * SG_SSR_blur_radius;

                result += texture(SG_SSR_raw, finalUV + offset);

                iterationsCount += 1.0;
            }
        }

        result /= iterationsCount;

        fragColor = vec4(albedo.rgb + result.rgb, albedo.a);
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