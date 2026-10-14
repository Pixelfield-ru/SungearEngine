//
// Created by stuka on 12.09.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_types.h"

namespace pf::rhi
{
    VkFormat toVkFormat(Format format) noexcept
    {
        switch(format)
        {
            case Format::kR8Unorm: return VK_FORMAT_R8_UNORM;
            case Format::kRg8Unorm: return VK_FORMAT_R8G8_UNORM;
            case Format::kRgb8Unorm: return VK_FORMAT_R8G8B8_UNORM;
            case Format::kRgba8Unorm: return VK_FORMAT_R8G8B8A8_UNORM;
            case Format::kBgra8Unorm: return VK_FORMAT_B8G8R8A8_UNORM;

            case Format::kRgba8Srgb: return VK_FORMAT_R8G8B8A8_SRGB;
            case Format::kBgra8Srgb: return VK_FORMAT_B8G8R8A8_SRGB;

            case Format::kR8Snorm: return VK_FORMAT_R8_SNORM;
            case Format::kRg8Snorm: return VK_FORMAT_R8G8_SNORM;
            case Format::kRgba8Snorm: return VK_FORMAT_R8G8B8A8_SNORM;

            case Format::kR8Uint: return VK_FORMAT_R8_UINT;
            case Format::kRg8Uint: return VK_FORMAT_R8G8_UINT;
            case Format::kRgba8Uint: return VK_FORMAT_R8G8B8A8_UINT;
            case Format::kR8Sint: return VK_FORMAT_R8_SINT;
            case Format::kRg8Sint: return VK_FORMAT_R8G8_SINT;
            case Format::kRgba8Sint: return VK_FORMAT_R8G8B8A8_SINT;

            case Format::kR16Unorm: return VK_FORMAT_R16_UNORM;
            case Format::kRg16Unorm: return VK_FORMAT_R16G16_UNORM;
            case Format::kRgba16Unorm: return VK_FORMAT_R16G16B16A16_UNORM;
            case Format::kR16Snorm: return VK_FORMAT_R16_SNORM;
            case Format::kRg16Snorm: return VK_FORMAT_R16G16_SNORM;
            case Format::kRgba16Snorm: return VK_FORMAT_R16G16B16A16_SNORM;
            case Format::kR16Uint: return VK_FORMAT_R16_UINT;
            case Format::kRg16Uint: return VK_FORMAT_R16G16_UINT;
            case Format::kRgba16Uint: return VK_FORMAT_R16G16B16A16_UINT;
            case Format::kR16Sint: return VK_FORMAT_R16_SINT;
            case Format::kRg16Sint: return VK_FORMAT_R16G16_SINT;
            case Format::kRgba16Sint: return VK_FORMAT_R16G16B16A16_SINT;
            case Format::kR16Float: return VK_FORMAT_R16_SFLOAT;
            case Format::kRg16Float: return VK_FORMAT_R16G16_SFLOAT;
            case Format::kRgba16Float: return VK_FORMAT_R16G16B16A16_SFLOAT;

            case Format::kR32Uint: return VK_FORMAT_R32_UINT;
            case Format::kRg32Uint: return VK_FORMAT_R32G32_UINT;
            case Format::kRgba32Uint: return VK_FORMAT_R32G32B32A32_UINT;
            case Format::kR32Sint: return VK_FORMAT_R32_SINT;
            case Format::kRg32Sint: return VK_FORMAT_R32G32_SINT;
            case Format::kRgba32Sint: return VK_FORMAT_R32G32B32A32_SINT;
            case Format::kR32Float: return VK_FORMAT_R32_SFLOAT;
            case Format::kRg32Float: return VK_FORMAT_R32G32_SFLOAT;
            case Format::kRgb32Float: return VK_FORMAT_R32G32B32_SFLOAT;
            case Format::kRgba32Float: return VK_FORMAT_R32G32B32A32_SFLOAT;

            case Format::kR10g10b10a2Unorm: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
            case Format::kB10g10r10a2Unorm: return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
            case Format::kR11g11b10Float: return VK_FORMAT_B10G11R11_UFLOAT_PACK32;

            case Format::kD16Unorm: return VK_FORMAT_D16_UNORM;
            case Format::kD24UnormS8Uint: return VK_FORMAT_D24_UNORM_S8_UINT;
            case Format::kD32Float: return VK_FORMAT_D32_SFLOAT;
            case Format::kD32FloatS8Uint: return VK_FORMAT_D32_SFLOAT_S8_UINT;
            case Format::kS8Uint: return VK_FORMAT_S8_UINT;

            case Format::kBc1RgbUnorm: return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
            case Format::kBc1RgbSrgb: return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
            case Format::kBc1RgbaUnorm: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
            case Format::kBc1RgbaSrgb: return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
            case Format::kBc2Unorm: return VK_FORMAT_BC2_UNORM_BLOCK;
            case Format::kBc2Srgb: return VK_FORMAT_BC2_SRGB_BLOCK;
            case Format::kBc3Unorm: return VK_FORMAT_BC3_UNORM_BLOCK;
            case Format::kBc3Srgb: return VK_FORMAT_BC3_SRGB_BLOCK;
            case Format::kBc4Unorm: return VK_FORMAT_BC4_UNORM_BLOCK;
            case Format::kBc4Snorm: return VK_FORMAT_BC4_SNORM_BLOCK;
            case Format::kBc5Unorm: return VK_FORMAT_BC5_UNORM_BLOCK;
            case Format::kBc5Snorm: return VK_FORMAT_BC5_SNORM_BLOCK;
            case Format::kBc6hUfloat: return VK_FORMAT_BC6H_UFLOAT_BLOCK;
            case Format::kBc6hSfloat: return VK_FORMAT_BC6H_SFLOAT_BLOCK;
            case Format::kBc7Unorm: return VK_FORMAT_BC7_UNORM_BLOCK;
            case Format::kBc7Srgb: return VK_FORMAT_BC7_SRGB_BLOCK;

            case Format::kUnknown:
            case Format::kCount:
            default:
                return VK_FORMAT_UNDEFINED;
        }
    }

