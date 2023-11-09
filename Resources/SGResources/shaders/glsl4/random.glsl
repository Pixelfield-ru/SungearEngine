#ifndef RANDOM_GLSL
#define RANDOM_GLSL

float interleavedGradientNoise(const in vec2 position_screen)
{
    vec3 magic = vec3(0.06711056f, 0.00583715f, 52.9829189f);
    return fract(magic.z * fract(dot(position_screen, magic.xy)));
}

float random(const in vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453123);
}

#endif // RANDOM_GLSL