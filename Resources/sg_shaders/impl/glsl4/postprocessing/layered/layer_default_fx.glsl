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

// ===================================================
// !! - TO GET INDICES OF ALL LAYERS USE UNIFORM ARRAY 'SGPP_LayersIndices' (UNIFORM INT)
// !! - TO GET LAYERS COUNT USE UNIFORM INT 'SGPP_LayersCount'
// !! - TO GET CURRENT LAYER INDEX USE UNIFORM INT 'SGPP_CurrentLayerIndex'. !! WARNING - THE INDEXES OF THE LAYERS ARE NOT NECESSARILY SEQUENTIAL
// !! - TO GET CURRENT LAYER SEQUENTIAL INDEX USE UNIFORM INT 'SGPP_CurrentLayerSeqIndex'
// !! - TO GET CURRENT LAYER CURRENT SUB PASS INDEX USE UNIFORM INT 'SGPP_CurrentSubPassIndex'
// !! - TO GET CURRENT FX SUBPASS SEQUENTIAL INDEX USE UNIFORM INT 'SGPP_CurrentFXSubPassSeqIndex'
// ===================================================

// =========== HOW TO WRITE/READ COLORS ==============
// !! USE LOCATION 7 TO WRITE IN DEFAULT ATTACHMENT THAT CONTAINS EFFECTS
// !! USE UNIFORM SAMPLER2D 'SGPP_LayersVolumes' TO GET LAYERS VOLUMES
// !! USE UNIFORM SAMPLER2D 'SGPP_LayersColors' TO GET LAYERS COLORS WITHOUT POSTPROCESS EFFECTS
// !! YOU CAN ADD YOUR CUSTOM INTERMEDIATE ATTACHMENTS TO STORE EFFECTS
// ===================================================

in vec2 vs_UVAttribute;

uniform sampler2D SGPP_LayersVolumes;
uniform sampler2D SGPP_LayersColors;
uniform sampler2D SGPP_LayersWBOITColorAccum;
uniform sampler2D SGPP_LayersWBOITReveal;
uniform int SGPP_CurrentLayerIndex;

// epsilon number
const float EPSILON = 0.00001f;

out vec4 fragColor;

// calculate floating point numbers equality accurately
bool isApproximatelyEqual(float a, float b)
{
    return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * EPSILON;
}

// get the max value between three values
float max3(vec3 v)
{
    return max(max(v.x, v.y), v.z);
}


void main()
{
    vec2 finalUV = vs_UVAttribute.xy;

    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - vs_UVAttribute.y;
    #endif

    const float exposure = 0.9;

    // just do nothing
    if(texture(SGPP_LayersVolumes, finalUV).rgb == calculatePPLayerVolume(SGPP_CurrentLayerIndex).rgb)
    {
        // vec4 layerColor = texture(SGPP_LayersColors, finalUV);

        // col.rgb = lottes(col.rgb);
        // col.rgb = reinhard2(col.rgb);
        // col.rgb = reinhard(col.rgb);
        // col.rgb = neutral(col.rgb);
        // col.rgb = filmic(col.rgb);

        // todo: make
        // if(u_WBOITEnabled == 1)
        {
            ivec2 texelCoord = ivec2(gl_FragCoord.xy);

            // vec4 layerColor = texelFetch(SGPP_LayersColors, texelCoord, 0);
            vec4 layerColor = texture(SGPP_LayersColors, finalUV);

            float wboitRevealage = texelFetch(SGPP_LayersWBOITReveal, texelCoord, 0).r;

            if (isApproximatelyEqual(wboitRevealage, 1.0f))
            {
                // discard;
            }

            vec4 wboitAccumulation = texelFetch(SGPP_LayersWBOITColorAccum, texelCoord, 0);

            if (isinf(max3(abs(wboitAccumulation.rgb))))
            {
                // wboitAccumulation.rgb = vec3(wboitAccumulation.a);
            }

            wboitAccumulation.rgb = wboitAccumulation.rgb / clamp(wboitRevealage, 1e-4, 5e4);
            // wboitAccumulation.rgb = wboitAccumulation.rgb / wboitRevealage;

            // wboitAccumulation.rgb = ACESTonemap(wboitAccumulation.rgb, exposure);

            wboitAccumulation.a = 1.0 - wboitRevealage;

            layerColor.rgb = ACESTonemap(layerColor.rgb, exposure);

            // fragColor = vec4(layerColor.rgb, 1.0);
            // fragColor = vec4(layerColor.rgb * (1.0 - wboitAccumulation.a) + wboitAccumulation.rgb * (wboitRevealage), wboitRevealage);
            fragColor = vec4(wboitAccumulation.rgb, wboitAccumulation.a);

            // if(wboitAccumulation.a > 0.8) discard;

            /*ivec2 upos = ivec2(gl_FragCoord.xy);
            vec4 cc = texelFetch(SGPP_LayersWBOITColorAccum, upos, 0);
            vec3 sumOfColors = cc.rgb;
            float sumOfWeights = cc.a;

            vec3 colorNT = texelFetch(SGPP_LayersColors, upos, 0).rgb;

            if (sumOfWeights == 0)
            { gl_FragColor = vec4(colorNT, 1.0); return; }

            float alpha = 1 - texelFetch(SGPP_LayersWBOITReveal, upos, 0).r;
            colorNT = sumOfColors / sumOfWeights * alpha +
            colorNT * (1 - alpha);
            gl_FragColor = vec4(colorNT, 1.0);*/
        }
    }
}

#end
