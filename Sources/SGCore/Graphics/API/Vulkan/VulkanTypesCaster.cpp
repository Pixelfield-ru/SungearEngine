//
// Created by 8bitniksis on 17.08.2026.
//

#include "VulkanTypesCaster.h"

VkFormat SGCore::VulkanTypesCaster::sggInternalFormatToVk(SGGColorInternalFormat format) noexcept
{
    switch(format)
    {
        case SGGColorInternalFormat::SGG_R8: return VK_FORMAT_R8_UNORM;
        case SGGColorInternalFormat::SGG_R8_SIGNED_NORMALIZED: return VK_FORMAT_R8_SNORM;
        case SGGColorInternalFormat::SGG_R16: return VK_FORMAT_R16_UNORM;
        case SGGColorInternalFormat::SGG_R16_SIGNED_NORMALIZED: return VK_FORMAT_R16_SNORM;
        case SGGColorInternalFormat::SGG_RG8: return VK_FORMAT_R8G8_UNORM;
        case SGGColorInternalFormat::SGG_RG8_SIGNED_NORMALIZED: return VK_FORMAT_R8G8_SNORM;
        case SGGColorInternalFormat::SGG_RG16: return VK_FORMAT_R16G16_UNORM;
        case SGGColorInternalFormat::SGG_RG16_SIGNED_NORMALIZED: return VK_FORMAT_R16G16_SNORM;
        case SGGColorInternalFormat::SGG_R3_G3_B2: return VK_FORMAT_R8G8B8A8_UNORM;
        case SGGColorInternalFormat::SGG_RGB4: return VK_FORMAT_R8G8B8A8_UNORM;
        case SGGColorInternalFormat::SGG_RGB5: return VK_FORMAT_R8G8B8A8_UNORM;
        case SGGColorInternalFormat::SGG_RGB8: return VK_FORMAT_R8G8B8A8_UNORM;
        case SGGColorInternalFormat::SGG_RGB8_SIGNED_NORMALIZED: return VK_FORMAT_R8G8B8A8_SNORM;
        case SGGColorInternalFormat::SGG_RGB10: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        case SGGColorInternalFormat::SGG_RGB12: return VK_FORMAT_R16G16B16A16_UNORM;
        case SGGColorInternalFormat::SGG_RGB16_SIGNED_NORMALIZED: return VK_FORMAT_R16G16B16A16_SNORM;
        case SGGColorInternalFormat::SGG_RGBA2: return VK_FORMAT_R8G8B8A8_UNORM;
        case SGGColorInternalFormat::SGG_RGBA4: return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
        case SGGColorInternalFormat::SGG_RGB5_A1: return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
        case SGGColorInternalFormat::SGG_RGBA8: return VK_FORMAT_R8G8B8A8_UNORM;
        case SGGColorInternalFormat::SGG_RGBA8_SIGNED_NORMALIZED: return VK_FORMAT_R8G8B8A8_SNORM;
        case SGGColorInternalFormat::SGG_RGB10_A2: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        case SGGColorInternalFormat::SGG_RGB10_A2_UNSIGNED_INT: return VK_FORMAT_A2B10G10R10_UINT_PACK32;
        case SGGColorInternalFormat::SGG_RGBA12: return VK_FORMAT_R16G16B16A16_UNORM;
        case SGGColorInternalFormat::SGG_RGBA16: return VK_FORMAT_R16G16B16A16_UNORM;
        case SGGColorInternalFormat::SGG_SRGB8: return VK_FORMAT_R8G8B8A8_SRGB;
        case SGGColorInternalFormat::SGG_SRGB8_ALPHA8: return VK_FORMAT_R8G8B8A8_SRGB;
        case SGGColorInternalFormat::SGG_R16_FLOAT: return VK_FORMAT_R16_SFLOAT;
        case SGGColorInternalFormat::SGG_RG16_FLOAT: return VK_FORMAT_R16G16_SFLOAT;
        case SGGColorInternalFormat::SGG_RGB16_FLOAT: return VK_FORMAT_R16G16B16A16_SFLOAT;
        case SGGColorInternalFormat::SGG_RGBA16_FLOAT: return VK_FORMAT_R16G16B16A16_SFLOAT;
        case SGGColorInternalFormat::SGG_R32_FLOAT: return VK_FORMAT_R32_SFLOAT;
        case SGGColorInternalFormat::SGG_RG32_FLOAT: return VK_FORMAT_R32G32_SFLOAT;
        case SGGColorInternalFormat::SGG_RGB32_FLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
        case SGGColorInternalFormat::SGG_RGBA32_FLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
        case SGGColorInternalFormat::SGG_R11_G11_B10_FLOAT: return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
        case SGGColorInternalFormat::SGG_RGB9_E5: return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
        case SGGColorInternalFormat::SGG_R8_INT: return VK_FORMAT_R8_SINT;
        case SGGColorInternalFormat::SGG_R8_UNSIGNED_INT: return VK_FORMAT_R8_UINT;
        case SGGColorInternalFormat::SGG_R16_INT: return VK_FORMAT_R16_SINT;
        case SGGColorInternalFormat::SGG_R16_UNSIGNED_INT: return VK_FORMAT_R16_UINT;
        case SGGColorInternalFormat::SGG_R32_INT: return VK_FORMAT_R32_SINT;
        case SGGColorInternalFormat::SGG_R32_UNSIGNED_INT: return VK_FORMAT_R32_UINT;
        case SGGColorInternalFormat::SGG_RG8_INT: return VK_FORMAT_R8G8_SINT;
        case SGGColorInternalFormat::SGG_RG8_UNSIGNED_INT: return VK_FORMAT_R8G8_UINT;
        case SGGColorInternalFormat::SGG_RG16_INT: return VK_FORMAT_R16G16_SINT;
        case SGGColorInternalFormat::SGG_RG16_UNSIGNED_INT: return VK_FORMAT_R16G16_UINT;
        case SGGColorInternalFormat::SGG_RG32_INT: return VK_FORMAT_R32G32_SINT;
        case SGGColorInternalFormat::SGG_RG32_UNSIGNED_INT: return VK_FORMAT_R32G32_UINT;
        case SGGColorInternalFormat::SGG_RGB8_INT: return VK_FORMAT_R8G8B8A8_SINT;
        case SGGColorInternalFormat::SGG_RGB8_UNSIGNED_INT: return VK_FORMAT_R8G8B8A8_UINT;
        case SGGColorInternalFormat::SGG_RGB16_INT: return VK_FORMAT_R16G16B16A16_SINT;
        case SGGColorInternalFormat::SGG_RGB16_UNSIGNED_INT: return VK_FORMAT_R16G16B16A16_UINT;
        case SGGColorInternalFormat::SGG_RGB32_INT: return VK_FORMAT_R32G32B32A32_SINT;
        case SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT: return VK_FORMAT_R32G32B32A32_UINT;
        case SGGColorInternalFormat::SGG_RGBA8_INT: return VK_FORMAT_R8G8B8A8_SINT;
        case SGGColorInternalFormat::SGG_RGBA8_UNSIGNED_INT: return VK_FORMAT_R8G8B8A8_UINT;
        case SGGColorInternalFormat::SGG_RGBA16_INT: return VK_FORMAT_R16G16B16A16_SINT;
        case SGGColorInternalFormat::SGG_RGBA16_UNSIGNED_INT: return VK_FORMAT_R16G16B16A16_UINT;
        case SGGColorInternalFormat::SGG_RGBA32_INT: return VK_FORMAT_R32G32B32A32_SINT;
        case SGGColorInternalFormat::SGG_RGBA32_UNSIGNED_INT: return VK_FORMAT_R32G32B32A32_UINT;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT16: return VK_FORMAT_D16_UNORM;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT24: return VK_FORMAT_X8_D24_UNORM_PACK32;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32: return VK_FORMAT_D32_SFLOAT;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F: return VK_FORMAT_D32_SFLOAT;
        case SGGColorInternalFormat::SGG_DEPTH24_STENCIL8: return VK_FORMAT_D24_UNORM_S8_UINT;
        case SGGColorInternalFormat::SGG_DEPTH32F_STENCIL8: return VK_FORMAT_D32_SFLOAT_S8_UINT;
        case SGGColorInternalFormat::SGG_STENCIL_INDEX8: return VK_FORMAT_S8_UINT;
        default: return VK_FORMAT_R8G8B8A8_UNORM;
    }
}

