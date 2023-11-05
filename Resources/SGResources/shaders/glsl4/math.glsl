#define PI 3.1415926

float mad(const in float mval, const in float aval, const in float bval)
{
    return mval * aval + bval;
}

vec2 rotate(const in vec2 pos, const in vec2 rotTrig)
{
    return vec2(pos.x * rotTrig.x - pos.y * rotTrig.y, pos.y * rotTrig.x + pos.x * rotTrig.y);
}