#sg_pragma once

vec2 raySphereIntersection(const in vec3 ro, const in vec3 rd, const in float sr)
{
    float b = dot(ro, rd);
    float c = dot(ro, ro) - sr * sr;
    float h = b * b - c;
    if(h < 0.0) return vec2(-1.0, -1.0);
    h = sqrt(h);
    return vec2(-b - h, -b + h);
}
