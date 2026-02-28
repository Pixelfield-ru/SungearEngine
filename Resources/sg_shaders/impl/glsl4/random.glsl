float interleavedGradientNoise(const in vec2 position_screen)
{
    vec3 magic = vec3(0.06711056f, 0.00583715f, 52.9829189f);
    return fract(magic.z * fract(dot(position_screen, magic.xy)));
}

highp float highpRandom(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt = dot(co.xy ,vec2(a,b));
    highp float sn = mod(dt,3.14);
    return fract(sin(sn) * c);
}

float random(vec2 co)
{
    return fract(sin(dot(co.xy,vec2(12.9898,78.233))) * 43758.5453);
}
