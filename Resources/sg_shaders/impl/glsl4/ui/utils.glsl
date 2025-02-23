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
// shit happens
vec3 calc9SliceVertexPos(in vec3 vertexPos, in int vertexSlice, in vec2 elementSize, vec2 borderTotalSize)
{
    vec2 cornerCoeff = vec2(1.0);
    vec2 borderCoeff = vec2(1.0);

    vec2 borderToElementSizeDif = vec2(borderTotalSize.x * 2.0 - elementSize.x,
                                       borderTotalSize.y * 2.0 - elementSize.y);

    // ========================================================
    // doing some magic to guarantee element size if borders are bigger than size of element
    if(borderTotalSize.x * 2.0 > elementSize.x && borderToElementSizeDif.x >= borderToElementSizeDif.y)
    {
        cornerCoeff.x = elementSize.x / (borderTotalSize.x * 2.0);
        cornerCoeff.y = cornerCoeff.x;

        borderCoeff.x = cornerCoeff.x;
        borderCoeff.y = 1.0 / cornerCoeff.y;
    }
    if(borderTotalSize.y * 2.0 > elementSize.y && borderToElementSizeDif.y > borderToElementSizeDif.x)
    {
        cornerCoeff.y = elementSize.y / (borderTotalSize.y * 2.0);
        cornerCoeff.x = cornerCoeff.y;

        borderCoeff.x = 1.0 / cornerCoeff.y;
        borderCoeff.y = cornerCoeff.y;
    }
    // ========================================================

    vec2 finalElementSize = elementSize;
    finalElementSize.x = max(elementSize.x - borderTotalSize.x * 2.0 / borderCoeff.x, 0.0);
    finalElementSize.y = max(elementSize.y - borderTotalSize.y * 2.0 / borderCoeff.y, 0.0);

    if(vertexSlice == CENTER_SLICE)
    {
        return vec3(vertexPos.xy * (finalElementSize / 2.0), 0.0);
    }
    else if(vertexSlice == LEFT_CENTER_SLICE)
    {
        return vec3(vertexPos.x * borderCoeff.x - finalElementSize.x / 2.0,
                    vertexPos.y * (finalElementSize.y / 2.0), 0.0);
    }
    else if(vertexSlice == RIGHT_CENTER_SLICE)
    {
        return vec3(vertexPos.x * borderCoeff.x + finalElementSize.x / 2.0,
                    vertexPos.y * (finalElementSize.y / 2.0), 0.0);
    }
    else if(vertexSlice == BOTTOM_CENTER_SLICE)
    {
        return vec3(vertexPos.x * (finalElementSize.x / 2.0),
                    vertexPos.y * borderCoeff.y + finalElementSize.y / 2.0, 0.0);
    }
    else if(vertexSlice == TOP_CENTER_SLICE)
    {
        return vec3(vertexPos.x * (finalElementSize.x / 2.0),
                    vertexPos.y * borderCoeff.y - finalElementSize.y / 2.0, 0.0);
    }
    else if(vertexSlice == LEFT_TOP_SLICE)
    {
        vec2 finalVertexPos = vec2(vertexPos.x * cornerCoeff.x - finalElementSize.x / 2.0,
                                   vertexPos.y * cornerCoeff.y - finalElementSize.y / 2.0);

        return vec3(finalVertexPos, 0.0);
    }
    else if(vertexSlice == RIGHT_TOP_SLICE)
    {
        vec2 finalVertexPos = vec2(vertexPos.x * cornerCoeff.x + finalElementSize.x / 2.0,
                                   vertexPos.y * cornerCoeff.y - finalElementSize.y / 2.0);

        return vec3(finalVertexPos, 0.0);
    }
    else if(vertexSlice == LEFT_BOTTOM_SLICE)
    {
        vec2 finalVertexPos = vec2(vertexPos.x * cornerCoeff.x - finalElementSize.x / 2.0,
                                   vertexPos.y * cornerCoeff.y + finalElementSize.y / 2.0);

        return vec3(finalVertexPos, 0.0);
    }
    else if(vertexSlice == RIGHT_BOTTOM_SLICE)
    {
        vec2 finalVertexPos = vec2(vertexPos.x * cornerCoeff.x + finalElementSize.x / 2.0,
                                   vertexPos.y * cornerCoeff.y + finalElementSize.y / 2.0);

        return vec3(finalVertexPos, 0.0);
    }

    return vec3(0, 0, 0);
}