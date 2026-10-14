//
// Created by stuka on 12.09.2026.
//

#include "pixelfield/rhi/opengl/opengl_types.h"

#include <utility>

// S3TC/DXT is an extension (not core). Provide enums if the loader headers omit them.
#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT 0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT 0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F
#endif

namespace pf::rhi
{
    GLFormatInfo toGLFormat(Format format) noexcept
    {
        switch(format)
        {
            case Format::kR8Unorm: return { GL_R8, GL_RED, GL_UNSIGNED_BYTE };
            case Format::kRg8Unorm: return { GL_RG8, GL_RG, GL_UNSIGNED_BYTE };
            case Format::kRgb8Unorm: return { GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE };
            case Format::kRgba8Unorm: return { GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE };
            case Format::kBgra8Unorm: return { GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE };

            case Format::kRgba8Srgb: return { GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE };
            case Format::kBgra8Srgb: return { GL_SRGB8_ALPHA8, GL_BGRA, GL_UNSIGNED_BYTE };

            case Format::kR8Snorm: return { GL_R8_SNORM, GL_RED, GL_BYTE };
            case Format::kRg8Snorm: return { GL_RG8_SNORM, GL_RG, GL_BYTE };
            case Format::kRgba8Snorm: return { GL_RGBA8_SNORM, GL_RGBA, GL_BYTE };

            case Format::kR8Uint: return { GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE };
            case Format::kRg8Uint: return { GL_RG8UI, GL_RG_INTEGER, GL_UNSIGNED_BYTE };
            case Format::kRgba8Uint: return { GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE };
            case Format::kR8Sint: return { GL_R8I, GL_RED_INTEGER, GL_BYTE };
            case Format::kRg8Sint: return { GL_RG8I, GL_RG_INTEGER, GL_BYTE };
            case Format::kRgba8Sint: return { GL_RGBA8I, GL_RGBA_INTEGER, GL_BYTE };

            case Format::kR16Unorm: return { GL_R16, GL_RED, GL_UNSIGNED_SHORT };
            case Format::kRg16Unorm: return { GL_RG16, GL_RG, GL_UNSIGNED_SHORT };
            case Format::kRgba16Unorm: return { GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT };
            case Format::kR16Snorm: return { GL_R16_SNORM, GL_RED, GL_SHORT };
            case Format::kRg16Snorm: return { GL_RG16_SNORM, GL_RG, GL_SHORT };
            case Format::kRgba16Snorm: return { GL_RGBA16_SNORM, GL_RGBA, GL_SHORT };
            case Format::kR16Uint: return { GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT };
            case Format::kRg16Uint: return { GL_RG16UI, GL_RG_INTEGER, GL_UNSIGNED_SHORT };
            case Format::kRgba16Uint: return { GL_RGBA16UI, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT };
            case Format::kR16Sint: return { GL_R16I, GL_RED_INTEGER, GL_SHORT };
            case Format::kRg16Sint: return { GL_RG16I, GL_RG_INTEGER, GL_SHORT };
            case Format::kRgba16Sint: return { GL_RGBA16I, GL_RGBA_INTEGER, GL_SHORT };
            case Format::kR16Float: return { GL_R16F, GL_RED, GL_HALF_FLOAT };
            case Format::kRg16Float: return { GL_RG16F, GL_RG, GL_HALF_FLOAT };
            case Format::kRgba16Float: return { GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT };

            case Format::kR32Uint: return { GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT };
            case Format::kRg32Uint: return { GL_RG32UI, GL_RG_INTEGER, GL_UNSIGNED_INT };
            case Format::kRgba32Uint: return { GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT };
            case Format::kR32Sint: return { GL_R32I, GL_RED_INTEGER, GL_INT };
            case Format::kRg32Sint: return { GL_RG32I, GL_RG_INTEGER, GL_INT };
            case Format::kRgba32Sint: return { GL_RGBA32I, GL_RGBA_INTEGER, GL_INT };
            case Format::kR32Float: return { GL_R32F, GL_RED, GL_FLOAT };
            case Format::kRg32Float: return { GL_RG32F, GL_RG, GL_FLOAT };
            case Format::kRgb32Float: return { GL_RGB32F, GL_RGB, GL_FLOAT };
            case Format::kRgba32Float: return { GL_RGBA32F, GL_RGBA, GL_FLOAT };

            case Format::kR10g10b10a2Unorm: return { GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV };
            case Format::kB10g10r10a2Unorm: return { GL_RGB10_A2, GL_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV };
            case Format::kR11g11b10Float: return { GL_R11F_G11F_B10F, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV };

            case Format::kD16Unorm: return { GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT };
            case Format::kD24UnormS8Uint: return { GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8 };
            case Format::kD32Float: return { GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT };
            case Format::kD32FloatS8Uint: return { GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV };
            case Format::kS8Uint: return { GL_STENCIL_INDEX8, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE };

            case Format::kBc1RgbUnorm: return { GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GL_RGB, GL_UNSIGNED_BYTE };
            case Format::kBc1RgbSrgb: return { GL_COMPRESSED_SRGB_S3TC_DXT1_EXT, GL_RGB, GL_UNSIGNED_BYTE };
            case Format::kBc1RgbaUnorm: return { GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_RGBA, GL_UNSIGNED_BYTE };
            case Format::kBc1RgbaSrgb: return { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT, GL_RGBA, GL_UNSIGNED_BYTE };
            case Format::kBc2Unorm: return { GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_RGBA, GL_UNSIGNED_BYTE };
            case Format::kBc2Srgb: return { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT, GL_RGBA, GL_UNSIGNED_BYTE };
            case Format::kBc3Unorm: return { GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_RGBA, GL_UNSIGNED_BYTE };
            case Format::kBc3Srgb: return { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT, GL_RGBA, GL_UNSIGNED_BYTE };
            case Format::kBc4Unorm: return { GL_COMPRESSED_RED_RGTC1, GL_RED, GL_UNSIGNED_BYTE };
            case Format::kBc4Snorm: return { GL_COMPRESSED_SIGNED_RED_RGTC1, GL_RED, GL_BYTE };
            case Format::kBc5Unorm: return { GL_COMPRESSED_RG_RGTC2, GL_RG, GL_UNSIGNED_BYTE };
            case Format::kBc5Snorm: return { GL_COMPRESSED_SIGNED_RG_RGTC2, GL_RG, GL_BYTE };
            case Format::kBc6hUfloat: return { GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT, GL_RGB, GL_FLOAT };
            case Format::kBc6hSfloat: return { GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT, GL_RGB, GL_FLOAT };
            case Format::kBc7Unorm: return { GL_COMPRESSED_RGBA_BPTC_UNORM, GL_RGBA, GL_UNSIGNED_BYTE };
            case Format::kBc7Srgb: return { GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM, GL_RGBA, GL_UNSIGNED_BYTE };

            case Format::kUnknown:
            case Format::kCount:
            default:
                return {};
        }
    }