    Format fromVkFormat(VkFormat format) noexcept
    {
        switch(format)
        {
            case VK_FORMAT_R8_UNORM: return Format::kR8Unorm;
            case VK_FORMAT_R8G8_UNORM: return Format::kRg8Unorm;
            case VK_FORMAT_R8G8B8_UNORM: return Format::kRgb8Unorm;
            case VK_FORMAT_R8G8B8A8_UNORM: return Format::kRgba8Unorm;
            case VK_FORMAT_B8G8R8A8_UNORM: return Format::kBgra8Unorm;

            case VK_FORMAT_R8G8B8A8_SRGB: return Format::kRgba8Srgb;
            case VK_FORMAT_B8G8R8A8_SRGB: return Format::kBgra8Srgb;

            case VK_FORMAT_R8_SNORM: return Format::kR8Snorm;
            case VK_FORMAT_R8G8_SNORM: return Format::kRg8Snorm;
            case VK_FORMAT_R8G8B8A8_SNORM: return Format::kRgba8Snorm;

            case VK_FORMAT_R8_UINT: return Format::kR8Uint;
            case VK_FORMAT_R8G8_UINT: return Format::kRg8Uint;
            case VK_FORMAT_R8G8B8A8_UINT: return Format::kRgba8Uint;
            case VK_FORMAT_R8_SINT: return Format::kR8Sint;
            case VK_FORMAT_R8G8_SINT: return Format::kRg8Sint;
            case VK_FORMAT_R8G8B8A8_SINT: return Format::kRgba8Sint;

            case VK_FORMAT_R16_UNORM: return Format::kR16Unorm;
            case VK_FORMAT_R16G16_UNORM: return Format::kRg16Unorm;
            case VK_FORMAT_R16G16B16A16_UNORM: return Format::kRgba16Unorm;
            case VK_FORMAT_R16_SNORM: return Format::kR16Snorm;
            case VK_FORMAT_R16G16_SNORM: return Format::kRg16Snorm;
            case VK_FORMAT_R16G16B16A16_SNORM: return Format::kRgba16Snorm;
            case VK_FORMAT_R16_UINT: return Format::kR16Uint;
            case VK_FORMAT_R16G16_UINT: return Format::kRg16Uint;
            case VK_FORMAT_R16G16B16A16_UINT: return Format::kRgba16Uint;
            case VK_FORMAT_R16_SINT: return Format::kR16Sint;
            case VK_FORMAT_R16G16_SINT: return Format::kRg16Sint;
            case VK_FORMAT_R16G16B16A16_SINT: return Format::kRgba16Sint;
            case VK_FORMAT_R16_SFLOAT: return Format::kR16Float;
            case VK_FORMAT_R16G16_SFLOAT: return Format::kRg16Float;
            case VK_FORMAT_R16G16B16A16_SFLOAT: return Format::kRgba16Float;

            case VK_FORMAT_R32_UINT: return Format::kR32Uint;
            case VK_FORMAT_R32G32_UINT: return Format::kRg32Uint;
            case VK_FORMAT_R32G32B32A32_UINT: return Format::kRgba32Uint;
            case VK_FORMAT_R32_SINT: return Format::kR32Sint;
            case VK_FORMAT_R32G32_SINT: return Format::kRg32Sint;
            case VK_FORMAT_R32G32B32A32_SINT: return Format::kRgba32Sint;
            case VK_FORMAT_R32_SFLOAT: return Format::kR32Float;
            case VK_FORMAT_R32G32_SFLOAT: return Format::kRg32Float;
            case VK_FORMAT_R32G32B32_SFLOAT: return Format::kRgb32Float;
            case VK_FORMAT_R32G32B32A32_SFLOAT: return Format::kRgba32Float;

            case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return Format::kR10g10b10a2Unorm;
            case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return Format::kB10g10r10a2Unorm;
            case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return Format::kR11g11b10Float;

            case VK_FORMAT_D16_UNORM: return Format::kD16Unorm;
            case VK_FORMAT_D24_UNORM_S8_UINT: return Format::kD24UnormS8Uint;
            case VK_FORMAT_D32_SFLOAT: return Format::kD32Float;
            case VK_FORMAT_D32_SFLOAT_S8_UINT: return Format::kD32FloatS8Uint;
            case VK_FORMAT_S8_UINT: return Format::kS8Uint;

            case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return Format::kBc1RgbUnorm;
            case VK_FORMAT_BC1_RGB_SRGB_BLOCK: return Format::kBc1RgbSrgb;
            case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return Format::kBc1RgbaUnorm;
            case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return Format::kBc1RgbaSrgb;
            case VK_FORMAT_BC2_UNORM_BLOCK: return Format::kBc2Unorm;
            case VK_FORMAT_BC2_SRGB_BLOCK: return Format::kBc2Srgb;
            case VK_FORMAT_BC3_UNORM_BLOCK: return Format::kBc3Unorm;
            case VK_FORMAT_BC3_SRGB_BLOCK: return Format::kBc3Srgb;
            case VK_FORMAT_BC4_UNORM_BLOCK: return Format::kBc4Unorm;
            case VK_FORMAT_BC4_SNORM_BLOCK: return Format::kBc4Snorm;
            case VK_FORMAT_BC5_UNORM_BLOCK: return Format::kBc5Unorm;
            case VK_FORMAT_BC5_SNORM_BLOCK: return Format::kBc5Snorm;
            case VK_FORMAT_BC6H_UFLOAT_BLOCK: return Format::kBc6hUfloat;
            case VK_FORMAT_BC6H_SFLOAT_BLOCK: return Format::kBc6hSfloat;
            case VK_FORMAT_BC7_UNORM_BLOCK: return Format::kBc7Unorm;
            case VK_FORMAT_BC7_SRGB_BLOCK: return Format::kBc7Srgb;

            default:
                return Format::kUnknown;
        }
    }

    VkColorSpaceKHR toVkColorSpace(ColorSpace color_space) noexcept
    {
        switch(color_space)
        {
            case ColorSpace::kSrgbNonlinear: return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
            case ColorSpace::kDisplayP3Nonlinear: return VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT;
            case ColorSpace::kExtendedSrgbLinear: return VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
            case ColorSpace::kDisplayP3Linear: return VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT;
            case ColorSpace::kDciP3Nonlinear: return VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT;
            case ColorSpace::kBt709Linear: return VK_COLOR_SPACE_BT709_LINEAR_EXT;
            case ColorSpace::kBt709Nonlinear: return VK_COLOR_SPACE_BT709_NONLINEAR_EXT;
            case ColorSpace::kBt2020Linear: return VK_COLOR_SPACE_BT2020_LINEAR_EXT;
            case ColorSpace::kHdr10St2084: return VK_COLOR_SPACE_HDR10_ST2084_EXT;
            case ColorSpace::kDolbyVision: return VK_COLOR_SPACE_DOLBYVISION_EXT;
            case ColorSpace::kHdr10Hlg: return VK_COLOR_SPACE_HDR10_HLG_EXT;
            case ColorSpace::kAdobeRgbLinear: return VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT;
            case ColorSpace::kAdobeRgbNonlinear: return VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT;
            case ColorSpace::kPassThrough: return VK_COLOR_SPACE_PASS_THROUGH_EXT;
            case ColorSpace::kExtendedSrgbNonlinear: return VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT;

            case ColorSpace::kUnknown:
            case ColorSpace::kCount:
            default:
                return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        }
    }

    ColorSpace fromVkColorSpace(VkColorSpaceKHR color_space) noexcept
    {
        switch(color_space)
        {
            case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR: return ColorSpace::kSrgbNonlinear;
            case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT: return ColorSpace::kDisplayP3Nonlinear;
            case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT: return ColorSpace::kExtendedSrgbLinear;
            case VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT: return ColorSpace::kDisplayP3Linear;
            case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT: return ColorSpace::kDciP3Nonlinear;
            case VK_COLOR_SPACE_BT709_LINEAR_EXT: return ColorSpace::kBt709Linear;
            case VK_COLOR_SPACE_BT709_NONLINEAR_EXT: return ColorSpace::kBt709Nonlinear;
            case VK_COLOR_SPACE_BT2020_LINEAR_EXT: return ColorSpace::kBt2020Linear;
            case VK_COLOR_SPACE_HDR10_ST2084_EXT: return ColorSpace::kHdr10St2084;
            case VK_COLOR_SPACE_DOLBYVISION_EXT: return ColorSpace::kDolbyVision;
            case VK_COLOR_SPACE_HDR10_HLG_EXT: return ColorSpace::kHdr10Hlg;
            case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT: return ColorSpace::kAdobeRgbLinear;
            case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT: return ColorSpace::kAdobeRgbNonlinear;
            case VK_COLOR_SPACE_PASS_THROUGH_EXT: return ColorSpace::kPassThrough;
            case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT: return ColorSpace::kExtendedSrgbNonlinear;
            default:
                return ColorSpace::kUnknown;
        }
    }

