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
#include "sg_shaders/impl/glsl4/color_correction/aces.glsl"
#include "sg_shaders/impl/glsl4/gaussian_weights.glsl"

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

uniform sampler2D SG_BLOOM_blur_pass_1;
uniform sampler2D SG_BLOOM_blur_pass_2;

out vec4 fragColor;

void main()
{
    vec2 finalUV = vs_UVAttribute.xy;

    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - vs_UVAttribute.y;
    #endif

    // const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    vec4 albedo = texture(SGPP_LayersColorsFX, finalUV);

    vec2 texelSize = 1.0 / textureSize(SGPP_LayersColorsFX, 0) * 2.0;

    // bloom bright fragments selection pass.
    // writes in SG_BLOOM_blur_pass_2
    // reads SGPP_LayersColorsFX
    if(SGPP_CurrentFXSubPassSeqIndex == 0)
    {
        vec3 brightFragColor = vec3(0.0);

        float brightness = dot(albedo.rgb, vec3(0.2126, 0.7152, 0.0722));
        if(brightness > 0.999 && texture(SGPP_LayersVolumes, finalUV).rgb == calculatePPLayerVolume(SGPP_CurrentLayerIndex).rgb)
        {
            brightFragColor = texture(SGPP_LayersColorsFX, finalUV).rgb;
        }

        fragColor = vec4(brightFragColor, albedo.a);
    }
    // bloom first blur pass.
    // writes in SG_BLOOM_blur_pass_1
    // reads SG_BLOOM_blur_pass_2
    else if(SGPP_CurrentFXSubPassSeqIndex == 1)
    {
        vec3 result = texture(SG_BLOOM_blur_pass_2, finalUV).rgb * gaussianKernel7[0];

        float totalWeight = 0.0;

        for(int i = 1; i < 7; ++i)
        {
            float weight = gaussianKernel7[i];

            vec2 right = finalUV + vec2(texelSize.x * i, 0.0);
            right.x = min(right.x, 1.0);

            vec2 left = finalUV - vec2(texelSize.x * i, 0.0);
            left.x = max(left.x, 0.0);

            result += texture(SG_BLOOM_blur_pass_2, right).rgb * weight;
            result += texture(SG_BLOOM_blur_pass_2, left).rgb * weight;

            totalWeight += weight * 2.0;
        }

        fragColor = vec4(result / totalWeight, albedo.a);
    }
    // bloom second blur pass.
    // writes in SG_BLOOM_blur_pass_2
    // reads SG_BLOOM_blur_pass_1
    else if(SGPP_CurrentFXSubPassSeqIndex == 2)
    {
        vec3 result = texture(SG_BLOOM_blur_pass_1, finalUV).rgb * gaussianKernel7[0];

        float totalWeight = 0.0;

        for(int i = 1; i < 7; ++i)
        {
            float weight = gaussianKernel7[i];

            vec2 up = finalUV + vec2(0.0, texelSize.y * i);
            up.y = min(up.y, 1.0);

            vec2 down = finalUV - vec2(0.0, texelSize.y * i);
            down.y = max(down.y, 0.0);

            result += texture(SG_BLOOM_blur_pass_1, up).rgb * weight;
            result += texture(SG_BLOOM_blur_pass_1, down).rgb * weight;

            totalWeight += weight * 2.0;
        }

        fragColor = vec4(result / totalWeight, albedo.a);
    }
    // bloom write pass.
    // writes in SGPP_LayersColorsFX
    // reads SG_BLOOM_blur_pass_2
    else if(SGPP_CurrentFXSubPassSeqIndex == 3)
    {
        vec4 bluredCol = texture(SG_BLOOM_blur_pass_2, finalUV);

        // float luminance = dot(albedo.rgb, vec3(0.2126, 0.7152, 0.0722));
        float bloomLuminance = dot(bluredCol.rgb, vec3(0.2126, 0.7152, 0.0722));

        const float bloomIntensity = 0.7;
        // bluredCol = min(bluredCol, vec4(brightness));
        /*if(brightness >= maxBrightness)
        {
            bluredCol *= 1.0 / (brightness / maxBrightness);
        }*/

        // float bloomFactor = (1.0 - luminance) * bloomIntensity;
        // fragColor = vec4(albedo.rgb + ACESTonemap(bluredCol.rgb, 0.7), albedo.a);
        fragColor = albedo + bluredCol * bloomIntensity;
        // fragColor = bluredCol;
        // fragColor = albedo + mix(bluredCol, albedo, bloomLuminance);
    }
}

#end