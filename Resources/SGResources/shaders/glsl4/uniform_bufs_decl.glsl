layout(std140) uniform ObjectTransformationData
{
    mat4 objectModelMatrix;
    vec3 objectPosition;
    vec3 objectRotation;
    vec3 objectScale;
};

layout(std140) uniform MaterialData
{
    vec4 materialDiffuseCol;
    vec4 materialSpecularCol;
    vec4 materialAmbientCol;
    vec4 materialEmissionCol;
    vec4 materialTransparentCol;
    float materialShininess;
    float materialMetallicFactor;
    float materialRoughnessFactor;
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