VkFormat SGCore::VulkanTypesCaster::sggInternalFormatToVkTexel(SGGColorInternalFormat format) noexcept
{
    switch(format)
    {
        case SGGColorInternalFormat::SGG_RGB8: return VK_FORMAT_R8G8B8_UNORM;
        case SGGColorInternalFormat::SGG_RGB16_FLOAT: return VK_FORMAT_R16G16B16_SFLOAT;
        case SGGColorInternalFormat::SGG_RGB32_FLOAT: return VK_FORMAT_R32G32B32_SFLOAT;
        case SGGColorInternalFormat::SGG_RGB8_INT: return VK_FORMAT_R8G8B8_SINT;
        case SGGColorInternalFormat::SGG_RGB8_UNSIGNED_INT: return VK_FORMAT_R8G8B8_UINT;
        case SGGColorInternalFormat::SGG_RGB16_INT: return VK_FORMAT_R16G16B16_SINT;
        case SGGColorInternalFormat::SGG_RGB16_UNSIGNED_INT: return VK_FORMAT_R16G16B16_UINT;
        case SGGColorInternalFormat::SGG_RGB32_INT: return VK_FORMAT_R32G32B32_SINT;
        case SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT: return VK_FORMAT_R32G32B32_UINT;
        default: return sggInternalFormatToVk(format);
    }
}

