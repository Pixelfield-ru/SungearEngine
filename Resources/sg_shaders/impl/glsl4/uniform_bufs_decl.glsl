#include "defines.glsl"
#include "structs_decl.glsl"

// todo: return
vec4 materialDiffuseCol = vec4(1.0);
vec4 materialSpecularCol = vec4(1.0);
vec4 materialAmbientCol = vec4(1.0);
vec4 materialEmissionCol = vec4(1.0);
vec4 materialTransparentCol = vec4(1.0);
float materialShininess = 32.0;
float materialMetallicFactor = 0.0;
float materialRoughnessFactor = 0.5;
float materialAmbientFactor = 0.25;

/*uniform vec4 materialDiffuseCol;
uniform vec4 materialSpecularCol;
uniform vec4 materialAmbientCol;
uniform vec4 materialEmissionCol;
uniform vec4 materialTransparentCol;
uniform float materialShininess;
uniform float materialMetallicFactor;
uniform float materialRoughnessFactor;*/

layout(std140) uniform CameraData
{
    IRenderingComponent camera;
};

layout(std140) uniform ProgramDataBlock
{
    ProgramData programData;
};

layout(std140) uniform AtmosphereBlock
{
    Atmosphere atmosphere;
};

/*layout(std140) uniform ObjectTransformBlock
{
    ObjectTransform objectTransform;
};*/

uniform ObjectTransform objectTransform;

#ifndef SG_NOT_INCLUDE_LIGHTS
    layout(std140) uniform DirectionalLightsBlock
    {
        DirectionalLight directionalLights[DIRECTIONAL_LIGHTS_MAX_COUNT];
        int directionalLightsCount;
    };

    // uniform int directionalLightsCount;

    // Suitable only for one-way (directional) light sources!
    uniform sampler2D sgmat_shadowMap2DSamplers[DIRECTIONAL_LIGHTS_MAX_COUNT];
#endif
