float saturate(float a) { return clamp(a, 0.0, 1.0); }
vec3 saturate(vec3 a) { return clamp(a, vec3(0), vec3(1)); }

vec3 ACESFilm(vec3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;

    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}