#ifndef UNIFORM_BUFS_DECL_GLSL
#define UNIFORM_BUFS_DECL_GLSL

#include "defines.glsl"

/*layout(std140) uniform ObjectTransformationData
{
    mat4 objectModelMatrix;
    vec3 objectPosition;
    vec3 objectRotation;
    vec3 objectScale;
};*/

uniform mat4 objectModelMatrix;
uniform vec3 objectPosition;
uniform vec3 objectRotation;
uniform vec3 objectScale;

/*layout(std140) uniform MaterialData
{
    vec4 materialDiffuseCol;
    vec4 materialSpecularCol;
    vec4 materialAmbientCol;
    vec4 materialEmissionCol;
    vec4 materialTransparentCol;
    float materialShininess;
    float materialMetallicFactor;
    float materialRoughnessFactor;
};*/

uniform vec4 materialDiffuseCol;
uniform vec4 materialSpecularCol;
uniform vec4 materialAmbientCol;
uniform vec4 materialEmissionCol;
uniform vec4 materialTransparentCol;
uniform float materialShininess;
uniform float materialMetallicFactor;
uniform float materialRoughnessFactor;

struct FrameBuffer
{
    //int index;

    int depthAttachmentsCount;
    int depthStencilAttachmentsCount;
    int colorAttachmentsCount;
    int renderAttachmentsCount;

    sampler2D depthAttachments[1];
    sampler2D depthStencilAttachments[1];
    sampler2D colorAttachments[3];
    sampler2D renderAttachments[1];
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

#endif // UNIFORM_BUFS_DECL_GLSL