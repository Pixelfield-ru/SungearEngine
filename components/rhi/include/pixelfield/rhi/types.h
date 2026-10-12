//
// Created by stuka on 09.09.2026.
//

#pragma once

#include <cstdint>
#include <vector>
#include <type_traits>
#include <functional>

namespace pf::rhi
{
    enum class QueueFamilyType
    {
        kGraphics = 0,
        kCompute,
        kTransfer,
        kSparseBinding,
        kVideoEncode,
        kVideoDecode,
        kOpticalFlow,
        kPresent,
        kCount
    };

    // vkCmdBindPipeline / vkCmdBindDescriptorSets bind point (VkPipelineBindPoint).
    enum class PipelineBindPoint : std::uint8_t
    {
        kGraphics = 0,
        kCompute,
        kRayTracing // VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR / NV
    };

    // Combinable shader stage flags (maps to VkShaderStageFlags).
    // OpenGL maps a single stage to GL_*_SHADER via toGLShaderType.
    enum class ShaderStageType : std::uint32_t
    {
        kUnknown = 0,
        kVertex = 1 << 0,
        kFragment = 1 << 1,
        kCompute = 1 << 2,
        kTessellationControl = 1 << 3,
        kTessellationEval = 1 << 4,
        kGeometry = 1 << 5,

        kAllGraphics = kVertex | kFragment | kTessellationControl | kTessellationEval | kGeometry,
        kAll = kAllGraphics | kCompute
    };

    constexpr ShaderStageType operator|(ShaderStageType a, ShaderStageType b) noexcept
    {
        return static_cast<ShaderStageType>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
    }

    constexpr ShaderStageType operator&(ShaderStageType a, ShaderStageType b) noexcept
    {
        return static_cast<ShaderStageType>(static_cast<std::uint32_t>(a) & static_cast<std::uint32_t>(b));
    }

    constexpr ShaderStageType& operator|=(ShaderStageType& a, ShaderStageType b) noexcept
    {
        a = a | b;
        return a;
    }

    constexpr ShaderStageType& operator&=(ShaderStageType& a, ShaderStageType b) noexcept
    {
        a = a & b;
        return a;
    }

    enum class DeviceType
    {
        kOther = 0,
        kIntegratedGpu,
        kDiscreteGpu,
        kVirtualGpu,
        kCpu
    };

    // API-agnostic texture/buffer image formats.
    // Map to VkFormat / GL internalformat(+format+type) in backend helpers.
    enum class Format : std::uint16_t
    {
        kUnknown = 0,

        // 8-bit UNORM
        kR8Unorm,
        kRg8Unorm,
        kRgb8Unorm,
        kRgba8Unorm,
        kBgra8Unorm,

        // 8-bit s_rgb
        kRgba8Srgb,
        kBgra8Srgb,

        // 8-bit SNORM
        kR8Snorm,
        kRg8Snorm,
        kRgba8Snorm,

        // 8-bit uint/sint
        kR8Uint,
        kRg8Uint,
        kRgba8Uint,
        kR8Sint,
        kRg8Sint,
        kRgba8Sint,

        // 16-bit
        kR16Unorm,
        kRg16Unorm,
        kRgba16Unorm,
        kR16Snorm,
        kRg16Snorm,
        kRgba16Snorm,
        kR16Uint,
        kRg16Uint,
        kRgba16Uint,
        kR16Sint,
        kRg16Sint,
        kRgba16Sint,
        kR16Float,
        kRg16Float,
        kRgba16Float,

        // 32-bit
        kR32Uint,
        kRg32Uint,
        kRgba32Uint,
        kR32Sint,
        kRg32Sint,
        kRgba32Sint,
        kR32Float,
        kRg32Float,
        kRgb32Float,
        kRgba32Float,

        // packed
        kR10g10b10a2Unorm,
        kB10g10r10a2Unorm,
        kR11g11b10Float,

        // depth / stencil
        kD16Unorm,
        kD24UnormS8Uint,
        kD32Float,
        kD32FloatS8Uint,
        kS8Uint,

        // block compression (Desktop / Vulkan BC == DXT/BPTC)
        kBc1RgbUnorm,
        kBc1RgbSrgb,
        kBc1RgbaUnorm,
        kBc1RgbaSrgb,
        kBc2Unorm,
        kBc2Srgb,
        kBc3Unorm,
        kBc3Srgb,
        kBc4Unorm,
        kBc4Snorm,
        kBc5Unorm,
        kBc5Snorm,
        kBc6hUfloat,
        kBc6hSfloat,
        kBc7Unorm,
        kBc7Srgb,

