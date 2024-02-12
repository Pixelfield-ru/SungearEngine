#sg_pragma once

#sg_include "defines.glsl"
#sg_include "rendering_structs_decl.glsl"

uniform mat4 objectModelMatrix;
uniform vec3 objectPosition;
uniform vec3 objectRotation;
uniform vec3 objectScale;

// todo: return
/*uniform*/ vec4 materialDiffuseCol = vec4(1.0);
/*uniform*/ vec4 materialSpecularCol = vec4(1.0);
/*uniform*/ vec4 materialAmbientCol = vec4(0.0);
/*uniform*/ vec4 materialEmissionCol = vec4(1.0);
/*uniform*/ vec4 materialTransparentCol = vec4(1.0);
/*uniform*/ float materialShininess = 32.0;
/*uniform*/ float materialMetallicFactor = 1.0;
/*uniform*/ float materialRoughnessFactor = 1.0;

layout(std140) uniform CameraData
{
    IRenderingComponent camera;
};

layout(std140) uniform ProgramData
{
    vec2 windowSize;
    float currentTime;
};

#ifndef SG_NOT_INCLUDE_LIGHTS
    layout(std140) uniform DirectionalLightsBlock
    {
        // int directionalLightsCount;
        DirectionalLight directionalLights[DIRECTIONAL_LIGHTS_MAX_COUNT];
    };

    // Suitable only for one-way (directional) light sources!
    uniform sampler2D sgmat_shadowMap2DSamplers[DIRECTIONAL_LIGHTS_MAX_COUNT];
#endif
