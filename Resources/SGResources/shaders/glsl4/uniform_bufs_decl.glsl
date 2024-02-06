#sg_ifndef UNIFORM_BUFS_DECL_GLSL
#sg_define UNIFORM_BUFS_DECL_GLSL

#sg_include "defines.glsl"
#sg_include "rendering_structs_decl.glsl"

uniform mat4 objectModelMatrix;
uniform vec3 objectPosition;
uniform vec3 objectRotation;
uniform vec3 objectScale;

uniform vec4 materialDiffuseCol;
uniform vec4 materialSpecularCol;
uniform vec4 materialAmbientCol;
uniform vec4 materialEmissionCol;
uniform vec4 materialTransparentCol;
uniform float materialShininess;
uniform float materialMetallicFactor;
uniform float materialRoughnessFactor;

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
        int directionalLightsCount;
        DirectionalLight directionalLights[DIRECTIONAL_LIGHTS_MAX_COUNT];
    };

    // Suitable only for one-way (directional) light sources!
    uniform sampler2D sgmat_shadowMap2DSamplers[DIRECTIONAL_LIGHTS_MAX_COUNT];
#endif

#sg_endif // UNIFORM_BUFS_DECL_GLSL