        kCount
    };

    // Uncompressed texel size in bytes. Returns 0 for unknown / block-compressed formats
    // (BC1–BC7 are block-based; use a block-size helper for those).
    constexpr std::uint32_t formatBytesPerPixel(Format format) noexcept
    {
        switch(format)
        {
            case Format::kR8Unorm:
            case Format::kR8Snorm:
            case Format::kR8Uint:
            case Format::kR8Sint:
            case Format::kS8Uint:
                return 1;

            case Format::kRg8Unorm:
            case Format::kRg8Snorm:
            case Format::kRg8Uint:
            case Format::kRg8Sint:
            case Format::kR16Unorm:
            case Format::kR16Snorm:
            case Format::kR16Uint:
            case Format::kR16Sint:
            case Format::kR16Float:
            case Format::kD16Unorm:
                return 2;

            case Format::kRgb8Unorm:
                return 3;

            case Format::kRgba8Unorm:
            case Format::kBgra8Unorm:
            case Format::kRgba8Srgb:
            case Format::kBgra8Srgb:
            case Format::kRgba8Snorm:
            case Format::kRgba8Uint:
            case Format::kRgba8Sint:
            case Format::kRg16Unorm:
            case Format::kRg16Snorm:
            case Format::kRg16Uint:
            case Format::kRg16Sint:
            case Format::kRg16Float:
            case Format::kR32Uint:
            case Format::kR32Sint:
            case Format::kR32Float:
            case Format::kR10g10b10a2Unorm:
            case Format::kB10g10r10a2Unorm:
            case Format::kR11g11b10Float:
            case Format::kD24UnormS8Uint:
            case Format::kD32Float:
                return 4;

            case Format::kRgba16Unorm:
            case Format::kRgba16Snorm:
            case Format::kRgba16Uint:
            case Format::kRgba16Sint:
            case Format::kRgba16Float:
            case Format::kRg32Uint:
            case Format::kRg32Sint:
            case Format::kRg32Float:
            case Format::kD32FloatS8Uint: // 4 + 4 packed
                return 8;

            case Format::kRgb32Float:
                return 12;

            case Format::kRgba32Uint:
            case Format::kRgba32Sint:
            case Format::kRgba32Float:
                return 16;

            // Block-compressed: not a fixed bytes-per-pixel value.
            case Format::kBc1RgbUnorm:
            case Format::kBc1RgbSrgb:
            case Format::kBc1RgbaUnorm:
            case Format::kBc1RgbaSrgb:
            case Format::kBc2Unorm:
            case Format::kBc2Srgb:
            case Format::kBc3Unorm:
            case Format::kBc3Srgb:
            case Format::kBc4Unorm:
            case Format::kBc4Snorm:
            case Format::kBc5Unorm:
            case Format::kBc5Snorm:
            case Format::kBc6hUfloat:
            case Format::kBc6hSfloat:
            case Format::kBc7Unorm:
            case Format::kBc7Srgb:
            case Format::kUnknown:
            case Format::kCount:
            default:
                return 0;
        }
    }

    // Presentation / surface color space (swapchain).
    // Maps to VkColorSpaceKHR; OpenGL only meaningfully distinguishes s_rgb vs linear.
    enum class ColorSpace : std::uint8_t
    {
        kUnknown = 0,
        kSrgbNonlinear,
        kDisplayP3Nonlinear,
        kExtendedSrgbLinear,
        kDisplayP3Linear,
        kDciP3Nonlinear,
        kBt709Linear,
        kBt709Nonlinear,
        kBt2020Linear,
        kHdr10St2084,
        kDolbyVision,
        kHdr10Hlg,
        kAdobeRgbLinear,
        kAdobeRgbNonlinear,
        kPassThrough,
        kExtendedSrgbNonlinear,
        kCount
    };

    // Texture / image dimensionality.
    // Maps to VkImageType (+ cube/array via view type / create flags); GL texture target.
    enum class ImageType : std::uint8_t
    {
        kUnknown = 0,
        k1D,
        k2D,
        k3D,
        kCube,
        k1DArray,
        k2DArray,
        kCubeArray,
        kCount
    };

    // VkImageTiling. OpenGL has no tiling concept.
    enum class ImageTiling : std::uint8_t
    {
        kOptimal = 0, // GPU-optimal layout (default)
        kLinear       // row-major, may be host-visible; rare
    };

