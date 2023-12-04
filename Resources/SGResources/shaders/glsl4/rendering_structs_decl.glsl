#ifndef RENDERING_STRUCTS_DECL_GLSL
#define RENDERING_STRUCTS_DECL_GLSL

#define DIRECTIONAL_LIGHTS_MAX_COUNT    5

struct IRenderingComponent
{
    mat4 projectionMatrix; // can be unused
    mat4 viewMatrix; // can be unused
    mat4 spaceMatrix;
    vec3 position;
    vec3 rotation; // can be unused
    vec3 scale; // can be unused
};

struct ILight
{
    IRenderingComponent renderingPart;
    vec4 color;
    float intensity;
    int shadowSamplesCount;
};

struct DirectionalLight
{
    ILight lightPart;
    // todo: make for dir light
};

#ifndef SG_NOT_INCLUDE_LIGHTS
    uniform int DIRECTIONAL_LIGHTS_COUNT;
    uniform DirectionalLight directionalLights[DIRECTIONAL_LIGHTS_MAX_COUNT];
    uniform sampler2D directionalLightsShadowMaps[DIRECTIONAL_LIGHTS_MAX_COUNT];
#endif // SG_INCLUDE_DIR_LIGHTS

#endif // RENDERING_STRUCTS_DECL_GLSL