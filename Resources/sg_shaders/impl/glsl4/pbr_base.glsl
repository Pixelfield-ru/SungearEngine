// N - normal
// VD - view dir
float GeometryShlickGGX(const in float NdotVD, const in float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float denom = NdotVD * (1.0 - k) + k;

    return NdotVD / denom;
}

// площадь поверхности, где микроскопические неровности перекрывают друг друга
float GeometrySmith(const in float NdotVD, const in float NdotL, float roughness)
{
    // ggx from view dir
    // насколько освещён фрагмент при виде от камеры
    float ggx2 = GeometryShlickGGX(NdotVD, roughness);
    // ggx from light dir
    // насколько освещён тот же фрагмент при виде от источника света
    float ggx1 = GeometryShlickGGX(NdotL, roughness);

    // если ggx1 = 1, а ggx2 = 0, то это geometry shadowing
    // если ggx1 = 0, а ggx2 = 1, то это geometry obstruction
    return ggx1 * ggx2;
}

/*float GeometrySmith(float NdotVD, float NdotL, float roughness)
{
    float m = saturate(roughness * roughness);

    float m2 = m * m;
    float a = NdotL + clamp(sqrt((NdotL - m2 * NdotL) * NdotL + m2), 0, 1);
    float b = NdotVD + clamp(sqrt((NdotVD - m2 * NdotVD) * NdotVD + m2), 0, 1);

    return 1.0 / a * b;
}*/

// Trowbridge-Reitz GGX (Normal Distribution Function)
float GGXTR(const in vec3 normal, const in vec3 medianVec, const in float roughness)
{
    float roughness4 = pow(roughness, 4.0);

    // косинус между медианным вектором и нормалью фрагмента
    float NdotMV = max(dot(normal, medianVec), 0.0);
    float NdotMV2 = NdotMV * NdotMV;

    // числитель
    float numerator = roughness4;
    // заменатель
    float denominator = (NdotMV2 * (roughness4 - 1.0) + 1.0);
    denominator = PI * denominator * denominator;

    return numerator / denominator;
}

vec3 SchlickFresnel(const in float cosTheta, const in vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}