bool SGCore::VulkanTypesCaster::needsAlphaExpansion(SGGColorInternalFormat format) noexcept
{
    switch(format)
    {
        case SGGColorInternalFormat::SGG_R3_G3_B2:
        case SGGColorInternalFormat::SGG_RGB4:
        case SGGColorInternalFormat::SGG_RGB5:
        case SGGColorInternalFormat::SGG_RGB8:
        case SGGColorInternalFormat::SGG_RGB8_SIGNED_NORMALIZED:
        case SGGColorInternalFormat::SGG_RGB12:
        case SGGColorInternalFormat::SGG_RGB16_SIGNED_NORMALIZED:
        case SGGColorInternalFormat::SGG_SRGB8:
        case SGGColorInternalFormat::SGG_RGB16_FLOAT:
        case SGGColorInternalFormat::SGG_RGB32_FLOAT:
        case SGGColorInternalFormat::SGG_RGB8_INT:
        case SGGColorInternalFormat::SGG_RGB8_UNSIGNED_INT:
        case SGGColorInternalFormat::SGG_RGB16_INT:
        case SGGColorInternalFormat::SGG_RGB16_UNSIGNED_INT:
        case SGGColorInternalFormat::SGG_RGB32_INT:
        case SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT:
            return true;
        default:
            return false;
    }
}

std::uint32_t SGCore::VulkanTypesCaster::formatTexelSize(VkFormat format) noexcept
{
    switch(format)
    {
        case VK_FORMAT_R8_UNORM: case VK_FORMAT_R8_SNORM: case VK_FORMAT_R8_UINT: case VK_FORMAT_R8_SINT:
        case VK_FORMAT_S8_UINT:
            return 1;
        case VK_FORMAT_R8G8_UNORM: case VK_FORMAT_R8G8_SNORM: case VK_FORMAT_R8G8_UINT: case VK_FORMAT_R8G8_SINT:
        case VK_FORMAT_R16_UNORM: case VK_FORMAT_R16_SNORM: case VK_FORMAT_R16_UINT: case VK_FORMAT_R16_SINT:
        case VK_FORMAT_R16_SFLOAT: case VK_FORMAT_R4G4B4A4_UNORM_PACK16: case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
        case VK_FORMAT_D16_UNORM:
            return 2;
        case VK_FORMAT_R8G8B8_UNORM: case VK_FORMAT_R8G8B8_SNORM: case VK_FORMAT_R8G8B8_UINT: case VK_FORMAT_R8G8B8_SINT:
            return 3;
        case VK_FORMAT_R8G8B8A8_UNORM: case VK_FORMAT_R8G8B8A8_SNORM: case VK_FORMAT_R8G8B8A8_UINT:
        case VK_FORMAT_R8G8B8A8_SINT: case VK_FORMAT_R8G8B8A8_SRGB:
        case VK_FORMAT_B8G8R8A8_UNORM: case VK_FORMAT_B8G8R8A8_SRGB:
        case VK_FORMAT_R16G16_UNORM: case VK_FORMAT_R16G16_SNORM: case VK_FORMAT_R16G16_UINT:
        case VK_FORMAT_R16G16_SINT: case VK_FORMAT_R16G16_SFLOAT:
        case VK_FORMAT_R32_UINT: case VK_FORMAT_R32_SINT: case VK_FORMAT_R32_SFLOAT:
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32: case VK_FORMAT_A2B10G10R10_UINT_PACK32:
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32: case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
        case VK_FORMAT_D32_SFLOAT: case VK_FORMAT_X8_D24_UNORM_PACK32: case VK_FORMAT_D24_UNORM_S8_UINT:
            return 4;
        case VK_FORMAT_R16G16B16_UNORM: case VK_FORMAT_R16G16B16_SNORM: case VK_FORMAT_R16G16B16_UINT:
        case VK_FORMAT_R16G16B16_SINT: case VK_FORMAT_R16G16B16_SFLOAT:
            return 6;
        case VK_FORMAT_R16G16B16A16_UNORM: case VK_FORMAT_R16G16B16A16_SNORM: case VK_FORMAT_R16G16B16A16_UINT:
        case VK_FORMAT_R16G16B16A16_SINT: case VK_FORMAT_R16G16B16A16_SFLOAT:
        case VK_FORMAT_R32G32_UINT: case VK_FORMAT_R32G32_SINT: case VK_FORMAT_R32G32_SFLOAT:
            return 8;
        case VK_FORMAT_R32G32B32_UINT: case VK_FORMAT_R32G32B32_SINT: case VK_FORMAT_R32G32B32_SFLOAT:
            return 12;
        case VK_FORMAT_R32G32B32A32_UINT: case VK_FORMAT_R32G32B32A32_SINT: case VK_FORMAT_R32G32B32A32_SFLOAT:
            return 16;
        default:
            return 0;
    }
}

