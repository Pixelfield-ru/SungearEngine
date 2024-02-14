#sg_pragma once

#sg_include "defines.glsl"
#sg_include "rendering_structs_decl.glsl"

uniform mat4 objectModelMatrix;
uniform vec3 objectPosition;
uniform vec3 objectRotation;
uniform vec3 objectScale;

// todo: return
vec4 materialDiffuseCol = vec4(1.0);
vec4 materialSpecularCol = vec4(1.0);
vec4 materialAmbientCol = vec4(0.0);
vec4 materialEmissionCol = vec4(1.0);
vec4 materialTransparentCol = vec4(1.0);
float materialShininess = 32.0;
float materialMetallicFactor = 1.0;
float materialRoughnessFactor = 1.0;

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

layout(std140) uniform ProgramData
{
    vec2 windowSize;
    float currentTime;
};

layout(std140) uniform SunBlock
{
    vec4 sunColor;
    vec3 sunPosition;
    float p0;
    float sunColors[128];
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
