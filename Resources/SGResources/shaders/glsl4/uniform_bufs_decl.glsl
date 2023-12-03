#ifndef UNIFORM_BUFS_DECL_GLSL
#define UNIFORM_BUFS_DECL_GLSL

#include "defines.glsl"
#include "rendering_structs_decl.glsl"

uniform mat4 objectModelMatrix;
uniform vec3 objectPosition;
uniform vec3 objectRotation;
uniform vec3 objectScale;

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

layout(std140) uniform CameraData
{
    IRenderingComponent camera;
};

layout(std140) uniform ProgramData
{
    vec2 windowSize;
};

#endif // UNIFORM_BUFS_DECL_GLSL