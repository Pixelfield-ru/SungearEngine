//
// Created by stuka on 12.09.2026.
//

#pragma once

#include <glad/glad.h>

#include "pixelfield/rhi/types.h"

namespace pf::rhi
{
    // OpenGL needs internalformat for storage, plus format+type for pixel transfers.
    struct GLFormatInfo
    {
        GLenum internal_format = GL_NONE; // sized internal format (e.g. GL_RGBA8)
        GLenum format = GL_NONE;         // pixel format (e.g. GL_RGBA)
        GLenum type = GL_NONE;           // pixel type (e.g. GL_UNSIGNED_BYTE)
    };

    GLFormatInfo toGLFormat(Format format) noexcept;
    Format fromGLInternalFormat(GLenum internal_format) noexcept;

    // EGL_GL_COLORSPACE / WGL_COLORSPACE: only s_rgb vs linear are portable.
    // Returns true when the surface should be created as s_rgb-capable.
    bool isGLSrgbColorSpace(ColorSpace color_space) noexcept;

    GLenum toGLTextureTarget(ImageType image_type) noexcept;

    // OpenGL has no image tiling; always returns GL_NONE / kOptimal.
    GLenum toGLImageTiling(ImageTiling tiling) noexcept;
    ImageTiling fromGLImageTiling(GLenum tiling) noexcept;

    // Primary GL bind/attachment target for image usage flags.
    // If several bits are set, picks one by priority
    // (color > depth/stencil > sampled > storage > transfer dst > transfer src).
    // Transient/input attachment map to GL_NONE.
    GLenum toGLImageUsageTarget(ImageUsage usage) noexcept;
    ImageUsage fromGLImageUsageTarget(GLenum target) noexcept;

    GLenum toGLPrimitiveTopology(PrimitiveTopology topology) noexcept;
    PrimitiveTopology fromGLPrimitiveTopology(GLenum mode) noexcept;

    GLenum toGLPolygonMode(PolygonMode mode) noexcept;
    PolygonMode fromGLPolygonMode(GLenum mode) noexcept;

    // OpenGL splits this into glEnable/glDisable(GL_CULL_FACE) and glCullFace.
    struct GLCullMode
    {
        bool enabled = false;
        GLenum face = GL_BACK;
    };

    GLCullMode toGLCullMode(CullMode mode) noexcept;
    CullMode fromGLCullMode(bool enabled, GLenum face) noexcept;

    GLenum toGLFrontFace(FrontFace face) noexcept;
    FrontFace fromGLFrontFace(GLenum face) noexcept;

    std::uint8_t toGLSampleCount(SampleCountFlag count) noexcept;
    SampleCountFlag fromGLSampleCount(std::uint8_t count) noexcept;

    // glColorMask(r, g, b, a).
    struct GLColorMask
    {
        bool r = true;
        bool g = true;
        bool b = true;
        bool a = true;
    };

    GLColorMask toGLColorMask(ColorComponent components) noexcept;
    ColorComponent fromGLColorMask(bool r, bool g, bool b, bool a) noexcept;

    GLenum toGLBlendFactor(BlendFactor factor) noexcept;
    BlendFactor fromGLBlendFactor(GLenum factor) noexcept;

    GLenum toGLBlendOp(BlendOp op) noexcept;
    BlendOp fromGLBlendOp(GLenum op) noexcept;

    GLenum toGLLogicOp(LogicOp op) noexcept;
    LogicOp fromGLLogicOp(GLenum op) noexcept;

    // kLoad: keep contents. kClear: glClear. kDontCare/kNone: glInvalidateFramebuffer.
    struct GLAttachmentLoadOp
    {
        bool clear = false;
        bool invalidate = false;
    };

    // kStore: keep contents. kDontCare/kNone: glInvalidateFramebuffer.
    struct GLAttachmentStoreOp
    {
        bool invalidate = false;
    };

    GLAttachmentLoadOp toGLAttachmentLoadOp(AttachmentLoadOp op) noexcept;
    AttachmentLoadOp fromGLAttachmentLoadOp(GLAttachmentLoadOp op) noexcept;

    GLAttachmentStoreOp toGLAttachmentStoreOp(AttachmentStoreOp op) noexcept;
    AttachmentStoreOp fromGLAttachmentStoreOp(GLAttachmentStoreOp op) noexcept;

