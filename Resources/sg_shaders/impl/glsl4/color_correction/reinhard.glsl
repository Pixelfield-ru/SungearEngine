vec3 reinhard2(vec3 x)
{
    const float L_white = 4.0;

    return (x * (1.0 + x / (L_white * L_white))) / (1.0 + x);
}

float reinhard2(float x)
{
    const float L_white = 4.0;

    return (x * (1.0 + x / (L_white * L_white))) / (1.0 + x);
}

vec3 reinhard(vec3 x)
{
    return x / (1.0 + x);
}

float reinhard(float x)
{
    return x / (1.0 + x);
}