    Format fromGLInternalFormat(GLenum internal_format) noexcept
    {
        switch(internal_format)
        {
            case GL_R8: return Format::kR8Unorm;
            case GL_RG8: return Format::kRg8Unorm;
            case GL_RGB8: return Format::kRgb8Unorm;
            case GL_RGBA8: return Format::kRgba8Unorm;
            case GL_SRGB8_ALPHA8: return Format::kRgba8Srgb;

            case GL_R8_SNORM: return Format::kR8Snorm;
            case GL_RG8_SNORM: return Format::kRg8Snorm;
            case GL_RGBA8_SNORM: return Format::kRgba8Snorm;

            case GL_R8UI: return Format::kR8Uint;
            case GL_RG8UI: return Format::kRg8Uint;
            case GL_RGBA8UI: return Format::kRgba8Uint;
            case GL_R8I: return Format::kR8Sint;
            case GL_RG8I: return Format::kRg8Sint;
            case GL_RGBA8I: return Format::kRgba8Sint;

            case GL_R16: return Format::kR16Unorm;
            case GL_RG16: return Format::kRg16Unorm;
            case GL_RGBA16: return Format::kRgba16Unorm;
            case GL_R16_SNORM: return Format::kR16Snorm;
            case GL_RG16_SNORM: return Format::kRg16Snorm;
            case GL_RGBA16_SNORM: return Format::kRgba16Snorm;
            case GL_R16UI: return Format::kR16Uint;
            case GL_RG16UI: return Format::kRg16Uint;
            case GL_RGBA16UI: return Format::kRgba16Uint;
            case GL_R16I: return Format::kR16Sint;
            case GL_RG16I: return Format::kRg16Sint;
            case GL_RGBA16I: return Format::kRgba16Sint;
            case GL_R16F: return Format::kR16Float;
            case GL_RG16F: return Format::kRg16Float;
            case GL_RGBA16F: return Format::kRgba16Float;

            case GL_R32UI: return Format::kR32Uint;
            case GL_RG32UI: return Format::kRg32Uint;
            case GL_RGBA32UI: return Format::kRgba32Uint;
            case GL_R32I: return Format::kR32Sint;
            case GL_RG32I: return Format::kRg32Sint;
            case GL_RGBA32I: return Format::kRgba32Sint;
            case GL_R32F: return Format::kR32Float;
            case GL_RG32F: return Format::kRg32Float;
            case GL_RGB32F: return Format::kRgb32Float;
            case GL_RGBA32F: return Format::kRgba32Float;

            case GL_RGB10_A2: return Format::kR10g10b10a2Unorm;
            case GL_R11F_G11F_B10F: return Format::kR11g11b10Float;

            case GL_DEPTH_COMPONENT16: return Format::kD16Unorm;
            case GL_DEPTH24_STENCIL8: return Format::kD24UnormS8Uint;
            case GL_DEPTH_COMPONENT32F: return Format::kD32Float;
            case GL_DEPTH32F_STENCIL8: return Format::kD32FloatS8Uint;
            case GL_STENCIL_INDEX8: return Format::kS8Uint;

            case GL_COMPRESSED_RGB_S3TC_DXT1_EXT: return Format::kBc1RgbUnorm;
            case GL_COMPRESSED_SRGB_S3TC_DXT1_EXT: return Format::kBc1RgbSrgb;
            case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: return Format::kBc1RgbaUnorm;
            case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT: return Format::kBc1RgbaSrgb;
            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT: return Format::kBc2Unorm;
            case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT: return Format::kBc2Srgb;
            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: return Format::kBc3Unorm;
            case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT: return Format::kBc3Srgb;
            case GL_COMPRESSED_RED_RGTC1: return Format::kBc4Unorm;
            case GL_COMPRESSED_SIGNED_RED_RGTC1: return Format::kBc4Snorm;
            case GL_COMPRESSED_RG_RGTC2: return Format::kBc5Unorm;
            case GL_COMPRESSED_SIGNED_RG_RGTC2: return Format::kBc5Snorm;
            case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT: return Format::kBc6hUfloat;
            case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT: return Format::kBc6hSfloat;
            case GL_COMPRESSED_RGBA_BPTC_UNORM: return Format::kBc7Unorm;
            case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM: return Format::kBc7Srgb;

            default:
                return Format::kUnknown;
        }
    }

