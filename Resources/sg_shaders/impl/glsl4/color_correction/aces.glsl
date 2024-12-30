float saturate(const in float a) { return clamp(a, 0.0, 1.0); }
vec2 saturate(const in vec2 a) { return clamp(a, vec2(0), vec2(1)); }
vec3 saturate(const in vec3 a) { return clamp(a, vec3(0), vec3(1)); }

vec3 ACESFilm(const in vec3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;

    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

vec3 ACESTonemap(vec3 color, float exposure)
{
    // Применяем экспозицию
    color *= exposure;

    // Применяем ACES Filmic тонмаппинг
    color = ACESFilm(color);

    // Слегка увеличиваем насыщенность для сочности
    float saturationBoost = 1.2; // Множитель насыщенности
    vec3 luma = vec3(0.2126, 0.7152, 0.0722);
    float luminance = dot(color, luma);
    color = mix(vec3(luminance), color, saturationBoost);

    return color;
}