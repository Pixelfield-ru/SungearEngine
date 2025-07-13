float ShadowMapPCF(sampler2D shadowMap, vec3 projCoord, vec2 resolution, vec2 pixelSize, int filterSize, float bias)
{
    float shadow = 0.0f;
    vec2 grad = fract(projCoord.xy * resolution + 0.5f);

    for (int i = -filterSize; i <= filterSize; i++)
    {
        for (int j = -filterSize; j <= filterSize; j++)
        {
            vec4 tmp = textureGather(shadowMap, projCoord.xy + vec2(i, j) * pixelSize);
            tmp.x = tmp.x < projCoord.z - bias ? 0.0f : 1.0f;
            tmp.y = tmp.y < projCoord.z - bias ? 0.0f : 1.0f;
            tmp.z = tmp.z < projCoord.z - bias ? 0.0f : 1.0f;
            tmp.w = tmp.w < projCoord.z - bias ? 0.0f : 1.0f;
            shadow += mix(mix(tmp.w, tmp.z, grad.x), mix(tmp.x, tmp.y, grad.x), grad.y);
        }
    }

    return shadow / float((2 * filterSize + 1) * (2 * filterSize + 1));
}