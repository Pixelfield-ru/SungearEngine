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

uniform sampler2D SG_FILM_GRAIN_final;

uniform float SG_FILM_GRAIN_intensity;

uniform int SG_FILM_GRAIN_ENABLED;

out vec4 fragColor;

void main()
{
    vec2 finalUV = vs_UVAttribute.xy;

    #ifdef FLIP_TEXTURES_Y
    finalUV.y = 1.0 - vs_UVAttribute.y;
    #endif

    if(SG_FILM_GRAIN_ENABLED == 0) return;

    // film grain calculate pass.
    // writes in SG_FILM_GRAIN_final
    // reads SGPP_LayersColorsFX
    if(SGPP_CurrentFXSubPassSeqIndex == 0)
    {
        vec4 albedo = texture(SGPP_LayersColorsFX, finalUV);

        float grain = random(finalUV * programData.currentTime) * SG_FILM_GRAIN_intensity;

        fragColor = vec4(albedo.rgb + grain, albedo.a);
    }
    // film grain write pass.
    // writes in SGPP_LayersColorsFX
    // reads SG_FILM_GRAIN_final
    else if(SGPP_CurrentFXSubPassSeqIndex == 1)
    {
        vec4 albedo = texture(SG_FILM_GRAIN_final, finalUV);
        fragColor = albedo;
    }
}

#end