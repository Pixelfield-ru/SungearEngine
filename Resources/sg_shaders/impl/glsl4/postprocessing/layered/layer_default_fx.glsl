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
// ST - stochastic transparency
uniform sampler2D SGPP_LayersSTColor;
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

vec4 gaussianBlur(in sampler2D inputTexture, vec2 uv) {
    vec2 offsets[5] = vec2[](
        vec2(-2.0, 0.0),
        vec2(-1.0, 0.0),
        vec2(0.0, 0.0),
        vec2(1.0, 0.0),
        vec2(2.0, 0.0)
    );

    float weights[5] = float[](0.05, 0.2, 0.5, 0.2, 0.05);

    vec4 result = vec4(0.0);
    for (int i = 0; i < 5; i++)
    {
        result += texture(inputTexture, uv + offsets[i] * 0.001) * weights[i];
    }
    return result;
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

            vec4 STColor = vec4(0.0, 0.0, 0.0, 0.0);

            const int BOX_SIZE = 1;

            int passesCount = 0;

            vec2 STColorTexSize = vec2(0.0, 0.0);

            {
                ivec2 tmpSize = textureSize(SGPP_LayersSTColor, 0);
                STColorTexSize = vec2(float(tmpSize.x), float(tmpSize.y));
            }

            /*for(int x = -BOX_SIZE; x <= BOX_SIZE; ++x)
            {
                for(int y = -BOX_SIZE; y <= BOX_SIZE; ++y)
                {
                    float tmpRevealage = texelFetch(SGPP_LayersWBOITReveal, texelCoord + ivec2(x, y), 0).r;

                    if(tmpRevealage != 0.0)
                    {
                        wboitAccumulation.rgb += gaussianBlur(SGPP_LayersWBOITColorAccum, (gl_FragCoord.xy + vec2(float(x), float(y)) / 1.0) / wboitAccumulationTexSize).rgb;
                        ++passesCount;
                    }
                }
            }*/

            STColor.rgba = gaussianBlur(SGPP_LayersSTColor, gl_FragCoord.xy / STColorTexSize).rgba;

            // STColor.rgba = texture(SGPP_LayersWBOITColorAccum, (gl_FragCoord.xy) / wboitAccumulationTexSize).rgba;
            // STColor.rgba = texelFetch(SGPP_LayersSTColor, texelCoord, 0).rgba;

            // STColor.rgb = ACESTonemap(STColor.rgb, exposure);

            // layerColor.rgb = ACESTonemap(layerColor.rgb);

            fragColor = vec4(layerColor.rgb * (1.0 - STColor.a) + STColor.rgb * (STColor.a), 1.0);
        }
    }
}

#end