    bool isGLSrgbColorSpace(ColorSpace color_space) noexcept
    {
        switch(color_space)
        {
            case ColorSpace::kSrgbNonlinear:
            case ColorSpace::kExtendedSrgbNonlinear:
            case ColorSpace::kDisplayP3Nonlinear:
            case ColorSpace::kDciP3Nonlinear:
            case ColorSpace::kBt709Nonlinear:
            case ColorSpace::kAdobeRgbNonlinear:
                return true;

            case ColorSpace::kUnknown:
            case ColorSpace::kExtendedSrgbLinear:
            case ColorSpace::kDisplayP3Linear:
            case ColorSpace::kBt709Linear:
            case ColorSpace::kBt2020Linear:
            case ColorSpace::kHdr10St2084:
            case ColorSpace::kDolbyVision:
            case ColorSpace::kHdr10Hlg:
            case ColorSpace::kAdobeRgbLinear:
            case ColorSpace::kPassThrough:
            case ColorSpace::kCount:
            default:
                return false;
        }
    }

    GLenum toGLTextureTarget(ImageType image_type) noexcept
    {
        switch(image_type)
        {
            case ImageType::k1D: return GL_TEXTURE_1D;
            case ImageType::k2D: return GL_TEXTURE_2D;
            case ImageType::k3D: return GL_TEXTURE_3D;
            case ImageType::kCube: return GL_TEXTURE_CUBE_MAP;
            case ImageType::k1DArray: return GL_TEXTURE_1D_ARRAY;
            case ImageType::k2DArray: return GL_TEXTURE_2D_ARRAY;
            case ImageType::kCubeArray: return GL_TEXTURE_CUBE_MAP_ARRAY;

            case ImageType::kUnknown:
            case ImageType::kCount:
            default:
                return GL_TEXTURE_2D;
        }
    }

    GLenum toGLImageTiling(ImageTiling) noexcept
    {
        return GL_NONE;
    }

    ImageTiling fromGLImageTiling(GLenum) noexcept
    {
        return ImageTiling::kOptimal;
    }