SGCore::ChannelLayout SGCore::VulkanTypesCaster::formatLayout(VkFormat format) noexcept
{
    using Kind = ChannelKind;

    switch(format)
    {
        case VK_FORMAT_R8_UNORM: return { Kind::UNORM, 1, 1, false };
        case VK_FORMAT_R8_SNORM: return { Kind::SNORM, 1, 1, false };
        case VK_FORMAT_R8_UINT: return { Kind::UINT, 1, 1, false };
        case VK_FORMAT_R8_SINT: return { Kind::SINT, 1, 1, false };

        case VK_FORMAT_R8G8_UNORM: return { Kind::UNORM, 2, 1, false };
        case VK_FORMAT_R8G8_SNORM: return { Kind::SNORM, 2, 1, false };
        case VK_FORMAT_R8G8_UINT: return { Kind::UINT, 2, 1, false };
        case VK_FORMAT_R8G8_SINT: return { Kind::SINT, 2, 1, false };

        case VK_FORMAT_R8G8B8_UNORM: return { Kind::UNORM, 3, 1, false };
        case VK_FORMAT_R8G8B8_SNORM: return { Kind::SNORM, 3, 1, false };
        case VK_FORMAT_R8G8B8_UINT: return { Kind::UINT, 3, 1, false };
        case VK_FORMAT_R8G8B8_SINT: return { Kind::SINT, 3, 1, false };

        // sRGB shares the UNORM layout: the transfer function lives in the sampler and the blender,
        // not in the memory layout, so a raw copy hands out the stored bytes either way
        case VK_FORMAT_R8G8B8A8_UNORM: case VK_FORMAT_R8G8B8A8_SRGB: return { Kind::UNORM, 4, 1, false };
        case VK_FORMAT_R8G8B8A8_SNORM: return { Kind::SNORM, 4, 1, false };
        case VK_FORMAT_R8G8B8A8_UINT: return { Kind::UINT, 4, 1, false };
        case VK_FORMAT_R8G8B8A8_SINT: return { Kind::SINT, 4, 1, false };
        case VK_FORMAT_B8G8R8A8_UNORM: case VK_FORMAT_B8G8R8A8_SRGB: return { Kind::UNORM, 4, 1, true };

        case VK_FORMAT_R16_UNORM: case VK_FORMAT_D16_UNORM: return { Kind::UNORM, 1, 2, false };
        case VK_FORMAT_R16_SNORM: return { Kind::SNORM, 1, 2, false };
        case VK_FORMAT_R16_UINT: return { Kind::UINT, 1, 2, false };
        case VK_FORMAT_R16_SINT: return { Kind::SINT, 1, 2, false };
        case VK_FORMAT_R16_SFLOAT: return { Kind::SFLOAT, 1, 2, false };

        case VK_FORMAT_R16G16_UNORM: return { Kind::UNORM, 2, 2, false };
        case VK_FORMAT_R16G16_SNORM: return { Kind::SNORM, 2, 2, false };
        case VK_FORMAT_R16G16_UINT: return { Kind::UINT, 2, 2, false };
        case VK_FORMAT_R16G16_SINT: return { Kind::SINT, 2, 2, false };
        case VK_FORMAT_R16G16_SFLOAT: return { Kind::SFLOAT, 2, 2, false };

        case VK_FORMAT_R16G16B16_UNORM: return { Kind::UNORM, 3, 2, false };
        case VK_FORMAT_R16G16B16_SNORM: return { Kind::SNORM, 3, 2, false };
        case VK_FORMAT_R16G16B16_UINT: return { Kind::UINT, 3, 2, false };
        case VK_FORMAT_R16G16B16_SINT: return { Kind::SINT, 3, 2, false };
        case VK_FORMAT_R16G16B16_SFLOAT: return { Kind::SFLOAT, 3, 2, false };

        case VK_FORMAT_R16G16B16A16_UNORM: return { Kind::UNORM, 4, 2, false };
        case VK_FORMAT_R16G16B16A16_SNORM: return { Kind::SNORM, 4, 2, false };
        case VK_FORMAT_R16G16B16A16_UINT: return { Kind::UINT, 4, 2, false };
        case VK_FORMAT_R16G16B16A16_SINT: return { Kind::SINT, 4, 2, false };
        case VK_FORMAT_R16G16B16A16_SFLOAT: return { Kind::SFLOAT, 4, 2, false };

        case VK_FORMAT_R32_UINT: return { Kind::UINT, 1, 4, false };
        case VK_FORMAT_R32_SINT: return { Kind::SINT, 1, 4, false };
        case VK_FORMAT_R32_SFLOAT: case VK_FORMAT_D32_SFLOAT: return { Kind::SFLOAT, 1, 4, false };

        case VK_FORMAT_R32G32_UINT: return { Kind::UINT, 2, 4, false };
        case VK_FORMAT_R32G32_SINT: return { Kind::SINT, 2, 4, false };
        case VK_FORMAT_R32G32_SFLOAT: return { Kind::SFLOAT, 2, 4, false };

        case VK_FORMAT_R32G32B32_UINT: return { Kind::UINT, 3, 4, false };
        case VK_FORMAT_R32G32B32_SINT: return { Kind::SINT, 3, 4, false };
        case VK_FORMAT_R32G32B32_SFLOAT: return { Kind::SFLOAT, 3, 4, false };

        case VK_FORMAT_R32G32B32A32_UINT: return { Kind::UINT, 4, 4, false };
        case VK_FORMAT_R32G32B32A32_SINT: return { Kind::SINT, 4, 4, false };
        case VK_FORMAT_R32G32B32A32_SFLOAT: return { Kind::SFLOAT, 4, 4, false };

        default:
            return { };
    }
}

