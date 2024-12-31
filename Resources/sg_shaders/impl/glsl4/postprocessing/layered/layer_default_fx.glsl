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
uniform sampler2D SGPP_LayersWBOITAlphaAccum;
uniform int SGPP_CurrentLayerIndex;

// epsilon number
const float EPSILON = 0.00001f;

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
        vec4 col = texture(SGPP_LayersColors, finalUV);

        // col.rgb = lottes(col.rgb);
        // col.rgb = reinhard2(col.rgb);
        // col.rgb = reinhard(col.rgb);
        // col.rgb = neutral(col.rgb);
        // col.rgb = filmic(col.rgb);

        // todo: make
        // if(u_WBOITEnabled == 1)
        {
            ivec2 texelCoord = ivec2(finalUV * textureSize(SGPP_LayersWBOITAlphaAccum, 0));

            // fragment revealage
            float wboitAccumAlpha = texelFetch(SGPP_LayersWBOITAlphaAccum, texelCoord, 0).r;

            // save the blending and color texture fetch cost if there is not a transparent fragment
            if (isApproximatelyEqual(wboitAccumAlpha, 1.0f))
            {
                // discard;
            }

            texelCoord = ivec2(finalUV * textureSize(SGPP_LayersColors, 0));
            // fragment color
            vec4 accumulation = texelFetch(SGPP_LayersColors, texelCoord, 0);

            if (isinf(max3(abs(accumulation.rgb))))
            {
                accumulation.rgb = vec3(accumulation.a);
            }

            // col.rgb = accumulation.rgb / max(accumulation.a, EPSILON);

            col.rgb = ACESTonemap(col.rgb, exposure);

            // gl_FragColor = col;
            gl_FragColor = vec4(col.rgb, 1.0);
        }
    }
}

#end
