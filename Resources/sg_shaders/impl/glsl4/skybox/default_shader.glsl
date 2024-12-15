#include "sg_shaders/impl/glsl4/defines.glsl"
#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

#subpass [GeometryPass]

#vertex

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;
layout (location = 2) in vec3 normalsAttribute;

out vec3 vs_UVAttribute;

void main()
{
    vs_UVAttribute = positionsAttribute;

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

layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;

uniform samplerCube mat_skyboxSamplers[1];
uniform int mat_skyboxSamplers_CURRENT_COUNT;

uniform int SGPP_CurrentLayerIndex;

in vec3 vs_UVAttribute;

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

    if(mat_skyboxSamplers_CURRENT_COUNT > 0)
    {
        float mixCoeff = 1.0 / mat_skyboxSamplers_CURRENT_COUNT;
        vec4 skyboxCol = vec4(0.0);
        for (int i = 0; i < mat_skyboxSamplers_CURRENT_COUNT; i++)
        {
            skyboxCol += texture(mat_skyboxSamplers[i], vs_UVAttribute.xyz) * mixCoeff;
        }

        layerColor = vec4(atmosphereCol * skyboxCol.rgb, skyboxCol.a);
    }
    else
    {
        layerColor = vec4(atmosphereCol, 1.0);
    }

    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);
}

#end