bool SGCore::VulkanTypesCaster::isDepthFormat(SGGColorInternalFormat format) noexcept
{
    switch(format)
    {
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT16:
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT24:
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32:
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F:
        case SGGColorInternalFormat::SGG_DEPTH24_STENCIL8:
        case SGGColorInternalFormat::SGG_DEPTH32F_STENCIL8:
            return true;
        default:
            return false;
    }
}

bool SGCore::VulkanTypesCaster::isDepthStencilFormat(SGGColorInternalFormat format) noexcept
{
    return format == SGGColorInternalFormat::SGG_DEPTH24_STENCIL8 ||
           format == SGGColorInternalFormat::SGG_DEPTH32F_STENCIL8;
}

VkFormat SGCore::VulkanTypesCaster::vertexAttributeFormat(SGGDataType type, std::uint32_t components, bool normalized) noexcept
{
    const std::uint32_t c = components == 0 ? 1 : (components > 4 ? 4 : components);

    switch(type)
    {
        case SGGDataType::SGG_FLOAT:
        case SGGDataType::SGG_FLOAT2:
        case SGGDataType::SGG_FLOAT3:
        case SGGDataType::SGG_FLOAT4:
        {
            constexpr VkFormat formats[4] = { VK_FORMAT_R32_SFLOAT, VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT };
            return formats[c - 1];
        }
        case SGGDataType::SGG_INT:
        case SGGDataType::SGG_INT2:
        case SGGDataType::SGG_INT3:
        case SGGDataType::SGG_INT4:
        {
            constexpr VkFormat formats[4] = { VK_FORMAT_R32_SINT, VK_FORMAT_R32G32_SINT, VK_FORMAT_R32G32B32_SINT, VK_FORMAT_R32G32B32A32_SINT };
            return formats[c - 1];
        }
        case SGGDataType::SGG_UNSIGNED_INT:
        {
            constexpr VkFormat formats[4] = { VK_FORMAT_R32_UINT, VK_FORMAT_R32G32_UINT, VK_FORMAT_R32G32B32_UINT, VK_FORMAT_R32G32B32A32_UINT };
            return formats[c - 1];
        }
        case SGGDataType::SGG_SHORT:
        {
            constexpr VkFormat norm[4] = { VK_FORMAT_R16_SNORM, VK_FORMAT_R16G16_SNORM, VK_FORMAT_R16G16B16_SNORM, VK_FORMAT_R16G16B16A16_SNORM };
            constexpr VkFormat raw[4] = { VK_FORMAT_R16_SINT, VK_FORMAT_R16G16_SINT, VK_FORMAT_R16G16B16_SINT, VK_FORMAT_R16G16B16A16_SINT };
            return normalized ? norm[c - 1] : raw[c - 1];
        }
        case SGGDataType::SGG_UNSIGNED_SHORT:
        {
            constexpr VkFormat norm[4] = { VK_FORMAT_R16_UNORM, VK_FORMAT_R16G16_UNORM, VK_FORMAT_R16G16B16_UNORM, VK_FORMAT_R16G16B16A16_UNORM };
            constexpr VkFormat raw[4] = { VK_FORMAT_R16_UINT, VK_FORMAT_R16G16_UINT, VK_FORMAT_R16G16B16_UINT, VK_FORMAT_R16G16B16A16_UINT };
            return normalized ? norm[c - 1] : raw[c - 1];
        }
        case SGGDataType::SGG_BYTE:
        {
            constexpr VkFormat norm[4] = { VK_FORMAT_R8_SNORM, VK_FORMAT_R8G8_SNORM, VK_FORMAT_R8G8B8_SNORM, VK_FORMAT_R8G8B8A8_SNORM };
            constexpr VkFormat raw[4] = { VK_FORMAT_R8_SINT, VK_FORMAT_R8G8_SINT, VK_FORMAT_R8G8B8_SINT, VK_FORMAT_R8G8B8A8_SINT };
            return normalized ? norm[c - 1] : raw[c - 1];
        }
        case SGGDataType::SGG_UNSIGNED_BYTE:
        {
            constexpr VkFormat norm[4] = { VK_FORMAT_R8_UNORM, VK_FORMAT_R8G8_UNORM, VK_FORMAT_R8G8B8_UNORM, VK_FORMAT_R8G8B8A8_UNORM };
            constexpr VkFormat raw[4] = { VK_FORMAT_R8_UINT, VK_FORMAT_R8G8_UINT, VK_FORMAT_R8G8B8_UINT, VK_FORMAT_R8G8B8A8_UINT };
            return normalized ? norm[c - 1] : raw[c - 1];
        }
        default:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
    }
}