    // Combinable image usage flags (maps to VkImageUsageFlags).
    // OpenGL maps to a primary bind/attachment target via toGLImageUsageTarget.
    enum class ImageUsage : std::uint16_t
    {
        kTransferSrc = 1 << 0,
        kTransferDst = 1 << 1,
        kSampled = 1 << 2,
        kStorage = 1 << 3,
        kColorAttachment = 1 << 4,
        kDepthStencilAttachment = 1 << 5,
        kTransientAttachment = 1 << 6,
        kInputAttachment = 1 << 7
    };

    constexpr ImageUsage operator|(ImageUsage a, ImageUsage b) noexcept
    {
        return static_cast<ImageUsage>(static_cast<std::uint16_t>(a) | static_cast<std::uint16_t>(b));
    }

    constexpr ImageUsage operator&(ImageUsage a, ImageUsage b) noexcept
    {
        return static_cast<ImageUsage>(static_cast<std::uint16_t>(a) & static_cast<std::uint16_t>(b));
    }

    constexpr ImageUsage& operator|=(ImageUsage& a, ImageUsage b) noexcept
    {
        a = a | b;
        return a;
    }

    constexpr ImageUsage& operator&=(ImageUsage& a, ImageUsage b) noexcept
    {
        a = a & b;
        return a;
    }

    // How vertices are assembled into primitives.
    enum class PrimitiveTopology : std::uint8_t
    {
        kUnknown = 0,
        kPointList,
        kLineList,
        kLineStrip,
        kLineLoop,
        kTriangleList,
        kTriangleStrip,
        kTriangleFan,
        kLineListWithAdjacency,
        kLineStripWithAdjacency,
        kTriangleListWithAdjacency,
        kTriangleStripWithAdjacency,
        kPatchList,
        kCount
    };

    // Rasterizer polygon fill mode. Maps to VkPolygonMode / glPolygonMode mode.
    enum class PolygonMode : std::uint8_t
    {
        kFill = 0,
        kLine,
        kPoint,
        kCount
    };

    // Which faces are discarded. Maps to VkCullModeFlags / glCullFace + GL_CULL_FACE.
    enum class CullMode : std::uint8_t
    {
        kNone = 0,
        kFront,
        kBack,
        kFrontAndBack,
        kCount
    };

    // Winding that is considered the front face.
    enum class FrontFace : std::uint8_t
    {
        kCounterClockwise = 0,
        kClockwise,
        kCount
    };

    // Bits of VkColorComponentFlags / glColorMask channels.
    enum class ColorComponent : std::uint8_t
    {
        kR = 1 << 0,
        kG = 1 << 1,
        kB = 1 << 2,
        kA = 1 << 3,
        kRgba = 0x0F
    };

    constexpr ColorComponent operator|(ColorComponent a, ColorComponent b) noexcept
    {
        return static_cast<ColorComponent>(static_cast<std::uint8_t>(a) | static_cast<std::uint8_t>(b));
    }

    constexpr ColorComponent operator&(ColorComponent a, ColorComponent b) noexcept
    {
        return static_cast<ColorComponent>(static_cast<std::uint8_t>(a) & static_cast<std::uint8_t>(b));
    }

    // Blend factors. Same values are used for color and alpha.
    enum class BlendFactor : std::uint8_t
    {
        kZero = 0,
        kOne,
        kSrcColor,
        kOneMinusSrcColor,
        kDstColor,
        kOneMinusDstColor,
        kSrcAlpha,
        kOneMinusSrcAlpha,
        kDstAlpha,
        kOneMinusDstAlpha,
        kConstantColor,
        kOneMinusConstantColor,
        kConstantAlpha,
        kOneMinusConstantAlpha,
        kSrcAlphaSaturate,
        kSrc1Color,
        kOneMinusSrc1Color,
        kSrc1Alpha,
        kOneMinusSrc1Alpha,
        kCount
    };

    enum class BlendOp : std::uint8_t
    {
        kAdd = 0,
        kSubtract,
        kReverseSubtract,
        kMin,
        kMax,
        kCount
    };

    // Bitwise framebuffer operation. Used only when logic op is enabled.
    enum class LogicOp : std::uint8_t
    {
        kClear = 0,
        kAnd,
        kAndReverse,
        kCopy,
        kAndInverted,
        kNoOp,
        kXor,
        kOr,
        kNor,
        kEquivalent,
        kInvert,
        kOrReverse,
        kCopyInverted,
        kOrInverted,
        kNand,
        kSet,
        kCount
    };