    VkImageType toVkImageType(ImageType image_type) noexcept
    {
        switch(image_type)
        {
            case ImageType::k1D:
            case ImageType::k1DArray:
                return VK_IMAGE_TYPE_1D;

            case ImageType::k2D:
            case ImageType::k2DArray:
            case ImageType::kCube:
            case ImageType::kCubeArray:
                return VK_IMAGE_TYPE_2D;

            case ImageType::k3D:
                return VK_IMAGE_TYPE_3D;

            case ImageType::kUnknown:
            case ImageType::kCount:
            default:
                return VK_IMAGE_TYPE_2D;
        }
    }

    ImageType fromVkImageType(VkImageType image_type) noexcept
    {
        switch(image_type)
        {
            case VK_IMAGE_TYPE_1D: return ImageType::k1D;
            case VK_IMAGE_TYPE_2D: return ImageType::k2D;
            case VK_IMAGE_TYPE_3D: return ImageType::k3D;
            default: return ImageType::kUnknown;
        }
    }

    VkImageViewType toVkImageViewType(ImageType image_type) noexcept
    {
        switch(image_type)
        {
            case ImageType::k1D: return VK_IMAGE_VIEW_TYPE_1D;
            case ImageType::k2D: return VK_IMAGE_VIEW_TYPE_2D;
            case ImageType::k3D: return VK_IMAGE_VIEW_TYPE_3D;
            case ImageType::kCube: return VK_IMAGE_VIEW_TYPE_CUBE;
            case ImageType::k1DArray: return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
            case ImageType::k2DArray: return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            case ImageType::kCubeArray: return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

            case ImageType::kUnknown:
            case ImageType::kCount:
            default:
                return VK_IMAGE_VIEW_TYPE_2D;
        }
    }

    VkImageTiling toVkImageTiling(ImageTiling tiling) noexcept
    {
        switch(tiling)
        {
            case ImageTiling::kLinear: return VK_IMAGE_TILING_LINEAR;
            case ImageTiling::kOptimal:
            default:
                return VK_IMAGE_TILING_OPTIMAL;
        }
    }

    ImageTiling fromVkImageTiling(VkImageTiling tiling) noexcept
    {
        switch(tiling)
        {
            case VK_IMAGE_TILING_LINEAR: return ImageTiling::kLinear;
            case VK_IMAGE_TILING_OPTIMAL:
            default:
                return ImageTiling::kOptimal;
        }
    }

