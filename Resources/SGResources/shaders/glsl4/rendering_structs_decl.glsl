#sg_pragma once

#define DIRECTIONAL_LIGHTS_MAX_COUNT    5

struct IRenderingComponent
{
    mat4 projectionMatrix; // can be unused
    mat4 viewMatrix; // can be unused
    mat4 spaceMatrix;
    vec3 position;
    // float p0;
    vec3 rotation; // can be unused
    // float p1;
    vec3 scale; // can be unused
    // float p2;
};

struct ILight extends IRenderingComponent
{
    vec4 color;
    float intensity;
    int shadowSamplesCount;
};

struct DirectionalLight extends ILight
{
    // todo: make for dir light
};