    GLenum toGLImageUsageTarget(ImageUsage usage) noexcept
    {
        const auto bits = static_cast<std::uint16_t>(usage);
        if(bits & static_cast<std::uint16_t>(ImageUsage::kColorAttachment)) return GL_COLOR_ATTACHMENT0;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kDepthStencilAttachment)) return GL_DEPTH_STENCIL_ATTACHMENT;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kSampled)) return GL_TEXTURE_2D;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kStorage)) return GL_TEXTURE_2D;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kTransferDst)) return GL_PIXEL_UNPACK_BUFFER;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kTransferSrc)) return GL_PIXEL_PACK_BUFFER;
        return GL_NONE;
    }

    ImageUsage fromGLImageUsageTarget(GLenum target) noexcept
    {
        switch(target)
        {
            case GL_COLOR_ATTACHMENT0: return ImageUsage::kColorAttachment;
            case GL_DEPTH_STENCIL_ATTACHMENT:
            case GL_DEPTH_ATTACHMENT:
            case GL_STENCIL_ATTACHMENT:
                return ImageUsage::kDepthStencilAttachment;
            case GL_PIXEL_UNPACK_BUFFER: return ImageUsage::kTransferDst;
            case GL_PIXEL_PACK_BUFFER: return ImageUsage::kTransferSrc;
            case GL_TEXTURE_1D:
            case GL_TEXTURE_2D:
            case GL_TEXTURE_3D:
            case GL_TEXTURE_CUBE_MAP:
            case GL_TEXTURE_2D_ARRAY:
            case GL_TEXTURE_CUBE_MAP_ARRAY:
                return ImageUsage::kSampled;
            default:
                return static_cast<ImageUsage>(0);
        }
    }

    GLenum toGLPrimitiveTopology(PrimitiveTopology topology) noexcept
    {
        switch(topology)
        {
            case PrimitiveTopology::kPointList: return GL_POINTS;
            case PrimitiveTopology::kLineList: return GL_LINES;
            case PrimitiveTopology::kLineStrip: return GL_LINE_STRIP;
            case PrimitiveTopology::kLineLoop: return GL_LINE_LOOP;
            case PrimitiveTopology::kTriangleList: return GL_TRIANGLES;
            case PrimitiveTopology::kTriangleStrip: return GL_TRIANGLE_STRIP;
            case PrimitiveTopology::kTriangleFan: return GL_TRIANGLE_FAN;
            case PrimitiveTopology::kLineListWithAdjacency: return GL_LINES_ADJACENCY;
            case PrimitiveTopology::kLineStripWithAdjacency: return GL_LINE_STRIP_ADJACENCY;
            case PrimitiveTopology::kTriangleListWithAdjacency: return GL_TRIANGLES_ADJACENCY;
            case PrimitiveTopology::kTriangleStripWithAdjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
            case PrimitiveTopology::kPatchList: return GL_PATCHES;

            case PrimitiveTopology::kUnknown:
            case PrimitiveTopology::kCount:
            default:
                return GL_TRIANGLES;
        }
    }

    PrimitiveTopology fromGLPrimitiveTopology(GLenum mode) noexcept
    {
        switch(mode)
        {
            case GL_POINTS: return PrimitiveTopology::kPointList;
            case GL_LINES: return PrimitiveTopology::kLineList;
            case GL_LINE_STRIP: return PrimitiveTopology::kLineStrip;
            case GL_LINE_LOOP: return PrimitiveTopology::kLineLoop;
            case GL_TRIANGLES: return PrimitiveTopology::kTriangleList;
            case GL_TRIANGLE_STRIP: return PrimitiveTopology::kTriangleStrip;
            case GL_TRIANGLE_FAN: return PrimitiveTopology::kTriangleFan;
            case GL_LINES_ADJACENCY: return PrimitiveTopology::kLineListWithAdjacency;
            case GL_LINE_STRIP_ADJACENCY: return PrimitiveTopology::kLineStripWithAdjacency;
            case GL_TRIANGLES_ADJACENCY: return PrimitiveTopology::kTriangleListWithAdjacency;
            case GL_TRIANGLE_STRIP_ADJACENCY: return PrimitiveTopology::kTriangleStripWithAdjacency;
            case GL_PATCHES: return PrimitiveTopology::kPatchList;
            default:
                return PrimitiveTopology::kUnknown;
        }
    }

    GLenum toGLPolygonMode(PolygonMode mode) noexcept
    {
        switch(mode)
        {
            case PolygonMode::kLine: return GL_LINE;
            case PolygonMode::kPoint: return GL_POINT;
            case PolygonMode::kFill:
            case PolygonMode::kCount:
            default:
                return GL_FILL;
        }
    }

    PolygonMode fromGLPolygonMode(GLenum mode) noexcept
    {
        switch(mode)
        {
            case GL_LINE: return PolygonMode::kLine;
            case GL_POINT: return PolygonMode::kPoint;
            case GL_FILL: return PolygonMode::kFill;
            default: return PolygonMode::kFill;
        }
    }

    GLCullMode toGLCullMode(CullMode mode) noexcept
    {
        switch(mode)
        {
            case CullMode::kFront: return { true, GL_FRONT };
            case CullMode::kBack: return { true, GL_BACK };
            case CullMode::kFrontAndBack: return { true, GL_FRONT_AND_BACK };
            case CullMode::kNone:
            case CullMode::kCount:
            default:
                return { false, GL_BACK };
        }
    }

    CullMode fromGLCullMode(bool enabled, GLenum face) noexcept
    {
        if(!enabled)
        {
            return CullMode::kNone;
        }

        switch(face)
        {
            case GL_FRONT: return CullMode::kFront;
            case GL_FRONT_AND_BACK: return CullMode::kFrontAndBack;
            case GL_BACK:
            default:
                return CullMode::kBack;
        }
    }

    GLenum toGLFrontFace(FrontFace face) noexcept
    {
        switch(face)
        {
            case FrontFace::kClockwise: return GL_CW;
            case FrontFace::kCounterClockwise:
            case FrontFace::kCount:
            default:
                return GL_CCW;
        }
    }

    FrontFace fromGLFrontFace(GLenum face) noexcept
    {
        switch(face)
        {
            case GL_CW: return FrontFace::kClockwise;
            case GL_CCW:
            default:
                return FrontFace::kCounterClockwise;
        }
    }

    std::uint8_t toGLSampleCount(SampleCountFlag count) noexcept
    {
        return std::to_underlying(count);
    }

    SampleCountFlag fromGLSampleCount(std::uint8_t count) noexcept
    {
        return static_cast<SampleCountFlag>(count);
    }

    GLColorMask toGLColorMask(ColorComponent components) noexcept
    {
        const auto bits = static_cast<std::uint8_t>(components);
        return {
            (bits & static_cast<std::uint8_t>(ColorComponent::kR)) != 0,
            (bits & static_cast<std::uint8_t>(ColorComponent::kG)) != 0,
            (bits & static_cast<std::uint8_t>(ColorComponent::kB)) != 0,
            (bits & static_cast<std::uint8_t>(ColorComponent::kA)) != 0
        };
    }

    ColorComponent fromGLColorMask(bool r, bool g, bool b, bool a) noexcept
    {
        std::uint8_t bits = 0;
        if(r) bits |= static_cast<std::uint8_t>(ColorComponent::kR);
        if(g) bits |= static_cast<std::uint8_t>(ColorComponent::kG);
        if(b) bits |= static_cast<std::uint8_t>(ColorComponent::kB);
        if(a) bits |= static_cast<std::uint8_t>(ColorComponent::kA);
        return static_cast<ColorComponent>(bits);
    }

    GLenum toGLBlendFactor(BlendFactor factor) noexcept
    {
        switch(factor)
        {
            case BlendFactor::kOne: return GL_ONE;
            case BlendFactor::kSrcColor: return GL_SRC_COLOR;
            case BlendFactor::kOneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
            case BlendFactor::kDstColor: return GL_DST_COLOR;
            case BlendFactor::kOneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
            case BlendFactor::kSrcAlpha: return GL_SRC_ALPHA;
            case BlendFactor::kOneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
            case BlendFactor::kDstAlpha: return GL_DST_ALPHA;
            case BlendFactor::kOneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
            case BlendFactor::kConstantColor: return GL_CONSTANT_COLOR;
            case BlendFactor::kOneMinusConstantColor: return GL_ONE_MINUS_CONSTANT_COLOR;
            case BlendFactor::kConstantAlpha: return GL_CONSTANT_ALPHA;
            case BlendFactor::kOneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
            case BlendFactor::kSrcAlphaSaturate: return GL_SRC_ALPHA_SATURATE;
            case BlendFactor::kSrc1Color: return GL_SRC1_COLOR;
            case BlendFactor::kOneMinusSrc1Color: return GL_ONE_MINUS_SRC1_COLOR;
            case BlendFactor::kSrc1Alpha: return GL_SRC1_ALPHA;
            case BlendFactor::kOneMinusSrc1Alpha: return GL_ONE_MINUS_SRC1_ALPHA;
            case BlendFactor::kZero:
            case BlendFactor::kCount:
            default:
                return GL_ZERO;
        }
    }

    BlendFactor fromGLBlendFactor(GLenum factor) noexcept
    {
        switch(factor)
        {
            case GL_ONE: return BlendFactor::kOne;
            case GL_SRC_COLOR: return BlendFactor::kSrcColor;
            case GL_ONE_MINUS_SRC_COLOR: return BlendFactor::kOneMinusSrcColor;
            case GL_DST_COLOR: return BlendFactor::kDstColor;
            case GL_ONE_MINUS_DST_COLOR: return BlendFactor::kOneMinusDstColor;
            case GL_SRC_ALPHA: return BlendFactor::kSrcAlpha;
            case GL_ONE_MINUS_SRC_ALPHA: return BlendFactor::kOneMinusSrcAlpha;
            case GL_DST_ALPHA: return BlendFactor::kDstAlpha;
            case GL_ONE_MINUS_DST_ALPHA: return BlendFactor::kOneMinusDstAlpha;
            case GL_CONSTANT_COLOR: return BlendFactor::kConstantColor;
            case GL_ONE_MINUS_CONSTANT_COLOR: return BlendFactor::kOneMinusConstantColor;
            case GL_CONSTANT_ALPHA: return BlendFactor::kConstantAlpha;
            case GL_ONE_MINUS_CONSTANT_ALPHA: return BlendFactor::kOneMinusConstantAlpha;
            case GL_SRC_ALPHA_SATURATE: return BlendFactor::kSrcAlphaSaturate;
            case GL_SRC1_COLOR: return BlendFactor::kSrc1Color;
            case GL_ONE_MINUS_SRC1_COLOR: return BlendFactor::kOneMinusSrc1Color;
            case GL_SRC1_ALPHA: return BlendFactor::kSrc1Alpha;
            case GL_ONE_MINUS_SRC1_ALPHA: return BlendFactor::kOneMinusSrc1Alpha;
            case GL_ZERO:
            default:
                return BlendFactor::kZero;
        }
    }

    GLenum toGLBlendOp(BlendOp op) noexcept
    {
        switch(op)
        {
            case BlendOp::kSubtract: return GL_FUNC_SUBTRACT;
            case BlendOp::kReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
            case BlendOp::kMin: return GL_MIN;
            case BlendOp::kMax: return GL_MAX;
            case BlendOp::kAdd:
            case BlendOp::kCount:
            default:
                return GL_FUNC_ADD;
        }
    }

    BlendOp fromGLBlendOp(GLenum op) noexcept
    {
        switch(op)
        {
            case GL_FUNC_SUBTRACT: return BlendOp::kSubtract;
            case GL_FUNC_REVERSE_SUBTRACT: return BlendOp::kReverseSubtract;
            case GL_MIN: return BlendOp::kMin;
            case GL_MAX: return BlendOp::kMax;
            case GL_FUNC_ADD:
            default:
                return BlendOp::kAdd;
        }
    }

    GLenum toGLLogicOp(LogicOp op) noexcept
    {
        switch(op)
        {
            case LogicOp::kClear: return GL_CLEAR;
            case LogicOp::kAnd: return GL_AND;
            case LogicOp::kAndReverse: return GL_AND_REVERSE;
            case LogicOp::kAndInverted: return GL_AND_INVERTED;
            case LogicOp::kNoOp: return GL_NOOP;
            case LogicOp::kXor: return GL_XOR;
            case LogicOp::kOr: return GL_OR;
            case LogicOp::kNor: return GL_NOR;
            case LogicOp::kEquivalent: return GL_EQUIV;
            case LogicOp::kInvert: return GL_INVERT;
            case LogicOp::kOrReverse: return GL_OR_REVERSE;
            case LogicOp::kCopyInverted: return GL_COPY_INVERTED;
            case LogicOp::kOrInverted: return GL_OR_INVERTED;
            case LogicOp::kNand: return GL_NAND;
            case LogicOp::kSet: return GL_SET;
            case LogicOp::kCopy:
            case LogicOp::kCount:
            default:
                return GL_COPY;
        }
    }

    LogicOp fromGLLogicOp(GLenum op) noexcept
    {
        switch(op)
        {
            case GL_CLEAR: return LogicOp::kClear;
            case GL_AND: return LogicOp::kAnd;
            case GL_AND_REVERSE: return LogicOp::kAndReverse;
            case GL_AND_INVERTED: return LogicOp::kAndInverted;
            case GL_NOOP: return LogicOp::kNoOp;
            case GL_XOR: return LogicOp::kXor;
            case GL_OR: return LogicOp::kOr;
            case GL_NOR: return LogicOp::kNor;
            case GL_EQUIV: return LogicOp::kEquivalent;
            case GL_INVERT: return LogicOp::kInvert;
            case GL_OR_REVERSE: return LogicOp::kOrReverse;
            case GL_COPY_INVERTED: return LogicOp::kCopyInverted;
            case GL_OR_INVERTED: return LogicOp::kOrInverted;
            case GL_NAND: return LogicOp::kNand;
            case GL_SET: return LogicOp::kSet;
            case GL_COPY:
            default:
                return LogicOp::kCopy;
        }
    }

    GLAttachmentLoadOp toGLAttachmentLoadOp(AttachmentLoadOp op) noexcept
    {
        switch(op)
        {
            case AttachmentLoadOp::kClear: return { true, false };
            case AttachmentLoadOp::kDontCare:
            case AttachmentLoadOp::kNone: return { false, true };
            case AttachmentLoadOp::kLoad:
            case AttachmentLoadOp::kCount:
            default:
                return {};
        }
    }

    AttachmentLoadOp fromGLAttachmentLoadOp(GLAttachmentLoadOp op) noexcept
    {
        if(op.clear) return AttachmentLoadOp::kClear;
        if(op.invalidate) return AttachmentLoadOp::kDontCare;
        return AttachmentLoadOp::kLoad;
    }

    GLAttachmentStoreOp toGLAttachmentStoreOp(AttachmentStoreOp op) noexcept
    {
        switch(op)
        {
            case AttachmentStoreOp::kDontCare:
            case AttachmentStoreOp::kNone: return { true };
            case AttachmentStoreOp::kStore:
            case AttachmentStoreOp::kCount:
            default:
                return {};
        }
    }

    AttachmentStoreOp fromGLAttachmentStoreOp(GLAttachmentStoreOp op) noexcept
    {
        return op.invalidate ? AttachmentStoreOp::kDontCare : AttachmentStoreOp::kStore;
    }

    GLImageAspect toGLImageAspect(ImageAspect aspect) noexcept
    {
        const auto bits = static_cast<std::uint8_t>(aspect);
        return {
            (bits & static_cast<std::uint8_t>(ImageAspect::kColor)) != 0,
            (bits & static_cast<std::uint8_t>(ImageAspect::kDepth)) != 0,
            (bits & static_cast<std::uint8_t>(ImageAspect::kStencil)) != 0
        };
    }

    ImageAspect fromGLImageAspect(GLImageAspect aspect) noexcept
    {
        std::uint8_t bits = 0;
        if(aspect.color) bits |= static_cast<std::uint8_t>(ImageAspect::kColor);
        if(aspect.depth) bits |= static_cast<std::uint8_t>(ImageAspect::kDepth);
        if(aspect.stencil) bits |= static_cast<std::uint8_t>(ImageAspect::kStencil);
        return static_cast<ImageAspect>(bits);
    }

    GLenum toGLComponentSwizzle(ComponentSwizzle swizzle, GLenum identity_value) noexcept
    {
        switch(swizzle)
        {
            case ComponentSwizzle::kZero: return GL_ZERO;
            case ComponentSwizzle::kOne: return GL_ONE;
            case ComponentSwizzle::kR: return GL_RED;
            case ComponentSwizzle::kG: return GL_GREEN;
            case ComponentSwizzle::kB: return GL_BLUE;
            case ComponentSwizzle::kA: return GL_ALPHA;
            case ComponentSwizzle::kIdentity:
            case ComponentSwizzle::kCount:
            default:
                return identity_value;
        }
    }

    ComponentSwizzle fromGLComponentSwizzle(GLenum swizzle, GLenum identity_value) noexcept
    {
        if(swizzle == identity_value) return ComponentSwizzle::kIdentity;
        switch(swizzle)
        {
            case GL_ZERO: return ComponentSwizzle::kZero;
            case GL_ONE: return ComponentSwizzle::kOne;
            case GL_RED: return ComponentSwizzle::kR;
            case GL_GREEN: return ComponentSwizzle::kG;
            case GL_BLUE: return ComponentSwizzle::kB;
            case GL_ALPHA: return ComponentSwizzle::kA;
            default:
                return ComponentSwizzle::kIdentity;
        }
    }

    GLComponentMapping toGLComponentMapping(const ComponentMapping& mapping) noexcept
    {
        return {
            toGLComponentSwizzle(mapping.r, GL_RED),
            toGLComponentSwizzle(mapping.g, GL_GREEN),
            toGLComponentSwizzle(mapping.b, GL_BLUE),
            toGLComponentSwizzle(mapping.a, GL_ALPHA)
        };
    }

    ComponentMapping fromGLComponentMapping(const GLComponentMapping& mapping) noexcept
    {
        return {
            fromGLComponentSwizzle(mapping.r, GL_RED),
            fromGLComponentSwizzle(mapping.g, GL_GREEN),
            fromGLComponentSwizzle(mapping.b, GL_BLUE),
            fromGLComponentSwizzle(mapping.a, GL_ALPHA)
        };
    }

    GLenum toGLBufferTarget(BufferType type) noexcept
    {
        const auto bits = static_cast<std::uint16_t>(type);
        // Prefer the "primary" bind target when several usage bits are set.
        if(bits & static_cast<std::uint16_t>(BufferType::kVertex)) return GL_ARRAY_BUFFER;
        if(bits & static_cast<std::uint16_t>(BufferType::kIndex)) return GL_ELEMENT_ARRAY_BUFFER;
        if(bits & static_cast<std::uint16_t>(BufferType::kUniform)) return GL_UNIFORM_BUFFER;
        if(bits & static_cast<std::uint16_t>(BufferType::kStorage)) return GL_SHADER_STORAGE_BUFFER;
        if(bits & static_cast<std::uint16_t>(BufferType::kIndirect)) return GL_DRAW_INDIRECT_BUFFER;
        if(bits & static_cast<std::uint16_t>(BufferType::kTransferSrc)) return GL_COPY_READ_BUFFER;
        if(bits & static_cast<std::uint16_t>(BufferType::kTransferDst)) return GL_COPY_WRITE_BUFFER;
        return GL_ARRAY_BUFFER;
    }

    BufferType fromGLBufferTarget(GLenum target) noexcept
    {
        switch(target)
        {
            case GL_ELEMENT_ARRAY_BUFFER: return BufferType::kIndex;
            case GL_UNIFORM_BUFFER: return BufferType::kUniform;
            case GL_SHADER_STORAGE_BUFFER: return BufferType::kStorage;
            case GL_DRAW_INDIRECT_BUFFER: return BufferType::kIndirect;
            case GL_COPY_READ_BUFFER: return BufferType::kTransferSrc;
            case GL_COPY_WRITE_BUFFER: return BufferType::kTransferDst;
            case GL_ARRAY_BUFFER:
            default:
                return BufferType::kVertex;
        }
    }

    GLenum toGLIndexType(IndexType type) noexcept
    {
        switch(type)
        {
            case IndexType::kUint16: return GL_UNSIGNED_SHORT;
            case IndexType::kUint32: return GL_UNSIGNED_INT;
            case IndexType::kUint8: return GL_UNSIGNED_BYTE;
            default: return GL_UNSIGNED_SHORT;
        }
    }

    IndexType fromGLIndexType(GLenum type) noexcept
    {
        switch(type)
        {
            case GL_UNSIGNED_INT: return IndexType::kUint32;
            case GL_UNSIGNED_BYTE: return IndexType::kUint8;
            case GL_UNSIGNED_SHORT:
            default:
                return IndexType::kUint16;
        }
    }

    GLenum toGLDescriptorTarget(DescriptorType type) noexcept
    {
        switch(type)
        {
            case DescriptorType::kUniformBuffer:
            case DescriptorType::kUniformBufferDynamic:
                return GL_UNIFORM_BUFFER;

            case DescriptorType::kStorageBuffer:
            case DescriptorType::kStorageBufferDynamic:
                return GL_SHADER_STORAGE_BUFFER;

            case DescriptorType::kUniformTexelBuffer:
            case DescriptorType::kStorageTexelBuffer:
                return GL_TEXTURE_BUFFER;

            case DescriptorType::kCombinedImageSampler:
            case DescriptorType::kSampledImage:
            case DescriptorType::kStorageImage:
                // Actual texture target is format/dimensionality-dependent;
                // 2D is the common default for RHI emulation.
                return GL_TEXTURE_2D;

            case DescriptorType::kSampler:
            case DescriptorType::kInputAttachment:
            default:
                return GL_NONE;
        }
    }

    DescriptorType fromGLDescriptorTarget(GLenum target) noexcept
    {
        switch(target)
        {
            case GL_SHADER_STORAGE_BUFFER: return DescriptorType::kStorageBuffer;
            case GL_TEXTURE_BUFFER: return DescriptorType::kUniformTexelBuffer;
            case GL_TEXTURE_2D:
            case GL_TEXTURE_1D:
            case GL_TEXTURE_3D:
            case GL_TEXTURE_CUBE_MAP:
            case GL_TEXTURE_2D_ARRAY:
            case GL_TEXTURE_CUBE_MAP_ARRAY:
            case GL_TEXTURE_2D_MULTISAMPLE:
                return DescriptorType::kCombinedImageSampler;
            case GL_UNIFORM_BUFFER:
            default:
                return DescriptorType::kUniformBuffer;
        }
    }

    GLenum toGLShaderType(ShaderStageType stage) noexcept
    {
        const auto bits = static_cast<std::uint32_t>(stage);
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kVertex)) return GL_VERTEX_SHADER;
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kFragment)) return GL_FRAGMENT_SHADER;
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kGeometry)) return GL_GEOMETRY_SHADER;
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kTessellationControl)) return GL_TESS_CONTROL_SHADER;
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kTessellationEval)) return GL_TESS_EVALUATION_SHADER;
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kCompute)) return GL_COMPUTE_SHADER;
        return GL_NONE;
    }

    ShaderStageType fromGLShaderType(GLenum type) noexcept
    {
        switch(type)
        {
            case GL_VERTEX_SHADER: return ShaderStageType::kVertex;
            case GL_FRAGMENT_SHADER: return ShaderStageType::kFragment;
            case GL_GEOMETRY_SHADER: return ShaderStageType::kGeometry;
            case GL_TESS_CONTROL_SHADER: return ShaderStageType::kTessellationControl;
            case GL_TESS_EVALUATION_SHADER: return ShaderStageType::kTessellationEval;
            case GL_COMPUTE_SHADER: return ShaderStageType::kCompute;
            default: return ShaderStageType::kUnknown;
        }
    }

    GLenum toGLQueueFamily(QueueFamilyType) noexcept
    {
        return GL_NONE;
    }

    QueueFamilyType fromGLQueueFamily(GLenum) noexcept
    {
        return QueueFamilyType::kGraphics;
    }

    GLenum toGLPipelineBindPoint(PipelineBindPoint) noexcept
    {
        return GL_NONE;
    }

    PipelineBindPoint fromGLPipelineBindPoint(GLenum) noexcept
    {
        return PipelineBindPoint::kGraphics;
    }

    GLenum toGLFilter(FilterType filter) noexcept
    {
        switch(filter)
        {
            case FilterType::kNearest: return GL_NEAREST;
            case FilterType::kLinear:
            default:
                return GL_LINEAR;
        }
    }

    FilterType fromGLFilter(GLenum filter) noexcept
    {
        switch(filter)
        {
            case GL_NEAREST:
            case GL_NEAREST_MIPMAP_NEAREST:
            case GL_NEAREST_MIPMAP_LINEAR:
                return FilterType::kNearest;
            case GL_LINEAR:
            case GL_LINEAR_MIPMAP_NEAREST:
            case GL_LINEAR_MIPMAP_LINEAR:
            default:
                return FilterType::kLinear;
        }
    }

    GLenum toGLMipmapMode(MipmapMode mode) noexcept
    {
        switch(mode)
        {
            case MipmapMode::kNearest: return GL_NEAREST;
            case MipmapMode::kLinear:
            default:
                return GL_LINEAR;
        }
    }

    MipmapMode fromGLMipmapMode(GLenum mode) noexcept
    {
        switch(mode)
        {
            case GL_NEAREST:
            case GL_NEAREST_MIPMAP_NEAREST:
            case GL_LINEAR_MIPMAP_NEAREST:
                return MipmapMode::kNearest;
            case GL_LINEAR:
            case GL_NEAREST_MIPMAP_LINEAR:
            case GL_LINEAR_MIPMAP_LINEAR:
            default:
                return MipmapMode::kLinear;
        }
    }

    GLenum toGLMinFilter(FilterType min_filter, MipmapMode mipmap_mode) noexcept
    {
        if(min_filter == FilterType::kNearest)
        {
            return mipmap_mode == MipmapMode::kNearest ? GL_NEAREST_MIPMAP_NEAREST
                                                       : GL_NEAREST_MIPMAP_LINEAR;
        }
        return mipmap_mode == MipmapMode::kNearest ? GL_LINEAR_MIPMAP_NEAREST
                                                  : GL_LINEAR_MIPMAP_LINEAR;
    }

    GLenum toGLRepeatMode(RepeatMode mode) noexcept
    {
        switch(mode)
        {
            case RepeatMode::kMirroredRepeat: return GL_MIRRORED_REPEAT;
            case RepeatMode::kClampToEdge: return GL_CLAMP_TO_EDGE;
            case RepeatMode::kClampToBorder: return GL_CLAMP_TO_BORDER;
            case RepeatMode::kMirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
            case RepeatMode::kRepeat:
            default:
                return GL_REPEAT;
        }
    }

    RepeatMode fromGLRepeatMode(GLenum mode) noexcept
    {
        switch(mode)
        {
            case GL_MIRRORED_REPEAT: return RepeatMode::kMirroredRepeat;
            case GL_CLAMP_TO_EDGE: return RepeatMode::kClampToEdge;
            case GL_CLAMP_TO_BORDER: return RepeatMode::kClampToBorder;
            case GL_MIRROR_CLAMP_TO_EDGE: return RepeatMode::kMirrorClampToEdge;
            case GL_REPEAT:
            default:
                return RepeatMode::kRepeat;
        }
    }

    GLenum toGLCompareOp(CompareOp op) noexcept
    {
        switch(op)
        {
            case CompareOp::kNever: return GL_NEVER;
            case CompareOp::kLess: return GL_LESS;
            case CompareOp::kEqual: return GL_EQUAL;
            case CompareOp::kLessOrEqual: return GL_LEQUAL;
            case CompareOp::kGreater: return GL_GREATER;
            case CompareOp::kNotEqual: return GL_NOTEQUAL;
            case CompareOp::kGreaterOrEqual: return GL_GEQUAL;
            case CompareOp::kAlways:
            default:
                return GL_ALWAYS;
        }
    }

    CompareOp fromGLCompareOp(GLenum op) noexcept
    {
        switch(op)
        {
            case GL_NEVER: return CompareOp::kNever;
            case GL_LESS: return CompareOp::kLess;
            case GL_EQUAL: return CompareOp::kEqual;
            case GL_LEQUAL: return CompareOp::kLessOrEqual;
            case GL_GREATER: return CompareOp::kGreater;
            case GL_NOTEQUAL: return CompareOp::kNotEqual;
            case GL_GEQUAL: return CompareOp::kGreaterOrEqual;
            case GL_ALWAYS:
            default:
                return CompareOp::kAlways;
        }
    }
}
