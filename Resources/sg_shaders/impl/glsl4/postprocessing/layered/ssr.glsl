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

uniform sampler2D SG_SSR_final;

float SG_SSR_smoothness = 1.0;
float SG_SSR_radius = 1000.0;

uniform int SG_SSR_ENABLED;

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

    if(SG_SSR_ENABLED == 0) return;

    // ssr calculate pass.
    // writes in SG_SSR_final
    // reads SGPP_LayersColorsFX
    if(SGPP_CurrentFXSubPassSeqIndex == 0)
    {
        vec3 texelPos = texture(u_GBufferWorldPos, finalUV).xyz;
        vec3 texelNormal = normalize(texture(u_GBufferFragmentNormal, finalUV).xyz);
        vec4 albedo = texture(SGPP_LayersColorsFX, finalUV);

        vec3 viewPos = (camera.viewMatrix * vec4(texelPos, 1.0)).xyz;
        vec3 viewNormal = normalize(mat3(camera.viewMatrix) * texelNormal);

        vec3 viewDir = normalize(viewPos); // camera -> surface in view space
        vec3 reflectDir = normalize(reflect(viewDir, viewNormal));

        const int maxSteps = 30;
        const int binarySteps = 4;
        float maxRayDist = max(SG_SSR_radius, 0.1);
        float stepSize = maxRayDist / float(maxSteps);
        float thickness = stepSize * 2.0;

        vec3 rayPos = viewPos;
        vec3 prevRayPos = rayPos;
        float prevDepthDiff = 0.0;

        vec3 result = vec3(0.0);
        float hit = 0.0;

        for(int i = 0; i < maxSteps; ++i)
        {
            rayPos += reflectDir * stepSize;

            vec4 clipPos = camera.projectionMatrix * vec4(rayPos, 1.0);
            if(clipPos.w <= 0.0) break;

            vec2 nuv = clipPos.xy / clipPos.w;
            nuv = nuv * 0.5 + 0.5;

            if(nuv.x <= 0.001 || nuv.x >= 0.999 || nuv.y <= 0.001 || nuv.y >= 0.999) break;

            vec3 sampleWorldPos = texture(u_GBufferWorldPos, nuv).xyz;
            vec3 sampleViewPos = (camera.viewMatrix * vec4(sampleWorldPos, 1.0)).xyz;

            float rayDepth = -rayPos.z;
            float sceneDepth = -sampleViewPos.z;
            float depthDiff = rayDepth - sceneDepth;

            if(depthDiff > 0.0)
            {
                vec3 a = prevRayPos;
                vec3 b = rayPos;

                for(int j = 0; j < binarySteps; ++j)
                {
                    vec3 mid = (a + b) * 0.5;
                    vec4 midClip = camera.projectionMatrix * vec4(mid, 1.0);
                    vec2 midUV = (midClip.xy / midClip.w) * 0.5 + 0.5;

                    vec3 midWorldPos = texture(u_GBufferWorldPos, midUV).xyz;
                    vec3 midViewPos = (camera.viewMatrix * vec4(midWorldPos, 1.0)).xyz;

                    float midDepthDiff = (-mid.z) - (-midViewPos.z);
                    if(midDepthDiff > 0.0) b = mid; else a = mid;
                }

                vec4 hitClip = camera.projectionMatrix * vec4(b, 1.0);
                vec2 hitUV = (hitClip.xy / hitClip.w) * 0.5 + 0.5;

                result = texture(SGPP_LayersColorsFX, hitUV).rgb;
                hit = step(0.0, thickness - abs(depthDiff));
                break;
            }

            prevRayPos = rayPos;
            prevDepthDiff = depthDiff;
        }

        vec3 viewToCamera = normalize(-viewPos); // surface -> camera
        float fresnel = pow(1.0 - clamp(dot(viewNormal, viewToCamera), 0.0, 1.0), 5.0);
        float reflectivity = clamp(SG_SSR_smoothness, 0.0, 1.0);
        float ssrWeight = fresnel * reflectivity * hit;

        fragColor = vec4(mix(albedo.rgb, result, ssrWeight), albedo.a);
    }
    // SSR write pass.
    // writes in SGPP_LayersColorsFX
    // reads SG_SSR_final
    else if(SGPP_CurrentFXSubPassSeqIndex == 1)
    {
        vec4 albedo = texture(SG_SSR_final, finalUV);
        fragColor = albedo;
    }
}

#end