    // Shared by color and stencil loadOp.
    enum class AttachmentLoadOp : std::uint8_t
    {
        kLoad = 0,
        kClear,
        kDontCare,
        kNone,
        kCount
    };

    // Shared by color and stencil storeOp.
    enum class AttachmentStoreOp : std::uint8_t
    {
        kStore = 0,
        kDontCare,
        kNone,
        kCount
    };

    // Shared by initialLayout and finalLayout.
    // OpenGL has no image layouts.
    enum class ImageLayout : std::uint8_t
    {
        kUndefined = 0,
        kGeneral,
        kColorAttachment,
        kDepthStencilAttachment,
        kDepthStencilReadOnly,
        kShaderReadOnly,
        kTransferSrc,
        kTransferDst,
        kPresent,
        kDepthAttachment,
        kDepthReadOnly,
        kStencilAttachment,
        kStencilReadOnly,
        kCount
    };

    // Which planes/parts of an image a view (or barrier/clear) refers to.
    // Maps to VkImageAspectFlags. OpenGL has no direct equivalent; used as a hint
    // for depth/stencil texture views and FBO attachment selection.
    enum class ImageAspect : std::uint8_t
    {
        kColor = 1 << 0,
        kDepth = 1 << 1,
        kStencil = 1 << 2,
        kMetadata = 1 << 3,
        kPlane0 = 1 << 4,
        kPlane1 = 1 << 5,
        kPlane2 = 1 << 6,
        kDepthStencil = (1 << 1) | (1 << 2)
    };

    constexpr ImageAspect operator|(ImageAspect a, ImageAspect b) noexcept
    {
        return static_cast<ImageAspect>(static_cast<std::uint8_t>(a) | static_cast<std::uint8_t>(b));
    }

    constexpr ImageAspect operator&(ImageAspect a, ImageAspect b) noexcept
    {
        return static_cast<ImageAspect>(static_cast<std::uint8_t>(a) & static_cast<std::uint8_t>(b));
    }

    // Per-channel remapping for image views. Maps to VkComponentSwizzle /
    // GL_TEXTURE_SWIZZLE_{R,G,B,A}.
    enum class ComponentSwizzle : std::uint8_t
    {
        kIdentity = 0,
        kZero,
        kOne,
        kR,
        kG,
        kB,
        kA,
        kCount
    };

    struct ComponentMapping final
    {
        ComponentSwizzle r = ComponentSwizzle::kIdentity;
        ComponentSwizzle g = ComponentSwizzle::kIdentity;
        ComponentSwizzle b = ComponentSwizzle::kIdentity;
        ComponentSwizzle a = ComponentSwizzle::kIdentity;
    };

    enum class SampleCountFlag : std::uint8_t
    {
        k1Sample = 1,
        k2Sample = 2,
        k4Sample = 4,
        k8Sample = 8,
        k16Sample = 16,
        k32Sample = 32,
        k64Sample = 64
    };

    enum class InputRate : std::uint8_t
    {
        kPerVertex = 0,
        kPerInstance
    };

    // Index buffer element size (vkCmdBindIndexBuffer / glDrawElements type).
    enum class IndexType : std::uint8_t
    {
        kUint16 = 0,
        kUint32,
        kUint8 // Vulkan: VK_INDEX_TYPE_UINT8_EXT / KHR; OpenGL: GL_UNSIGNED_BYTE
    };

    // Resource binding kind (VkDescriptorType).
    // OpenGL has no descriptors; maps to the bind target / concept used when emulating.
    enum class DescriptorType : std::uint8_t
    {
        kSampler = 0,
        kCombinedImageSampler,
        kSampledImage,
        kStorageImage,
        kUniformTexelBuffer,
        kStorageTexelBuffer,
        kUniformBuffer,
        kStorageBuffer,
        kUniformBufferDynamic,
        kStorageBufferDynamic,
        kInputAttachment // Vulkan subpass input; no OpenGL equivalent
    };

    // How a buffer may be used. Combinable flags (maps to VkBufferUsageFlags).
    // OpenGL maps to a primary glBindBuffer target via toGLBufferTarget.
    enum class BufferType : std::uint16_t
    {
        kVertex = 1 << 0,
        kIndex = 1 << 1,
        kUniform = 1 << 2,
        kStorage = 1 << 3,
        kIndirect = 1 << 4,
        kTransferSrc = 1 << 5,
        kTransferDst = 1 << 6
    };

