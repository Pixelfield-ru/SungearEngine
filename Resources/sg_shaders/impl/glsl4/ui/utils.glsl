// FIRST ROW
#define LEFT_TOP_SLICE      1
#define TOP_CENTER_SLICE    2
#define RIGHT_TOP_SLICE     3

// SECOND ROW
#define LEFT_CENTER_SLICE   4
#define CENTER_SLICE        5
#define RIGHT_CENTER_SLICE  6

// THIRD ROW
#define LEFT_BOTTOM_SLICE   7
#define BOTTOM_CENTER_SLICE 8
#define RIGHT_BOTTOM_SLICE  9

// HERE WE ASSUME THAT THE CENTRAL SLICE HAS VERTICES
// WITH COORDINATES FROM -1 TO 1 (I.E. THE MAGNIFICATION IN ALL DIRECTIONS IS THE SAME)
vec3 calc9SliceVertexPos(in vec3 vertexPos, in int vertexSlice, in vec2 elementSize)
{
    if(vertexSlice == CENTER_SLICE)
    {
        return vec3(vertexPos.xy * (elementSize / 2.0), 0.0);
    }
    else if(vertexSlice == LEFT_CENTER_SLICE)
    {
        return vec3(vertexPos.x - elementSize.x / 2.0, vertexPos.y * (elementSize.y / 2.0), 0.0);
    }
    else if(vertexSlice == RIGHT_CENTER_SLICE)
    {
        return vec3(vertexPos.x + elementSize.x / 2.0, vertexPos.y * (elementSize.y / 2.0), 0.0);
    }
    else if(vertexSlice == BOTTOM_CENTER_SLICE)
    {
        return vec3(vertexPos.x * (elementSize.x / 2.0), vertexPos.y + elementSize.y / 2.0, 0.0);
    }
    else if(vertexSlice == TOP_CENTER_SLICE)
    {
        return vec3(vertexPos.x * (elementSize.x / 2.0), vertexPos.y - elementSize.y / 2.0, 0.0);
    }

    return vec3(0, 0, 0);
}