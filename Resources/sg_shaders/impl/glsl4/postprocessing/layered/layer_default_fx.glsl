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

uniform sampler2D u_GBufferWorldPos;

// epsilon number
const float EPSILON = 0.00001f;

out vec4 fragColor;

uniform float u_FogDensity = 0.0005;

vec3 Uncharted2Tonemap(vec3 x)
{
    const float A = 0.15;
    const float B = 0.50;
    const float C = 0.10;
    const float D = 0.20;
    const float E = 0.02;
    const float F = 0.30;
    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F)) - E/F;
}

vec3 ToneMap_Uncharted2(vec3 color)
{
    float exposureBias = 0.5;
    color *= exposureBias;

    vec3 curr = Uncharted2Tonemap(color);

    // White point normalization (to map 11.2 to 1.0)
    const float W = 0.5;
    vec3 whiteScale = 1.0 / Uncharted2Tonemap(vec3(W));
    return curr * whiteScale;
}

vec3 applyFog(in vec3 col,   // color of pixel
              in float t,     // distance to point
              in vec3 rd,    // camera to point
              in vec3 lig)  // sun direction
{
    float sunAmount = clamp(dot(rd, lig), 0.0, 1.0);
    float fogAmount = 1.0 - exp(-t * u_FogDensity);
    // float fogFactor = exp(-distance * mix(be, be * 0.3, sunAmount));
    vec3  fogColor  = mix(vec3(0.5,0.6,0.7), // blue
                          vec3(1.0,0.9,0.7), // yellow
                          pow(sunAmount, 8.0));

    return mix(col, fogColor, fogAmount);
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

            vec2 STColorTexSize = vec2(0.0, 0.0);

            {
                ivec2 tmpSize = textureSize(SGPP_LayersSTColor, 0);
                STColorTexSize = vec2(float(tmpSize.x), float(tmpSize.y));
            }

            // STColor.rgba = gaussianBlur(SGPP_LayersSTColor, gl_FragCoord.xy / STColorTexSize).rgba;

            // STColor.rgba = texture(SGPP_LayersWBOITColorAccum, (gl_FragCoord.xy) / wboitAccumulationTexSize).rgba;
            // STColor.rgba = texelFetch(SGPP_LayersSTColor, texelCoord, 0).rgba;

            // STColor.rgb = ACESTonemap(STColor.rgb, exposure);

            // layerColor.rgb = ACESTonemap(layerColor.rgb);

            fragColor = vec4(layerColor.rgb * (1.0 - STColor.a) + STColor.rgb * (STColor.a), 1.0);

            vec3 fragPos = texture(u_GBufferWorldPos, finalUV).xyz;

            /*float f_distance = length(fragPos - camera.position);
            float f_height = fragPos.y;

            float fogFactorDist = smoothstep(uFogStart, uFogEnd, f_distance);

            float fogFactorHeight = exp(f_height * uFogHeightFalloff);
            fogFactorHeight = clamp(1.0 - fogFactorHeight, 0.0, 1.0);

            float fogFactor = fogFactorDist;*/

            float exposure = 0.7;
            // fragColor.rgb = ACESTonemap(fragColor.rgb, exposure);

            // fragColor.rgb = ToneMap_Uncharted2(fragColor.rgb);

            vec3 viewDir = normalize(fragPos - camera.position);
            // fragColor.rgb = applyFog(fragColor.rgb, distance(fragPos, camera.position), viewDir, atmosphere.sunPosition);
        }
    }
}

#end
