uvec2 unpackU32ToU16Vec2(uint value)
{
    uvec2 r;

    uint high = (value >> 16u) & 0xFFFFu;
    uint low = value & 0xFFFFu;

    r.x = high;
    r.y = low;

    return r;
}