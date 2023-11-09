#ifndef LIGHT_STRUCTS_DECL_GLSL
#define LIGHT_STRUCTS_DECL_GLSL

struct ShadowsCaster
{
    mat4 shadowsCasterSpace;
    vec3 position;
};

struct DirectionalLight
{
    vec3 position;
    vec4 color;
    float intensity;
};

#endif // LIGHT_STRUCTS_DECL_GLSL