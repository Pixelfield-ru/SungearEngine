#include "sg_shaders/impl/glsl4/defines.glsl"
#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

#subpass [GeometryPass]

#vertex

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;
layout (location = 2) in vec3 normalsAttribute;

out vec3 vs_UVAttribute;
out vec3 vs_fragWorldPos;

void main()
{
    vs_UVAttribute = positionsAttribute;

    vs_fragWorldPos = vec3(objectTransform.modelMatrix * vec4(positionsAttribute, 1.0));

    // gl_Position = camera.spaceMatrix * objectModelMatrix * vec4(positionsAttribute, 1.0);
    gl_Position = camera.projectionMatrix * mat4(mat3(camera.viewMatrix)) * objectTransform.modelMatrix * vec4(positionsAttribute, 1.0);
    //vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(positionsAttribute, 1.0);
    //gl_Position = vec4(pos.xy, pos.w, pos.w);
}

#end

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

#include "atmosphere_scattering.glsl"
#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"
#include "sg_shaders/impl/glsl4/alpha_resolving/wboit.glsl"
#include "sg_shaders/impl/glsl4/alpha_resolving/stochastic_transparency.glsl"
#include "sg_shaders/impl/glsl4/color_correction/aces.glsl"
#include "sg_shaders/impl/glsl4/color_correction/filmic.glsl"

// REQUIRED COLORS!!! ===========
layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 2) out vec3 pickingColor;
// COLOR FOR STOCHASTIC TRANSPARNCY
layout(location = 3) out vec4 layerSTColor;
layout(location = 4) out vec3 layerWorldPosColor;
// REQUIRED COLORS!!! ===========

uniform samplerCube mat_skyboxSamplers[1];
uniform int mat_skyboxSamplers_CURRENT_COUNT;

uniform int SGPP_CurrentLayerIndex;

in vec3 vs_UVAttribute;
in vec3 vs_fragWorldPos;

void main()
{
    vec3 atmosphereCol = atmosphereScattering(
                vs_UVAttribute,                                     // normalized ray direction
                atmosphere.rayOrigin,                               // ray origin
                atmosphere.sunPosition,                             // position of the sun
                atmosphere.sunIntensity,                            // intensity of the sun
                atmosphere.planetRadius,                            // radius of the planet in meters
                atmosphere.atmosphereRadius,                        // radius of the atmosphere in meters
                atmosphere.rayleighScatteringCoeff,                 // Rayleigh scattering coefficient
                atmosphere.mieScatteringCoeff,                      // Mie scattering coefficient
                atmosphere.rayleighScaleHeight,                     // Rayleigh scale height
                atmosphere.mieScaleHeight,                          // Mie scale height
                atmosphere.miePreferredScatteringDirection          // Mie preferred scattering direction
    );

    // atmosphereCol = pow(atmosphereCol, vec3(10.0));

    vec4 skyboxCol = vec4(0.0);

    if(mat_skyboxSamplers_CURRENT_COUNT > 0)
    {
        /*float mixCoeff = 1.0 / mat_skyboxSamplers_CURRENT_COUNT;
        for (int i = 0; i < mat_skyboxSamplers_CURRENT_COUNT; i++)
        {
            skyboxCol += texture(mat_skyboxSamplers[i], vs_UVAttribute.xyz) * mixCoeff;
        }

        float a = dot(skyboxCol.rgb, atmosphereCol.rgb);
        skyboxCol.rgb += atmosphereCol.rgb * (1.0 - skyboxCol.rgb);
        skyboxCol.rgb *= a * atmosphereCol.rgb;*/

        skyboxCol.rgb = atmosphereCol;
    }
    else
    {
        skyboxCol = vec4(atmosphereCol, 1.0);
    }

    // skyboxCol.rgb = filmic(skyboxCol.rgb);

    // if(u_isStochasticTransparencyEnabled) // todo: impl
    {
        if(calculateStochasticTransparencyComponents(skyboxCol.rgb, skyboxCol.a, layerSTColor, layerColor, vs_UVAttribute.xy, 0))
        {
            discard;
        }

        // calculateWBOITComponents(skyboxCol.rgb, skyboxCol.a, gl_FragCoord.z, layerWBOITColorAccum, layerColor, layerWBOITReveal, 0);

        layerColor = skyboxCol;
        // layerWBOITReveal = 0.0;

        /*calculateWBOITComponents(skyboxCol.rgb, skyboxCol.a, gl_FragCoord.z, layerColor, layerWBOITAccumAlpha.r);
        layerWBOITAccumAlpha.a = 1.0;*/
    }

    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);

    layerWorldPosColor = vec3(vs_fragWorldPos) * 10.0;

    // layerWorldPosColor = vec3(1.0);
}

#end