    constexpr BufferType operator|(BufferType a, BufferType b) noexcept
    {
        return static_cast<BufferType>(static_cast<std::uint16_t>(a) | static_cast<std::uint16_t>(b));
    }

    constexpr BufferType operator&(BufferType a, BufferType b) noexcept
    {
        return static_cast<BufferType>(static_cast<std::uint16_t>(a) & static_cast<std::uint16_t>(b));
    }

    constexpr BufferType& operator|=(BufferType& a, BufferType b) noexcept
    {
        a = a | b;
        return a;
    }

    // Sampler mag/min filter (VkFilter / GL_TEXTURE_MAG_FILTER, min without mip).
    enum class FilterType : std::uint8_t
    {
        kNearest = 0,
        kLinear
    };

    // Sampler mipmap filter (VkSamplerMipmapMode).
    // OpenGL has no standalone mip mode; combine with FilterType via toGLMinFilter.
    enum class MipmapMode : std::uint8_t
    {
        kNearest = 0,
        kLinear
    };

    // Sampler addressing / wrap (VkSamplerAddressMode / GL_TEXTURE_WRAP_*).
    enum class RepeatMode : std::uint8_t
    {
        kRepeat = 0,
        kMirroredRepeat,
        kClampToEdge,
        kClampToBorder,
        kMirrorClampToEdge // GL: GL_MIRROR_CLAMP_TO_EDGE
    };

    // Comparison op (VkCompareOp / GL compare func). Sampler shadows + depth tests.
    enum class CompareOp : std::uint8_t
    {
        kNever = 0,
        kLess,
        kEqual,
        kLessOrEqual,
        kGreater,
        kNotEqual,
        kGreaterOrEqual,
        kAlways
    };

    struct AttachmentBlendInfo final
    {
        bool enable_blend = false;

        ColorComponent color_write_mask = ColorComponent::kRgba;
        BlendFactor src_color_blend_factor = BlendFactor::kSrcAlpha;
        BlendFactor dst_color_blend_factor = BlendFactor::kOneMinusSrcAlpha;
        BlendOp color_blend_op = BlendOp::kAdd;
        BlendFactor src_alpha_blend_factor = BlendFactor::kOne;
        BlendFactor dst_alpha_blend_factor = BlendFactor::kZero;
        BlendOp alpha_blend_op = BlendOp::kAdd;
    };

    struct AttachmentCreateInfo
    {
        Format format = Format::kUnknown;
        SampleCountFlag storage_sample_count = SampleCountFlag::k1Sample;
        AttachmentLoadOp load_op = AttachmentLoadOp::kClear;
        AttachmentStoreOp store_op = AttachmentStoreOp::kStore;
        AttachmentLoadOp stencil_load_op = AttachmentLoadOp::kDontCare;
        AttachmentStoreOp stencil_store_op = AttachmentStoreOp::kDontCare;
        ImageLayout initial_layout = ImageLayout::kUndefined;
        ImageLayout final_layout = ImageLayout::kPresent;
    };

    struct ColorAttachmentCreateInfo final : AttachmentCreateInfo
    {

    };

    struct DepthStencilAttachmentCreateInfo final : AttachmentCreateInfo
    {

    };

    struct VertexAttributeDescription final
    {
        std::uint16_t location {};
        Format format = Format::kUnknown;
        std::uint32_t offset {};
    };

    struct VertexBindingDescription final
    {
        std::uint16_t binding {};
        std::uint32_t stride {};
        InputRate input_rate = InputRate::kPerVertex;

        std::vector<VertexAttributeDescription> attributes;
    };

    template<typename ScalarT>
    requires(std::is_scalar_v<ScalarT>)
    struct Rect final
    {
        ScalarT x {};
        ScalarT y {};
        ScalarT width {};
        ScalarT height {};
    };

    struct ViewportInfo final
    {
        Rect<float> rect {};
        float min_depth = 0.0f;
        float max_depth = 1.0f;
    };

    struct Defer final
    {
        Defer(std::function<void()> func) noexcept : func_(std::move(func)) {}
        Defer(const Defer&) = delete;
        Defer(Defer&&) noexcept = default;
        ~Defer() noexcept
        {
            func_();
        }

        Defer& operator=(const Defer&) = delete;
        Defer& operator=(Defer&&) = default;

    private:
        std::function<void()> func_;
    };
}

#define rhi_concat_impl(a, b) a##b
#define rhi_concat(a, b) rhi_concat_impl(a, b)
#define rhi_defer(func) pf::rhi::Defer rhi_concat(defer_, __LINE__)(func)