VkCompareOp SGCore::VulkanTypesCaster::sggCompareToVk(SGDepthStencilFunc func) noexcept
{
    switch(func)
    {
        case SGDepthStencilFunc::SGG_NEVER: return VK_COMPARE_OP_NEVER;
        case SGDepthStencilFunc::SGG_LESS: return VK_COMPARE_OP_LESS;
        case SGDepthStencilFunc::SGG_LEQUAL: return VK_COMPARE_OP_LESS_OR_EQUAL;
        case SGDepthStencilFunc::SGG_GREATER: return VK_COMPARE_OP_GREATER;
        case SGDepthStencilFunc::SGG_GEQUAL: return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case SGDepthStencilFunc::SGG_EQUAL: return VK_COMPARE_OP_EQUAL;
        case SGDepthStencilFunc::SGG_NOTEQUAL: return VK_COMPARE_OP_NOT_EQUAL;
        case SGDepthStencilFunc::SGG_ALWAYS: return VK_COMPARE_OP_ALWAYS;
    }
    return VK_COMPARE_OP_ALWAYS;
}

VkStencilOp SGCore::VulkanTypesCaster::sggStencilOpToVk(SGStencilOp op) noexcept
{
    switch(op)
    {
        case SGStencilOp::SGG_KEEP: return VK_STENCIL_OP_KEEP;
        case SGStencilOp::SGG_REPLACE: return VK_STENCIL_OP_REPLACE;
        case SGStencilOp::SGG_ZERO: return VK_STENCIL_OP_ZERO;
        case SGStencilOp::SGG_INCR: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
        case SGStencilOp::SGG_INCR_WRAP: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
        case SGStencilOp::SGG_DECR: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
        case SGStencilOp::SGG_DECR_WRAP: return VK_STENCIL_OP_DECREMENT_AND_WRAP;
        case SGStencilOp::SGG_INVERT: return VK_STENCIL_OP_INVERT;
    }
    return VK_STENCIL_OP_KEEP;
}