    VkImageUsageFlags toVkImageUsage(ImageUsage usage) noexcept
    {
        VkImageUsageFlags flags = 0;
        const auto bits = static_cast<std::uint16_t>(usage);
        if(bits & static_cast<std::uint16_t>(ImageUsage::kTransferSrc))
            flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kTransferDst))
            flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kSampled))
            flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kStorage))
            flags |= VK_IMAGE_USAGE_STORAGE_BIT;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kColorAttachment))
            flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kDepthStencilAttachment))
            flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kTransientAttachment))
            flags |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
        if(bits & static_cast<std::uint16_t>(ImageUsage::kInputAttachment))
            flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
        return flags;
    }

    ImageUsage fromVkImageUsage(VkImageUsageFlags usage) noexcept
    {
        std::uint16_t bits = 0;
        if(usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
            bits |= static_cast<std::uint16_t>(ImageUsage::kTransferSrc);
        if(usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            bits |= static_cast<std::uint16_t>(ImageUsage::kTransferDst);
        if(usage & VK_IMAGE_USAGE_SAMPLED_BIT)
            bits |= static_cast<std::uint16_t>(ImageUsage::kSampled);
        if(usage & VK_IMAGE_USAGE_STORAGE_BIT)
            bits |= static_cast<std::uint16_t>(ImageUsage::kStorage);
        if(usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            bits |= static_cast<std::uint16_t>(ImageUsage::kColorAttachment);
        if(usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
            bits |= static_cast<std::uint16_t>(ImageUsage::kDepthStencilAttachment);
        if(usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)
            bits |= static_cast<std::uint16_t>(ImageUsage::kTransientAttachment);
        if(usage & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT)
            bits |= static_cast<std::uint16_t>(ImageUsage::kInputAttachment);
        return static_cast<ImageUsage>(bits);
    }

    VkPrimitiveTopology toVkPrimitiveTopology(PrimitiveTopology topology) noexcept
    {
        switch(topology)
        {
            case PrimitiveTopology::kPointList: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
            case PrimitiveTopology::kLineList: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            case PrimitiveTopology::kLineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
            case PrimitiveTopology::kTriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            case PrimitiveTopology::kTriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
            case PrimitiveTopology::kTriangleFan: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
            case PrimitiveTopology::kLineListWithAdjacency: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
            case PrimitiveTopology::kLineStripWithAdjacency: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
            case PrimitiveTopology::kTriangleListWithAdjacency: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
            case PrimitiveTopology::kTriangleStripWithAdjacency: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
            case PrimitiveTopology::kPatchList: return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

            // Vulkan has no line loop.
            case PrimitiveTopology::kLineLoop:
            case PrimitiveTopology::kUnknown:
            case PrimitiveTopology::kCount:
            default:
                return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
        }
    }

    PrimitiveTopology fromVkPrimitiveTopology(VkPrimitiveTopology topology) noexcept
    {
        switch(topology)
        {
            case VK_PRIMITIVE_TOPOLOGY_POINT_LIST: return PrimitiveTopology::kPointList;
            case VK_PRIMITIVE_TOPOLOGY_LINE_LIST: return PrimitiveTopology::kLineList;
            case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: return PrimitiveTopology::kLineStrip;
            case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: return PrimitiveTopology::kTriangleList;
            case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: return PrimitiveTopology::kTriangleStrip;
            case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN: return PrimitiveTopology::kTriangleFan;
            case VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY: return PrimitiveTopology::kLineListWithAdjacency;
            case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY: return PrimitiveTopology::kLineStripWithAdjacency;
            case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY: return PrimitiveTopology::kTriangleListWithAdjacency;
            case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY: return PrimitiveTopology::kTriangleStripWithAdjacency;
            case VK_PRIMITIVE_TOPOLOGY_PATCH_LIST: return PrimitiveTopology::kPatchList;
            default:
                return PrimitiveTopology::kUnknown;
        }
    }

    VkPolygonMode toVkPolygonMode(PolygonMode mode) noexcept
    {
        switch(mode)
        {
            case PolygonMode::kLine: return VK_POLYGON_MODE_LINE;
            case PolygonMode::kPoint: return VK_POLYGON_MODE_POINT;
            case PolygonMode::kFill:
            case PolygonMode::kCount:
            default:
                return VK_POLYGON_MODE_FILL;
        }
    }

    PolygonMode fromVkPolygonMode(VkPolygonMode mode) noexcept
    {
        switch(mode)
        {
            case VK_POLYGON_MODE_LINE: return PolygonMode::kLine;
            case VK_POLYGON_MODE_POINT: return PolygonMode::kPoint;
            case VK_POLYGON_MODE_FILL: return PolygonMode::kFill;
            default: return PolygonMode::kFill;
        }
    }

    VkCullModeFlags toVkCullMode(CullMode mode) noexcept
    {
        switch(mode)
        {
            case CullMode::kFront: return VK_CULL_MODE_FRONT_BIT;
            case CullMode::kBack: return VK_CULL_MODE_BACK_BIT;
            case CullMode::kFrontAndBack: return VK_CULL_MODE_FRONT_AND_BACK;
            case CullMode::kNone:
            case CullMode::kCount:
            default:
                return VK_CULL_MODE_NONE;
        }
    }

    CullMode fromVkCullMode(VkCullModeFlags mode) noexcept
    {
        switch(mode)
        {
            case VK_CULL_MODE_FRONT_BIT: return CullMode::kFront;
            case VK_CULL_MODE_BACK_BIT: return CullMode::kBack;
            case VK_CULL_MODE_FRONT_AND_BACK: return CullMode::kFrontAndBack;
            case VK_CULL_MODE_NONE:
            default:
                return CullMode::kNone;
        }
    }

    VkFrontFace toVkFrontFace(FrontFace face) noexcept
    {
        switch(face)
        {
            case FrontFace::kClockwise: return VK_FRONT_FACE_CLOCKWISE;
            case FrontFace::kCounterClockwise:
            case FrontFace::kCount:
            default:
                return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        }
    }

    FrontFace fromVkFrontFace(VkFrontFace face) noexcept
    {
        switch(face)
        {
            case VK_FRONT_FACE_CLOCKWISE: return FrontFace::kClockwise;
            case VK_FRONT_FACE_COUNTER_CLOCKWISE:
            default:
                return FrontFace::kCounterClockwise;
        }
    }

    VkSampleCountFlagBits toVkSampleCount(SampleCountFlag count) noexcept
    {
        return static_cast<VkSampleCountFlagBits>(count);
    }

    SampleCountFlag fromVkSampleCount(VkSampleCountFlagBits count) noexcept
    {
        return static_cast<SampleCountFlag>(count);
    }

    VkColorComponentFlags toVkColorComponents(ColorComponent components) noexcept
    {
        VkColorComponentFlags flags = 0;
        const auto bits = static_cast<std::uint8_t>(components);
        if(bits & static_cast<std::uint8_t>(ColorComponent::kR)) flags |= VK_COLOR_COMPONENT_R_BIT;
        if(bits & static_cast<std::uint8_t>(ColorComponent::kG)) flags |= VK_COLOR_COMPONENT_G_BIT;
        if(bits & static_cast<std::uint8_t>(ColorComponent::kB)) flags |= VK_COLOR_COMPONENT_B_BIT;
        if(bits & static_cast<std::uint8_t>(ColorComponent::kA)) flags |= VK_COLOR_COMPONENT_A_BIT;
        return flags;
    }

    ColorComponent fromVkColorComponents(VkColorComponentFlags components) noexcept
    {
        std::uint8_t bits = 0;
        if(components & VK_COLOR_COMPONENT_R_BIT) bits |= static_cast<std::uint8_t>(ColorComponent::kR);
        if(components & VK_COLOR_COMPONENT_G_BIT) bits |= static_cast<std::uint8_t>(ColorComponent::kG);
        if(components & VK_COLOR_COMPONENT_B_BIT) bits |= static_cast<std::uint8_t>(ColorComponent::kB);
        if(components & VK_COLOR_COMPONENT_A_BIT) bits |= static_cast<std::uint8_t>(ColorComponent::kA);
        return static_cast<ColorComponent>(bits);
    }

    VkBlendFactor toVkBlendFactor(BlendFactor factor) noexcept
    {
        switch(factor)
        {
            case BlendFactor::kOne: return VK_BLEND_FACTOR_ONE;
            case BlendFactor::kSrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
            case BlendFactor::kOneMinusSrcColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
            case BlendFactor::kDstColor: return VK_BLEND_FACTOR_DST_COLOR;
            case BlendFactor::kOneMinusDstColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
            case BlendFactor::kSrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
            case BlendFactor::kOneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            case BlendFactor::kDstAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
            case BlendFactor::kOneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
            case BlendFactor::kConstantColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
            case BlendFactor::kOneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
            case BlendFactor::kConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
            case BlendFactor::kOneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
            case BlendFactor::kSrcAlphaSaturate: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
            case BlendFactor::kSrc1Color: return VK_BLEND_FACTOR_SRC1_COLOR;
            case BlendFactor::kOneMinusSrc1Color: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
            case BlendFactor::kSrc1Alpha: return VK_BLEND_FACTOR_SRC1_ALPHA;
            case BlendFactor::kOneMinusSrc1Alpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
            case BlendFactor::kZero:
            case BlendFactor::kCount:
            default:
                return VK_BLEND_FACTOR_ZERO;
        }
    }

    BlendFactor fromVkBlendFactor(VkBlendFactor factor) noexcept
    {
        switch(factor)
        {
            case VK_BLEND_FACTOR_ONE: return BlendFactor::kOne;
            case VK_BLEND_FACTOR_SRC_COLOR: return BlendFactor::kSrcColor;
            case VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR: return BlendFactor::kOneMinusSrcColor;
            case VK_BLEND_FACTOR_DST_COLOR: return BlendFactor::kDstColor;
            case VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR: return BlendFactor::kOneMinusDstColor;
            case VK_BLEND_FACTOR_SRC_ALPHA: return BlendFactor::kSrcAlpha;
            case VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA: return BlendFactor::kOneMinusSrcAlpha;
            case VK_BLEND_FACTOR_DST_ALPHA: return BlendFactor::kDstAlpha;
            case VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA: return BlendFactor::kOneMinusDstAlpha;
            case VK_BLEND_FACTOR_CONSTANT_COLOR: return BlendFactor::kConstantColor;
            case VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR: return BlendFactor::kOneMinusConstantColor;
            case VK_BLEND_FACTOR_CONSTANT_ALPHA: return BlendFactor::kConstantAlpha;
            case VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA: return BlendFactor::kOneMinusConstantAlpha;
            case VK_BLEND_FACTOR_SRC_ALPHA_SATURATE: return BlendFactor::kSrcAlphaSaturate;
            case VK_BLEND_FACTOR_SRC1_COLOR: return BlendFactor::kSrc1Color;
            case VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR: return BlendFactor::kOneMinusSrc1Color;
            case VK_BLEND_FACTOR_SRC1_ALPHA: return BlendFactor::kSrc1Alpha;
            case VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA: return BlendFactor::kOneMinusSrc1Alpha;
            case VK_BLEND_FACTOR_ZERO:
            default:
                return BlendFactor::kZero;
        }
    }

    VkBlendOp toVkBlendOp(BlendOp op) noexcept
    {
        switch(op)
        {
            case BlendOp::kSubtract: return VK_BLEND_OP_SUBTRACT;
            case BlendOp::kReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
            case BlendOp::kMin: return VK_BLEND_OP_MIN;
            case BlendOp::kMax: return VK_BLEND_OP_MAX;
            case BlendOp::kAdd:
            case BlendOp::kCount:
            default:
                return VK_BLEND_OP_ADD;
        }
    }

    BlendOp fromVkBlendOp(VkBlendOp op) noexcept
    {
        switch(op)
        {
            case VK_BLEND_OP_SUBTRACT: return BlendOp::kSubtract;
            case VK_BLEND_OP_REVERSE_SUBTRACT: return BlendOp::kReverseSubtract;
            case VK_BLEND_OP_MIN: return BlendOp::kMin;
            case VK_BLEND_OP_MAX: return BlendOp::kMax;
            case VK_BLEND_OP_ADD:
            default:
                return BlendOp::kAdd;
        }
    }

    VkLogicOp toVkLogicOp(LogicOp op) noexcept
    {
        switch(op)
        {
            case LogicOp::kClear: return VK_LOGIC_OP_CLEAR;
            case LogicOp::kAnd: return VK_LOGIC_OP_AND;
            case LogicOp::kAndReverse: return VK_LOGIC_OP_AND_REVERSE;
            case LogicOp::kAndInverted: return VK_LOGIC_OP_AND_INVERTED;
            case LogicOp::kNoOp: return VK_LOGIC_OP_NO_OP;
            case LogicOp::kXor: return VK_LOGIC_OP_XOR;
            case LogicOp::kOr: return VK_LOGIC_OP_OR;
            case LogicOp::kNor: return VK_LOGIC_OP_NOR;
            case LogicOp::kEquivalent: return VK_LOGIC_OP_EQUIVALENT;
            case LogicOp::kInvert: return VK_LOGIC_OP_INVERT;
            case LogicOp::kOrReverse: return VK_LOGIC_OP_OR_REVERSE;
            case LogicOp::kCopyInverted: return VK_LOGIC_OP_COPY_INVERTED;
            case LogicOp::kOrInverted: return VK_LOGIC_OP_OR_INVERTED;
            case LogicOp::kNand: return VK_LOGIC_OP_NAND;
            case LogicOp::kSet: return VK_LOGIC_OP_SET;
            case LogicOp::kCopy:
            case LogicOp::kCount:
            default:
                return VK_LOGIC_OP_COPY;
        }
    }

    LogicOp fromVkLogicOp(VkLogicOp op) noexcept
    {
        switch(op)
        {
            case VK_LOGIC_OP_CLEAR: return LogicOp::kClear;
            case VK_LOGIC_OP_AND: return LogicOp::kAnd;
            case VK_LOGIC_OP_AND_REVERSE: return LogicOp::kAndReverse;
            case VK_LOGIC_OP_AND_INVERTED: return LogicOp::kAndInverted;
            case VK_LOGIC_OP_NO_OP: return LogicOp::kNoOp;
            case VK_LOGIC_OP_XOR: return LogicOp::kXor;
            case VK_LOGIC_OP_OR: return LogicOp::kOr;
            case VK_LOGIC_OP_NOR: return LogicOp::kNor;
            case VK_LOGIC_OP_EQUIVALENT: return LogicOp::kEquivalent;
            case VK_LOGIC_OP_INVERT: return LogicOp::kInvert;
            case VK_LOGIC_OP_OR_REVERSE: return LogicOp::kOrReverse;
            case VK_LOGIC_OP_COPY_INVERTED: return LogicOp::kCopyInverted;
            case VK_LOGIC_OP_OR_INVERTED: return LogicOp::kOrInverted;
            case VK_LOGIC_OP_NAND: return LogicOp::kNand;
            case VK_LOGIC_OP_SET: return LogicOp::kSet;
            case VK_LOGIC_OP_COPY:
            default:
                return LogicOp::kCopy;
        }
    }

    VkAttachmentLoadOp toVkAttachmentLoadOp(AttachmentLoadOp op) noexcept
    {
        switch(op)
        {
            case AttachmentLoadOp::kClear: return VK_ATTACHMENT_LOAD_OP_CLEAR;
            case AttachmentLoadOp::kDontCare: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            case AttachmentLoadOp::kNone: return VK_ATTACHMENT_LOAD_OP_NONE;
            case AttachmentLoadOp::kLoad:
            case AttachmentLoadOp::kCount:
            default:
                return VK_ATTACHMENT_LOAD_OP_LOAD;
        }
    }

    AttachmentLoadOp fromVkAttachmentLoadOp(VkAttachmentLoadOp op) noexcept
    {
        switch(op)
        {
            case VK_ATTACHMENT_LOAD_OP_CLEAR: return AttachmentLoadOp::kClear;
            case VK_ATTACHMENT_LOAD_OP_DONT_CARE: return AttachmentLoadOp::kDontCare;
            case VK_ATTACHMENT_LOAD_OP_NONE: return AttachmentLoadOp::kNone;
            case VK_ATTACHMENT_LOAD_OP_LOAD:
            default:
                return AttachmentLoadOp::kLoad;
        }
    }

    VkAttachmentStoreOp toVkAttachmentStoreOp(AttachmentStoreOp op) noexcept
    {
        switch(op)
        {
            case AttachmentStoreOp::kDontCare: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
            case AttachmentStoreOp::kNone: return VK_ATTACHMENT_STORE_OP_NONE;
            case AttachmentStoreOp::kStore:
            case AttachmentStoreOp::kCount:
            default:
                return VK_ATTACHMENT_STORE_OP_STORE;
        }
    }

    AttachmentStoreOp fromVkAttachmentStoreOp(VkAttachmentStoreOp op) noexcept
    {
        switch(op)
        {
            case VK_ATTACHMENT_STORE_OP_DONT_CARE: return AttachmentStoreOp::kDontCare;
            case VK_ATTACHMENT_STORE_OP_NONE: return AttachmentStoreOp::kNone;
            case VK_ATTACHMENT_STORE_OP_STORE:
            default:
                return AttachmentStoreOp::kStore;
        }
    }

    VkImageLayout toVkImageLayout(ImageLayout layout) noexcept
    {
        switch(layout)
        {
            case ImageLayout::kGeneral: return VK_IMAGE_LAYOUT_GENERAL;
            case ImageLayout::kColorAttachment: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case ImageLayout::kDepthStencilAttachment: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case ImageLayout::kDepthStencilReadOnly: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            case ImageLayout::kShaderReadOnly: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case ImageLayout::kTransferSrc: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case ImageLayout::kTransferDst: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            case ImageLayout::kPresent: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            case ImageLayout::kDepthAttachment: return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
            case ImageLayout::kDepthReadOnly: return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
            case ImageLayout::kStencilAttachment: return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
            case ImageLayout::kStencilReadOnly: return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
            case ImageLayout::kUndefined:
            case ImageLayout::kCount:
            default:
                return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }

    ImageLayout fromVkImageLayout(VkImageLayout layout) noexcept
    {
        switch(layout)
        {
            case VK_IMAGE_LAYOUT_GENERAL: return ImageLayout::kGeneral;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: return ImageLayout::kColorAttachment;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return ImageLayout::kDepthStencilAttachment;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL: return ImageLayout::kDepthStencilReadOnly;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return ImageLayout::kShaderReadOnly;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: return ImageLayout::kTransferSrc;
            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: return ImageLayout::kTransferDst;
            case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: return ImageLayout::kPresent;
            case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL: return ImageLayout::kDepthAttachment;
            case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL: return ImageLayout::kDepthReadOnly;
            case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL: return ImageLayout::kStencilAttachment;
            case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL: return ImageLayout::kStencilReadOnly;
            case VK_IMAGE_LAYOUT_UNDEFINED:
            default:
                return ImageLayout::kUndefined;
        }
    }

    VkAccessFlags toVkAccessMask(ImageLayout layout) noexcept
    {
        switch(layout)
        {
            case ImageLayout::kGeneral:
                return VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;

            case ImageLayout::kColorAttachment:
                return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            case ImageLayout::kDepthStencilAttachment:
            case ImageLayout::kDepthAttachment:
            case ImageLayout::kStencilAttachment:
                return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                       VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            case ImageLayout::kDepthStencilReadOnly:
            case ImageLayout::kDepthReadOnly:
            case ImageLayout::kStencilReadOnly:
                return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_SHADER_READ_BIT;

            case ImageLayout::kShaderReadOnly:
                return VK_ACCESS_SHADER_READ_BIT;

            case ImageLayout::kTransferSrc:
                return VK_ACCESS_TRANSFER_READ_BIT;

            case ImageLayout::kTransferDst:
                return VK_ACCESS_TRANSFER_WRITE_BIT;

            case ImageLayout::kPresent:
            case ImageLayout::kUndefined:
            case ImageLayout::kCount:
            default:
                return 0;
        }
    }

    VkPipelineStageFlags toVkPipelineStageFlags(ImageLayout layout) noexcept
    {
        switch(layout)
        {
            case ImageLayout::kGeneral:
                return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

            case ImageLayout::kColorAttachment:
                return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

            case ImageLayout::kDepthStencilAttachment:
            case ImageLayout::kDepthStencilReadOnly:
            case ImageLayout::kDepthAttachment:
            case ImageLayout::kDepthReadOnly:
            case ImageLayout::kStencilAttachment:
            case ImageLayout::kStencilReadOnly:
                return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                       VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

            case ImageLayout::kShaderReadOnly:
                // Core stages only. MESH / RAY_TRACING bits require enabled extensions
                // and must not appear in stage masks otherwise.
                return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                       VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT |
                       VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT |
                       VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT |
                       VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT |
                       VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

            case ImageLayout::kTransferSrc:
            case ImageLayout::kTransferDst:
                return VK_PIPELINE_STAGE_TRANSFER_BIT;

            case ImageLayout::kPresent:
                return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

            case ImageLayout::kUndefined:
            case ImageLayout::kCount:
            default:
                return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        }
    }

    VkImageAspectFlags toVkImageAspect(ImageAspect aspect) noexcept
    {
        VkImageAspectFlags flags = 0;
        const auto bits = static_cast<std::uint8_t>(aspect);
        if(bits & static_cast<std::uint8_t>(ImageAspect::kColor)) flags |= VK_IMAGE_ASPECT_COLOR_BIT;
        if(bits & static_cast<std::uint8_t>(ImageAspect::kDepth)) flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
        if(bits & static_cast<std::uint8_t>(ImageAspect::kStencil)) flags |= VK_IMAGE_ASPECT_STENCIL_BIT;
        if(bits & static_cast<std::uint8_t>(ImageAspect::kMetadata)) flags |= VK_IMAGE_ASPECT_METADATA_BIT;
        if(bits & static_cast<std::uint8_t>(ImageAspect::kPlane0)) flags |= VK_IMAGE_ASPECT_PLANE_0_BIT;
        if(bits & static_cast<std::uint8_t>(ImageAspect::kPlane1)) flags |= VK_IMAGE_ASPECT_PLANE_1_BIT;
        if(bits & static_cast<std::uint8_t>(ImageAspect::kPlane2)) flags |= VK_IMAGE_ASPECT_PLANE_2_BIT;
        return flags;
    }

    ImageAspect fromVkImageAspect(VkImageAspectFlags aspect) noexcept
    {
        std::uint8_t bits = 0;
        if(aspect & VK_IMAGE_ASPECT_COLOR_BIT) bits |= static_cast<std::uint8_t>(ImageAspect::kColor);
        if(aspect & VK_IMAGE_ASPECT_DEPTH_BIT) bits |= static_cast<std::uint8_t>(ImageAspect::kDepth);
        if(aspect & VK_IMAGE_ASPECT_STENCIL_BIT) bits |= static_cast<std::uint8_t>(ImageAspect::kStencil);
        if(aspect & VK_IMAGE_ASPECT_METADATA_BIT) bits |= static_cast<std::uint8_t>(ImageAspect::kMetadata);
        if(aspect & VK_IMAGE_ASPECT_PLANE_0_BIT) bits |= static_cast<std::uint8_t>(ImageAspect::kPlane0);
        if(aspect & VK_IMAGE_ASPECT_PLANE_1_BIT) bits |= static_cast<std::uint8_t>(ImageAspect::kPlane1);
        if(aspect & VK_IMAGE_ASPECT_PLANE_2_BIT) bits |= static_cast<std::uint8_t>(ImageAspect::kPlane2);
        return static_cast<ImageAspect>(bits);
    }

    VkComponentSwizzle toVkComponentSwizzle(ComponentSwizzle swizzle) noexcept
    {
        switch(swizzle)
        {
            case ComponentSwizzle::kZero: return VK_COMPONENT_SWIZZLE_ZERO;
            case ComponentSwizzle::kOne: return VK_COMPONENT_SWIZZLE_ONE;
            case ComponentSwizzle::kR: return VK_COMPONENT_SWIZZLE_R;
            case ComponentSwizzle::kG: return VK_COMPONENT_SWIZZLE_G;
            case ComponentSwizzle::kB: return VK_COMPONENT_SWIZZLE_B;
            case ComponentSwizzle::kA: return VK_COMPONENT_SWIZZLE_A;
            case ComponentSwizzle::kIdentity:
            case ComponentSwizzle::kCount:
            default:
                return VK_COMPONENT_SWIZZLE_IDENTITY;
        }
    }

    ComponentSwizzle fromVkComponentSwizzle(VkComponentSwizzle swizzle) noexcept
    {
        switch(swizzle)
        {
            case VK_COMPONENT_SWIZZLE_ZERO: return ComponentSwizzle::kZero;
            case VK_COMPONENT_SWIZZLE_ONE: return ComponentSwizzle::kOne;
            case VK_COMPONENT_SWIZZLE_R: return ComponentSwizzle::kR;
            case VK_COMPONENT_SWIZZLE_G: return ComponentSwizzle::kG;
            case VK_COMPONENT_SWIZZLE_B: return ComponentSwizzle::kB;
            case VK_COMPONENT_SWIZZLE_A: return ComponentSwizzle::kA;
            case VK_COMPONENT_SWIZZLE_IDENTITY:
            default:
                return ComponentSwizzle::kIdentity;
        }
    }

    VkComponentMapping toVkComponentMapping(const ComponentMapping& mapping) noexcept
    {
        return {
            toVkComponentSwizzle(mapping.r),
            toVkComponentSwizzle(mapping.g),
            toVkComponentSwizzle(mapping.b),
            toVkComponentSwizzle(mapping.a)
        };
    }

    ComponentMapping fromVkComponentMapping(const VkComponentMapping& mapping) noexcept
    {
        return {
            fromVkComponentSwizzle(mapping.r),
            fromVkComponentSwizzle(mapping.g),
            fromVkComponentSwizzle(mapping.b),
            fromVkComponentSwizzle(mapping.a)
        };
    }

    VkVertexInputRate toVkInputRate(InputRate rate) noexcept
    {
        switch(rate)
        {
            case InputRate::kPerVertex: return VK_VERTEX_INPUT_RATE_VERTEX;
            case InputRate::kPerInstance: return VK_VERTEX_INPUT_RATE_INSTANCE;
            default: return VK_VERTEX_INPUT_RATE_VERTEX;
        }
    }

    InputRate fromVkInputRate(VkVertexInputRate rate) noexcept
    {
        switch(rate)
        {
            case VK_VERTEX_INPUT_RATE_INSTANCE: return InputRate::kPerInstance;
            case VK_VERTEX_INPUT_RATE_VERTEX:
            case VK_VERTEX_INPUT_RATE_MAX_ENUM:
            default:
                return InputRate::kPerVertex;
        }
    }

    VkIndexType toVkIndexType(IndexType type) noexcept
    {
        switch(type)
        {
            case IndexType::kUint16: return VK_INDEX_TYPE_UINT16;
            case IndexType::kUint32: return VK_INDEX_TYPE_UINT32;
            case IndexType::kUint8:
#if defined(VK_INDEX_TYPE_UINT8_KHR)
                return VK_INDEX_TYPE_UINT8_KHR;
#elif defined(VK_INDEX_TYPE_UINT8_EXT)
                return VK_INDEX_TYPE_UINT8_EXT;
#else
                return VK_INDEX_TYPE_UINT16;
#endif
            default: return VK_INDEX_TYPE_UINT16;
        }
    }

    IndexType fromVkIndexType(VkIndexType type) noexcept
    {
        switch(type)
        {
            case VK_INDEX_TYPE_UINT32: return IndexType::kUint32;
            case VK_INDEX_TYPE_UINT16: return IndexType::kUint16;
            default:
#if defined(VK_INDEX_TYPE_UINT8_KHR)
                if(type == VK_INDEX_TYPE_UINT8_KHR) return IndexType::kUint8;
#endif
#if defined(VK_INDEX_TYPE_UINT8_EXT)
                if(type == VK_INDEX_TYPE_UINT8_EXT) return IndexType::kUint8;
#endif
                return IndexType::kUint16;
        }
    }

    VkDescriptorType toVkDescriptorType(DescriptorType type) noexcept
    {
        switch(type)
        {
            case DescriptorType::kSampler: return VK_DESCRIPTOR_TYPE_SAMPLER;
            case DescriptorType::kCombinedImageSampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            case DescriptorType::kSampledImage: return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            case DescriptorType::kStorageImage: return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            case DescriptorType::kUniformTexelBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
            case DescriptorType::kStorageTexelBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
            case DescriptorType::kUniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case DescriptorType::kStorageBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            case DescriptorType::kUniformBufferDynamic: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            case DescriptorType::kStorageBufferDynamic: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            case DescriptorType::kInputAttachment: return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
            default: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        }
    }

    DescriptorType fromVkDescriptorType(VkDescriptorType type) noexcept
    {
        switch(type)
        {
            case VK_DESCRIPTOR_TYPE_SAMPLER: return DescriptorType::kSampler;
            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return DescriptorType::kCombinedImageSampler;
            case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE: return DescriptorType::kSampledImage;
            case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE: return DescriptorType::kStorageImage;
            case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: return DescriptorType::kUniformTexelBuffer;
            case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: return DescriptorType::kStorageTexelBuffer;
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: return DescriptorType::kStorageBuffer;
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: return DescriptorType::kUniformBufferDynamic;
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return DescriptorType::kStorageBufferDynamic;
            case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: return DescriptorType::kInputAttachment;
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            default:
                return DescriptorType::kUniformBuffer;
        }
    }

    VkShaderStageFlags toVkShaderStageFlags(ShaderStageType stages) noexcept
    {
        VkShaderStageFlags flags = 0;
        const auto bits = static_cast<std::uint32_t>(stages);
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kVertex))
            flags |= VK_SHADER_STAGE_VERTEX_BIT;
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kFragment))
            flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kCompute))
            flags |= VK_SHADER_STAGE_COMPUTE_BIT;
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kTessellationControl))
            flags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kTessellationEval))
            flags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        if(bits & static_cast<std::uint32_t>(ShaderStageType::kGeometry))
            flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
        return flags;
    }

    ShaderStageType fromVkShaderStageFlags(VkShaderStageFlags stages) noexcept
    {
        std::uint32_t bits = 0;
        if(stages & VK_SHADER_STAGE_VERTEX_BIT)
            bits |= static_cast<std::uint32_t>(ShaderStageType::kVertex);
        if(stages & VK_SHADER_STAGE_FRAGMENT_BIT)
            bits |= static_cast<std::uint32_t>(ShaderStageType::kFragment);
        if(stages & VK_SHADER_STAGE_COMPUTE_BIT)
            bits |= static_cast<std::uint32_t>(ShaderStageType::kCompute);
        if(stages & VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT)
            bits |= static_cast<std::uint32_t>(ShaderStageType::kTessellationControl);
        if(stages & VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT)
            bits |= static_cast<std::uint32_t>(ShaderStageType::kTessellationEval);
        if(stages & VK_SHADER_STAGE_GEOMETRY_BIT)
            bits |= static_cast<std::uint32_t>(ShaderStageType::kGeometry);
        return static_cast<ShaderStageType>(bits);
    }

    VkBufferUsageFlags toVkBufferUsage(BufferType type) noexcept
    {
        VkBufferUsageFlags flags = 0;
        const auto bits = static_cast<std::uint16_t>(type);
        if(bits & static_cast<std::uint16_t>(BufferType::kVertex)) flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        if(bits & static_cast<std::uint16_t>(BufferType::kIndex)) flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        if(bits & static_cast<std::uint16_t>(BufferType::kUniform)) flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        if(bits & static_cast<std::uint16_t>(BufferType::kStorage)) flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        if(bits & static_cast<std::uint16_t>(BufferType::kIndirect)) flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
        if(bits & static_cast<std::uint16_t>(BufferType::kTransferSrc)) flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        if(bits & static_cast<std::uint16_t>(BufferType::kTransferDst)) flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        return flags;
    }

    BufferType fromVkBufferUsage(VkBufferUsageFlags usage) noexcept
    {
        std::uint16_t bits = 0;
        if(usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) bits |= static_cast<std::uint16_t>(BufferType::kVertex);
        if(usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT) bits |= static_cast<std::uint16_t>(BufferType::kIndex);
        if(usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) bits |= static_cast<std::uint16_t>(BufferType::kUniform);
        if(usage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) bits |= static_cast<std::uint16_t>(BufferType::kStorage);
        if(usage & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT) bits |= static_cast<std::uint16_t>(BufferType::kIndirect);
        if(usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) bits |= static_cast<std::uint16_t>(BufferType::kTransferSrc);
        if(usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT) bits |= static_cast<std::uint16_t>(BufferType::kTransferDst);
        return static_cast<BufferType>(bits);
    }

    VkQueueFlags toVkQueueFlags(QueueFamilyType type) noexcept
    {
        switch(type)
        {
            case QueueFamilyType::kGraphics: return VK_QUEUE_GRAPHICS_BIT;
            case QueueFamilyType::kCompute: return VK_QUEUE_COMPUTE_BIT;
            case QueueFamilyType::kTransfer: return VK_QUEUE_TRANSFER_BIT;
            case QueueFamilyType::kSparseBinding: return VK_QUEUE_SPARSE_BINDING_BIT;
#if defined(VK_QUEUE_VIDEO_ENCODE_BIT_KHR)
            case QueueFamilyType::kVideoEncode: return VK_QUEUE_VIDEO_ENCODE_BIT_KHR;
#endif
#if defined(VK_QUEUE_VIDEO_DECODE_BIT_KHR)
            case QueueFamilyType::kVideoDecode: return VK_QUEUE_VIDEO_DECODE_BIT_KHR;
#endif
#if defined(VK_QUEUE_OPTICAL_FLOW_BIT_NV)
            case QueueFamilyType::kOpticalFlow: return VK_QUEUE_OPTICAL_FLOW_BIT_NV;
#endif
            case QueueFamilyType::kPresent:
            case QueueFamilyType::kCount:
            default:
                return 0;
        }
    }

    QueueFamilyType fromVkQueueFlags(VkQueueFlags flags) noexcept
    {
        if(flags & VK_QUEUE_GRAPHICS_BIT) return QueueFamilyType::kGraphics;
        if(flags & VK_QUEUE_COMPUTE_BIT) return QueueFamilyType::kCompute;
        if(flags & VK_QUEUE_TRANSFER_BIT) return QueueFamilyType::kTransfer;
        if(flags & VK_QUEUE_SPARSE_BINDING_BIT) return QueueFamilyType::kSparseBinding;
#if defined(VK_QUEUE_VIDEO_ENCODE_BIT_KHR)
        if(flags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) return QueueFamilyType::kVideoEncode;
#endif
#if defined(VK_QUEUE_VIDEO_DECODE_BIT_KHR)
        if(flags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) return QueueFamilyType::kVideoDecode;
#endif
#if defined(VK_QUEUE_OPTICAL_FLOW_BIT_NV)
        if(flags & VK_QUEUE_OPTICAL_FLOW_BIT_NV) return QueueFamilyType::kOpticalFlow;
#endif
        return QueueFamilyType::kGraphics;
    }

    VkPipelineBindPoint toVkPipelineBindPoint(PipelineBindPoint bind_point) noexcept
    {
        switch(bind_point)
        {
            case PipelineBindPoint::kCompute: return VK_PIPELINE_BIND_POINT_COMPUTE;
            case PipelineBindPoint::kRayTracing:
#if defined(VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR)
                return VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
#elif defined(VK_PIPELINE_BIND_POINT_RAY_TRACING_NV)
                return VK_PIPELINE_BIND_POINT_RAY_TRACING_NV;
#else
                return VK_PIPELINE_BIND_POINT_GRAPHICS;
#endif
            case PipelineBindPoint::kGraphics:
            default:
                return VK_PIPELINE_BIND_POINT_GRAPHICS;
        }
    }

    PipelineBindPoint fromVkPipelineBindPoint(VkPipelineBindPoint bind_point) noexcept
    {
        switch(bind_point)
        {
            case VK_PIPELINE_BIND_POINT_COMPUTE: return PipelineBindPoint::kCompute;
            case VK_PIPELINE_BIND_POINT_GRAPHICS: return PipelineBindPoint::kGraphics;
            default:
#if defined(VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR)
                if(bind_point == VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR) return PipelineBindPoint::kRayTracing;
#endif
#if defined(VK_PIPELINE_BIND_POINT_RAY_TRACING_NV)
                if(bind_point == VK_PIPELINE_BIND_POINT_RAY_TRACING_NV) return PipelineBindPoint::kRayTracing;
#endif
                return PipelineBindPoint::kGraphics;
        }
    }

    VkFilter toVkFilter(FilterType filter) noexcept
    {
        switch(filter)
        {
            case FilterType::kNearest: return VK_FILTER_NEAREST;
            case FilterType::kLinear:
            default:
                return VK_FILTER_LINEAR;
        }
    }

    FilterType fromVkFilter(VkFilter filter) noexcept
    {
        switch(filter)
        {
            case VK_FILTER_NEAREST: return FilterType::kNearest;
            case VK_FILTER_LINEAR:
            default:
                return FilterType::kLinear;
        }
    }

    VkSamplerMipmapMode toVkMipmapMode(MipmapMode mode) noexcept
    {
        switch(mode)
        {
            case MipmapMode::kNearest: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
            case MipmapMode::kLinear:
            default:
                return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        }
    }

    MipmapMode fromVkMipmapMode(VkSamplerMipmapMode mode) noexcept
    {
        switch(mode)
        {
            case VK_SAMPLER_MIPMAP_MODE_NEAREST: return MipmapMode::kNearest;
            case VK_SAMPLER_MIPMAP_MODE_LINEAR:
            default:
                return MipmapMode::kLinear;
        }
    }

    VkSamplerAddressMode toVkAddressMode(RepeatMode mode) noexcept
    {
        switch(mode)
        {
            case RepeatMode::kMirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            case RepeatMode::kClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            case RepeatMode::kClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            case RepeatMode::kMirrorClampToEdge: return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
            case RepeatMode::kRepeat:
            default:
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        }
    }

    RepeatMode fromVkAddressMode(VkSamplerAddressMode mode) noexcept
    {
        switch(mode)
        {
            case VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT: return RepeatMode::kMirroredRepeat;
            case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE: return RepeatMode::kClampToEdge;
            case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER: return RepeatMode::kClampToBorder;
            case VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE: return RepeatMode::kMirrorClampToEdge;
            case VK_SAMPLER_ADDRESS_MODE_REPEAT:
            default:
                return RepeatMode::kRepeat;
        }
    }

    VkCompareOp toVkCompareOp(CompareOp op) noexcept
    {
        switch(op)
        {
            case CompareOp::kNever: return VK_COMPARE_OP_NEVER;
            case CompareOp::kLess: return VK_COMPARE_OP_LESS;
            case CompareOp::kEqual: return VK_COMPARE_OP_EQUAL;
            case CompareOp::kLessOrEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
            case CompareOp::kGreater: return VK_COMPARE_OP_GREATER;
            case CompareOp::kNotEqual: return VK_COMPARE_OP_NOT_EQUAL;
            case CompareOp::kGreaterOrEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
            case CompareOp::kAlways:
            default:
                return VK_COMPARE_OP_ALWAYS;
        }
    }

    CompareOp fromVkCompareOp(VkCompareOp op) noexcept
    {
        switch(op)
        {
            case VK_COMPARE_OP_NEVER: return CompareOp::kNever;
            case VK_COMPARE_OP_LESS: return CompareOp::kLess;
            case VK_COMPARE_OP_EQUAL: return CompareOp::kEqual;
            case VK_COMPARE_OP_LESS_OR_EQUAL: return CompareOp::kLessOrEqual;
            case VK_COMPARE_OP_GREATER: return CompareOp::kGreater;
            case VK_COMPARE_OP_NOT_EQUAL: return CompareOp::kNotEqual;
            case VK_COMPARE_OP_GREATER_OR_EQUAL: return CompareOp::kGreaterOrEqual;
            case VK_COMPARE_OP_ALWAYS:
            default:
                return CompareOp::kAlways;
        }
    }
}