    // Which parts of a depth/stencil (or color) texture the view/attachment uses.
    // color is true for ordinary color textures; depth/stencil select D/S planes.
    struct GLImageAspect
    {
        bool color = false;
        bool depth = false;
        bool stencil = false;
    };

    GLImageAspect toGLImageAspect(ImageAspect aspect) noexcept;
    ImageAspect fromGLImageAspect(GLImageAspect aspect) noexcept;

    // identity_value: GL_RED / GL_GREEN / GL_BLUE / GL_ALPHA for that channel
    // when swizzle is kIdentity.
    GLenum toGLComponentSwizzle(ComponentSwizzle swizzle, GLenum identity_value) noexcept;
    ComponentSwizzle fromGLComponentSwizzle(GLenum swizzle, GLenum identity_value) noexcept;

    struct GLComponentMapping
    {
        GLenum r = GL_RED;
        GLenum g = GL_GREEN;
        GLenum b = GL_BLUE;
        GLenum a = GL_ALPHA;
    };

    GLComponentMapping toGLComponentMapping(const ComponentMapping& mapping) noexcept;
    ComponentMapping fromGLComponentMapping(const GLComponentMapping& mapping) noexcept;

    // Primary glBindBuffer target for the given usage flags.
    // If several bits are set, picks one by priority (vertex > index > uniform > ...).
    GLenum toGLBufferTarget(BufferType type) noexcept;
    BufferType fromGLBufferTarget(GLenum target) noexcept;

    // glDrawElements / glDrawElementsInstanced index type.
    GLenum toGLIndexType(IndexType type) noexcept;
    IndexType fromGLIndexType(GLenum type) noexcept;

    // OpenGL bind target / object kind used to emulate a Vulkan descriptor type.
    // Returns GL_NONE when there is no meaningful single target (e.g. input attachment,
    // standalone sampler — sampler objects have no glBindBuffer-style target).
    GLenum toGLDescriptorTarget(DescriptorType type) noexcept;
    DescriptorType fromGLDescriptorTarget(GLenum target) noexcept;

    // glCreateShader type. If several stage bits are set, picks one by priority
    // (vertex > fragment > geometry > tess control > tess eval > compute).
    GLenum toGLShaderType(ShaderStageType stage) noexcept;
    ShaderStageType fromGLShaderType(GLenum type) noexcept;

    // OpenGL has a single implicit queue (the context); no queue-family targets exist.
    // Always returns GL_NONE. fromGLQueueFamily always returns kGraphics.
    GLenum toGLQueueFamily(QueueFamilyType type) noexcept;
    QueueFamilyType fromGLQueueFamily(GLenum type) noexcept;

    // OpenGL has no pipeline bind points (glUseProgram is unified).
    // Always returns GL_NONE. fromGLPipelineBindPoint always returns kGraphics.
    GLenum toGLPipelineBindPoint(PipelineBindPoint bind_point) noexcept;
    PipelineBindPoint fromGLPipelineBindPoint(GLenum bind_point) noexcept;

    // GL_TEXTURE_MAG_FILTER / standalone filter component.
    GLenum toGLFilter(FilterType filter) noexcept;
    FilterType fromGLFilter(GLenum filter) noexcept;

    // Mip filter component only (GL_NEAREST / GL_LINEAR). Prefer toGLMinFilter for MIN_FILTER.
    GLenum toGLMipmapMode(MipmapMode mode) noexcept;
    MipmapMode fromGLMipmapMode(GLenum mode) noexcept;

    // GL_TEXTURE_MIN_FILTER value from min filter + mip mode.
    GLenum toGLMinFilter(FilterType min_filter, MipmapMode mipmap_mode) noexcept;

    // GL_TEXTURE_WRAP_S/T/R.
    GLenum toGLRepeatMode(RepeatMode mode) noexcept;
    RepeatMode fromGLRepeatMode(GLenum mode) noexcept;

    // GL_TEXTURE_COMPARE_FUNC / glDepthFunc.
    GLenum toGLCompareOp(CompareOp op) noexcept;
    CompareOp fromGLCompareOp(GLenum op) noexcept;
}