VkBlendFactor SGCore::VulkanTypesCaster::sggBlendFactorToVk(SGBlendingFactor factor) noexcept
{
    switch(factor)
    {
        case SGBlendingFactor::SGG_ZERO: return VK_BLEND_FACTOR_ZERO;
        case SGBlendingFactor::SGG_ONE: return VK_BLEND_FACTOR_ONE;
        case SGBlendingFactor::SGG_SRC_COLOR: return VK_BLEND_FACTOR_SRC_COLOR;
        case SGBlendingFactor::SGG_ONE_MINUS_SRC_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case SGBlendingFactor::SGG_DST_COLOR: return VK_BLEND_FACTOR_DST_COLOR;
        case SGBlendingFactor::SGG_ONE_MINUS_DST_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case SGBlendingFactor::SGG_SRC_ALPHA: return VK_BLEND_FACTOR_SRC_ALPHA;
        case SGBlendingFactor::SGG_ONE_MINUS_SRC_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case SGBlendingFactor::SGG_DST_ALPHA: return VK_BLEND_FACTOR_DST_ALPHA;
        case SGBlendingFactor::SGG_ONE_MINUS_DST_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        case SGBlendingFactor::SGG_CONST_COLOR: return VK_BLEND_FACTOR_CONSTANT_COLOR;
        case SGBlendingFactor::SGG_ONE_MINUS_CONST_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
        case SGBlendingFactor::SGG_CONST_ALPHA: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
        case SGBlendingFactor::SGG_ONE_MINUS_CONST_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
    }
    return VK_BLEND_FACTOR_ONE;
}

VkBlendOp SGCore::VulkanTypesCaster::sggBlendEquationToVk(SGEquation equation) noexcept
{
    switch(equation)
    {
        case SGEquation::SGG_FUNC_ADD: return VK_BLEND_OP_ADD;
        case SGEquation::SGG_FUNC_SUBTRACT: return VK_BLEND_OP_SUBTRACT;
        case SGEquation::SGG_FUNC_REVERSE_SUBTRACT: return VK_BLEND_OP_REVERSE_SUBTRACT;
        case SGEquation::SGG_MIN: return VK_BLEND_OP_MIN;
        case SGEquation::SGG_MAX: return VK_BLEND_OP_MAX;
    }
    return VK_BLEND_OP_ADD;
}

VkCullModeFlags SGCore::VulkanTypesCaster::sggFaceTypeToVk(SGFaceType faceType) noexcept
{
    switch(faceType)
    {
        case SGFaceType::SGG_FRONT_FACE: return VK_CULL_MODE_FRONT_BIT;
        case SGFaceType::SGG_BACK_FACE: return VK_CULL_MODE_BACK_BIT;
        case SGFaceType::SGG_FRONT_BACK_FACE: return VK_CULL_MODE_FRONT_AND_BACK;
    }
    return VK_CULL_MODE_BACK_BIT;
}

VkPrimitiveTopology SGCore::VulkanTypesCaster::sggDrawModeToVk(SGDrawMode drawMode) noexcept
{
    switch(drawMode)
    {
        case SGDrawMode::SGG_TRIANGLES: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case SGDrawMode::SGG_TRIANGLE_STRIP: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        case SGDrawMode::SGG_TRIANGLE_FAN: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        case SGDrawMode::SGG_LINES: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        // GL_QUADS has no Vulkan counterpart; the legacy quad users go through index buffers anyway
        case SGDrawMode::SGG_QUADS: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case SGDrawMode::SGG_POINTS: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case SGDrawMode::SGG_PATCHES: return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
    }
    return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
}
