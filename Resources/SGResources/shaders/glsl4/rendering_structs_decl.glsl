#sg_pragma once

#define DIRECTIONAL_LIGHTS_MAX_COUNT    5

struct IRenderingComponent
{
    mat4 projectionMatrix; // can be unused
    mat4 viewMatrix; // can be unused
    mat4 spaceMatrix;
    vec3 position;
    float p0;
    vec3 rotation; // can be unused
    float p1;
    vec3 scale; // can be unused
    float p2;
};

struct ILight extends IRenderingComponent
{
    vec4 color;
    float intensity;
    int shadowSamplesCount;
};

// 264 байта с паддингами p0, p1, p2
// нужно 272 байта
struct DirectionalLight extends ILight
{
    float p3;
    float p4;
    // todo: make for dir light
};

struct Atmosphere
{
    vec3 sunPosition;
    float p0;

    vec3 sunColor;
    float rayleighScaleHeight;

    vec3 rayleighScatteringCoeff;
    float mieScatteringCoeff;

    float mieScaleHeight;
    float sunIntensity;

    float planetRadius;
    float atmosphereRadius;

    vec3 rayOrigin;
    float miePreferredScatteringDirection;
};
