layout(std140) uniform ObjectMatrices
{
    mat4 objectModelMatrix;
};

layout(std140) uniform ViewMatrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    vec3 viewDirection;
};

layout(std140) uniform ProgramData
{
    vec2 windowSize;
};
