#include "defines.glsl"
#include "structs_decl.glsl"

// todo: return
/*vec4 u_materialDiffuseCol = vec4(1.0);
vec4 u_materialSpecularCol = vec4(1.0);
vec4 u_materialAmbientCol = vec4(1.0);
vec4 u_materialEmissionCol = vec4(1.0);
vec4 u_materialTransparentCol = vec4(1.0);
float u_materialShininess = 32.0;
float u_materialMetallicFactor = 0.2;
float u_materialRoughnessFactor = 0.5;*/
// float u_materialAmbientFactor = 0.25;

uniform vec4 u_materialDiffuseCol;
uniform vec4 u_materialSpecularCol;
uniform vec4 u_materialAmbientCol;
uniform vec4 u_materialEmissionCol;
uniform vec4 u_materialTransparentCol;
uniform float u_materialShininess;
uniform float u_materialMetallicFactor;
uniform float u_materialRoughnessFactor;
float materialAmbientFactor = 0.25;

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

layout(std140) uniform BonesData
{
    mat4 u_bonesTransformations[SG_MAX_BONES_PER_MESH];
    int u_bonesCount;
};

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
