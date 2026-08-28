//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12TypesCaster.h"

#if defined(_WIN32)

DXGI_FORMAT SGCore::DX12TypesCaster::sggInternalFormatToDXGI(SGGColorInternalFormat format) noexcept
{
    switch(format)
    {
        case SGGColorInternalFormat::SGG_R8: return DXGI_FORMAT_R8_UNORM;
        case SGGColorInternalFormat::SGG_R8_SIGNED_NORMALIZED: return DXGI_FORMAT_R8_SNORM;
        case SGGColorInternalFormat::SGG_R16: return DXGI_FORMAT_R16_UNORM;
        case SGGColorInternalFormat::SGG_R16_SIGNED_NORMALIZED: return DXGI_FORMAT_R16_SNORM;
        case SGGColorInternalFormat::SGG_RG8: return DXGI_FORMAT_R8G8_UNORM;
        case SGGColorInternalFormat::SGG_RG8_SIGNED_NORMALIZED: return DXGI_FORMAT_R8G8_SNORM;
        case SGGColorInternalFormat::SGG_RG16: return DXGI_FORMAT_R16G16_UNORM;
        case SGGColorInternalFormat::SGG_RG16_SIGNED_NORMALIZED: return DXGI_FORMAT_R16G16_SNORM;
        // packed low-bit formats have no DXGI equivalent worth keeping: widened, as on Vulkan
        case SGGColorInternalFormat::SGG_R3_G3_B2:
        case SGGColorInternalFormat::SGG_RGB4:
        case SGGColorInternalFormat::SGG_RGB5:
        case SGGColorInternalFormat::SGG_RGB8:
        case SGGColorInternalFormat::SGG_RGBA2:
        case SGGColorInternalFormat::SGG_RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
        case SGGColorInternalFormat::SGG_RGB8_SIGNED_NORMALIZED:
        case SGGColorInternalFormat::SGG_RGBA8_SIGNED_NORMALIZED: return DXGI_FORMAT_R8G8B8A8_SNORM;
        case SGGColorInternalFormat::SGG_RGB10:
        case SGGColorInternalFormat::SGG_RGB10_A2: return DXGI_FORMAT_R10G10B10A2_UNORM;
        case SGGColorInternalFormat::SGG_RGB10_A2_UNSIGNED_INT: return DXGI_FORMAT_R10G10B10A2_UINT;
        case SGGColorInternalFormat::SGG_RGB12:
        case SGGColorInternalFormat::SGG_RGBA12:
        case SGGColorInternalFormat::SGG_RGBA16: return DXGI_FORMAT_R16G16B16A16_UNORM;
        case SGGColorInternalFormat::SGG_RGB16_SIGNED_NORMALIZED: return DXGI_FORMAT_R16G16B16A16_SNORM;
        case SGGColorInternalFormat::SGG_RGBA4: return DXGI_FORMAT_B4G4R4A4_UNORM;
        case SGGColorInternalFormat::SGG_RGB5_A1: return DXGI_FORMAT_B5G5R5A1_UNORM;
        case SGGColorInternalFormat::SGG_SRGB8:
        case SGGColorInternalFormat::SGG_SRGB8_ALPHA8: return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case SGGColorInternalFormat::SGG_R16_FLOAT: return DXGI_FORMAT_R16_FLOAT;
        case SGGColorInternalFormat::SGG_RG16_FLOAT: return DXGI_FORMAT_R16G16_FLOAT;
        case SGGColorInternalFormat::SGG_RGB16_FLOAT:
        case SGGColorInternalFormat::SGG_RGBA16_FLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case SGGColorInternalFormat::SGG_R32_FLOAT: return DXGI_FORMAT_R32_FLOAT;
        case SGGColorInternalFormat::SGG_RG32_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;
        case SGGColorInternalFormat::SGG_RGB32_FLOAT:
        case SGGColorInternalFormat::SGG_RGBA32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case SGGColorInternalFormat::SGG_R11_G11_B10_FLOAT: return DXGI_FORMAT_R11G11B10_FLOAT;
        case SGGColorInternalFormat::SGG_RGB9_E5: return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
        case SGGColorInternalFormat::SGG_R8_INT: return DXGI_FORMAT_R8_SINT;
        case SGGColorInternalFormat::SGG_R8_UNSIGNED_INT: return DXGI_FORMAT_R8_UINT;
        case SGGColorInternalFormat::SGG_R16_INT: return DXGI_FORMAT_R16_SINT;
        case SGGColorInternalFormat::SGG_R16_UNSIGNED_INT: return DXGI_FORMAT_R16_UINT;
        case SGGColorInternalFormat::SGG_R32_INT: return DXGI_FORMAT_R32_SINT;
        case SGGColorInternalFormat::SGG_R32_UNSIGNED_INT: return DXGI_FORMAT_R32_UINT;
        case SGGColorInternalFormat::SGG_RG8_INT: return DXGI_FORMAT_R8G8_SINT;
        case SGGColorInternalFormat::SGG_RG8_UNSIGNED_INT: return DXGI_FORMAT_R8G8_UINT;
        case SGGColorInternalFormat::SGG_RG16_INT: return DXGI_FORMAT_R16G16_SINT;
        case SGGColorInternalFormat::SGG_RG16_UNSIGNED_INT: return DXGI_FORMAT_R16G16_UINT;
        case SGGColorInternalFormat::SGG_RG32_INT: return DXGI_FORMAT_R32G32_SINT;
        case SGGColorInternalFormat::SGG_RG32_UNSIGNED_INT: return DXGI_FORMAT_R32G32_UINT;
        case SGGColorInternalFormat::SGG_RGB8_INT:
        case SGGColorInternalFormat::SGG_RGBA8_INT: return DXGI_FORMAT_R8G8B8A8_SINT;
        case SGGColorInternalFormat::SGG_RGB8_UNSIGNED_INT:
        case SGGColorInternalFormat::SGG_RGBA8_UNSIGNED_INT: return DXGI_FORMAT_R8G8B8A8_UINT;
        case SGGColorInternalFormat::SGG_RGB16_INT:
        case SGGColorInternalFormat::SGG_RGBA16_INT: return DXGI_FORMAT_R16G16B16A16_SINT;
        case SGGColorInternalFormat::SGG_RGB16_UNSIGNED_INT:
        case SGGColorInternalFormat::SGG_RGBA16_UNSIGNED_INT: return DXGI_FORMAT_R16G16B16A16_UINT;
        case SGGColorInternalFormat::SGG_RGB32_INT:
        case SGGColorInternalFormat::SGG_RGBA32_INT: return DXGI_FORMAT_R32G32B32A32_SINT;
        case SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT:
        case SGGColorInternalFormat::SGG_RGBA32_UNSIGNED_INT: return DXGI_FORMAT_R32G32B32A32_UINT;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT16: return DXGI_FORMAT_D16_UNORM;
        // DXGI has no depth-only 24-bit format: the packed one is the standard stand-in
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT24:
        case SGGColorInternalFormat::SGG_DEPTH24_STENCIL8:
        case SGGColorInternalFormat::SGG_STENCIL_INDEX8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32:
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F: return DXGI_FORMAT_D32_FLOAT;
        case SGGColorInternalFormat::SGG_DEPTH32F_STENCIL8: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        default: return DXGI_FORMAT_R8G8B8A8_UNORM;
    }
}

DXGI_FORMAT SGCore::DX12TypesCaster::sggInternalFormatToDXGITexel(SGGColorInternalFormat format) noexcept
{
    switch(format)
    {
        case SGGColorInternalFormat::SGG_RGB32_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
        case SGGColorInternalFormat::SGG_RGB32_INT: return DXGI_FORMAT_R32G32B32_SINT;
        case SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT: return DXGI_FORMAT_R32G32B32_UINT;
        default: return sggInternalFormatToDXGI(format);
    }
}

bool SGCore::DX12TypesCaster::needsAlphaExpansion(SGGColorInternalFormat format) noexcept
{
    switch(format)
    {
        case SGGColorInternalFormat::SGG_RGB8:
        case SGGColorInternalFormat::SGG_RGB8_SIGNED_NORMALIZED:
        case SGGColorInternalFormat::SGG_RGB12:
        case SGGColorInternalFormat::SGG_RGB16_SIGNED_NORMALIZED:
        case SGGColorInternalFormat::SGG_RGB16_FLOAT:
        case SGGColorInternalFormat::SGG_RGB32_FLOAT:
        case SGGColorInternalFormat::SGG_RGB8_INT:
        case SGGColorInternalFormat::SGG_RGB8_UNSIGNED_INT:
        case SGGColorInternalFormat::SGG_RGB16_INT:
        case SGGColorInternalFormat::SGG_RGB16_UNSIGNED_INT:
        case SGGColorInternalFormat::SGG_RGB32_INT:
        case SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT: return true;
        default: return false;
    }
}

std::uint32_t SGCore::DX12TypesCaster::formatTexelSize(DXGI_FORMAT format) noexcept
{
    switch(format)
    {
        case DXGI_FORMAT_R8_UNORM:
        case DXGI_FORMAT_R8_SNORM:
        case DXGI_FORMAT_R8_UINT:
        case DXGI_FORMAT_R8_SINT: return 1;
        case DXGI_FORMAT_R8G8_UNORM:
        case DXGI_FORMAT_R8G8_SNORM:
        case DXGI_FORMAT_R8G8_UINT:
        case DXGI_FORMAT_R8G8_SINT:
        case DXGI_FORMAT_R16_UNORM:
        case DXGI_FORMAT_R16_SNORM:
        case DXGI_FORMAT_R16_UINT:
        case DXGI_FORMAT_R16_SINT:
        case DXGI_FORMAT_R16_FLOAT:
        case DXGI_FORMAT_D16_UNORM:
        case DXGI_FORMAT_B4G4R4A4_UNORM:
        case DXGI_FORMAT_B5G5R5A1_UNORM: return 2;
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_SNORM:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_SINT:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_R16G16_UNORM:
        case DXGI_FORMAT_R16G16_SNORM:
        case DXGI_FORMAT_R16G16_UINT:
        case DXGI_FORMAT_R16G16_SINT:
        case DXGI_FORMAT_R16G16_FLOAT:
        case DXGI_FORMAT_R32_UINT:
        case DXGI_FORMAT_R32_SINT:
        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_D32_FLOAT:
        case DXGI_FORMAT_D24_UNORM_S8_UINT:
        case DXGI_FORMAT_R10G10B10A2_UNORM:
        case DXGI_FORMAT_R10G10B10A2_UINT:
        case DXGI_FORMAT_R11G11B10_FLOAT:
        case DXGI_FORMAT_R9G9B9E5_SHAREDEXP: return 4;
        case DXGI_FORMAT_R16G16B16A16_UNORM:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_SINT:
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R32G32_UINT:
        case DXGI_FORMAT_R32G32_SINT:
        case DXGI_FORMAT_R32G32_FLOAT:
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: return 8;
        case DXGI_FORMAT_R32G32B32A32_UINT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
        case DXGI_FORMAT_R32G32B32A32_FLOAT: return 16;
        default: return 0;
    }
}

SGCore::ChannelLayout SGCore::DX12TypesCaster::formatLayout(DXGI_FORMAT format) noexcept
{
    using Kind = ChannelKind;

    switch(format)
    {
        case DXGI_FORMAT_R8_UNORM: return { Kind::UNORM, 1, 1, false };
        case DXGI_FORMAT_R8_SNORM: return { Kind::SNORM, 1, 1, false };
        case DXGI_FORMAT_R8_UINT: return { Kind::UINT, 1, 1, false };
        case DXGI_FORMAT_R8_SINT: return { Kind::SINT, 1, 1, false };

        case DXGI_FORMAT_R8G8_UNORM: return { Kind::UNORM, 2, 1, false };
        case DXGI_FORMAT_R8G8_SNORM: return { Kind::SNORM, 2, 1, false };
        case DXGI_FORMAT_R8G8_UINT: return { Kind::UINT, 2, 1, false };
        case DXGI_FORMAT_R8G8_SINT: return { Kind::SINT, 2, 1, false };

        // sRGB shares the UNORM layout: the transfer function lives in the sampler and the blender,
        // not in the memory layout
        case DXGI_FORMAT_R8G8B8A8_UNORM: case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: return { Kind::UNORM, 4, 1, false };
        case DXGI_FORMAT_R8G8B8A8_SNORM: return { Kind::SNORM, 4, 1, false };
        case DXGI_FORMAT_R8G8B8A8_UINT: return { Kind::UINT, 4, 1, false };
        case DXGI_FORMAT_R8G8B8A8_SINT: return { Kind::SINT, 4, 1, false };
        case DXGI_FORMAT_B8G8R8A8_UNORM: case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: return { Kind::UNORM, 4, 1, true };

        case DXGI_FORMAT_R16_UNORM: case DXGI_FORMAT_D16_UNORM: return { Kind::UNORM, 1, 2, false };
        case DXGI_FORMAT_R16_SNORM: return { Kind::SNORM, 1, 2, false };
        case DXGI_FORMAT_R16_UINT: return { Kind::UINT, 1, 2, false };
        case DXGI_FORMAT_R16_SINT: return { Kind::SINT, 1, 2, false };
        case DXGI_FORMAT_R16_FLOAT: return { Kind::SFLOAT, 1, 2, false };

        case DXGI_FORMAT_R16G16_UNORM: return { Kind::UNORM, 2, 2, false };
        case DXGI_FORMAT_R16G16_SNORM: return { Kind::SNORM, 2, 2, false };
        case DXGI_FORMAT_R16G16_UINT: return { Kind::UINT, 2, 2, false };
        case DXGI_FORMAT_R16G16_SINT: return { Kind::SINT, 2, 2, false };
        case DXGI_FORMAT_R16G16_FLOAT: return { Kind::SFLOAT, 2, 2, false };

        case DXGI_FORMAT_R16G16B16A16_UNORM: return { Kind::UNORM, 4, 2, false };
        case DXGI_FORMAT_R16G16B16A16_SNORM: return { Kind::SNORM, 4, 2, false };
        case DXGI_FORMAT_R16G16B16A16_UINT: return { Kind::UINT, 4, 2, false };
        case DXGI_FORMAT_R16G16B16A16_SINT: return { Kind::SINT, 4, 2, false };
        case DXGI_FORMAT_R16G16B16A16_FLOAT: return { Kind::SFLOAT, 4, 2, false };

        case DXGI_FORMAT_R32_UINT: return { Kind::UINT, 1, 4, false };
        case DXGI_FORMAT_R32_SINT: return { Kind::SINT, 1, 4, false };
        case DXGI_FORMAT_R32_FLOAT: case DXGI_FORMAT_D32_FLOAT: return { Kind::SFLOAT, 1, 4, false };

        case DXGI_FORMAT_R32G32_UINT: return { Kind::UINT, 2, 4, false };
        case DXGI_FORMAT_R32G32_SINT: return { Kind::SINT, 2, 4, false };
        case DXGI_FORMAT_R32G32_FLOAT: return { Kind::SFLOAT, 2, 4, false };

        case DXGI_FORMAT_R32G32B32_UINT: return { Kind::UINT, 3, 4, false };
        case DXGI_FORMAT_R32G32B32_SINT: return { Kind::SINT, 3, 4, false };
        case DXGI_FORMAT_R32G32B32_FLOAT: return { Kind::SFLOAT, 3, 4, false };

        case DXGI_FORMAT_R32G32B32A32_UINT: return { Kind::UINT, 4, 4, false };
        case DXGI_FORMAT_R32G32B32A32_SINT: return { Kind::SINT, 4, 4, false };
        case DXGI_FORMAT_R32G32B32A32_FLOAT: return { Kind::SFLOAT, 4, 4, false };

        // packed formats (R10G10B10A2, R11G11B10, R9G9B9E5, B4G4R4A4, B5G5R5A1) and the
        // depth-stencil pairs have no channel array to walk
        default: return { };
    }
}

bool SGCore::DX12TypesCaster::isDepthFormat(DXGI_FORMAT format) noexcept
{
    switch(format)
    {
        case DXGI_FORMAT_D16_UNORM:
        case DXGI_FORMAT_D24_UNORM_S8_UINT:
        case DXGI_FORMAT_D32_FLOAT:
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: return true;
        default: return false;
    }
}

DXGI_FORMAT SGCore::DX12TypesCaster::depthToTypeless(DXGI_FORMAT format) noexcept
{
    switch(format)
    {
        case DXGI_FORMAT_D16_UNORM: return DXGI_FORMAT_R16_TYPELESS;
        case DXGI_FORMAT_D24_UNORM_S8_UINT: return DXGI_FORMAT_R24G8_TYPELESS;
        case DXGI_FORMAT_D32_FLOAT: return DXGI_FORMAT_R32_TYPELESS;
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: return DXGI_FORMAT_R32G8X24_TYPELESS;
        default: return format;
    }
}

DXGI_FORMAT SGCore::DX12TypesCaster::depthToShaderView(DXGI_FORMAT format) noexcept
{
    switch(format)
    {
        case DXGI_FORMAT_D16_UNORM: return DXGI_FORMAT_R16_UNORM;
        case DXGI_FORMAT_D24_UNORM_S8_UINT: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        case DXGI_FORMAT_D32_FLOAT: return DXGI_FORMAT_R32_FLOAT;
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        default: return format;
    }
}

DXGI_FORMAT SGCore::DX12TypesCaster::vertexAttributeFormat(SGGDataType type, std::uint32_t components, bool normalized) noexcept
{
    const std::uint32_t count = components == 0 ? 1 : (components > 4 ? 4 : components);

    switch(type)
    {
        case SGGDataType::SGG_FLOAT:
        case SGGDataType::SGG_FLOAT2:
        case SGGDataType::SGG_FLOAT3:
        case SGGDataType::SGG_FLOAT4:
        case SGGDataType::SGG_MAT2:
        case SGGDataType::SGG_MAT3:
        case SGGDataType::SGG_MAT4:
            switch(count)
            {
                case 1: return DXGI_FORMAT_R32_FLOAT;
                case 2: return DXGI_FORMAT_R32G32_FLOAT;
                case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
                default: return DXGI_FORMAT_R32G32B32A32_FLOAT;
            }
        case SGGDataType::SGG_INT:
        case SGGDataType::SGG_INT2:
        case SGGDataType::SGG_INT3:
        case SGGDataType::SGG_INT4:
            switch(count)
            {
                case 1: return DXGI_FORMAT_R32_SINT;
                case 2: return DXGI_FORMAT_R32G32_SINT;
                case 3: return DXGI_FORMAT_R32G32B32_SINT;
                default: return DXGI_FORMAT_R32G32B32A32_SINT;
            }
        case SGGDataType::SGG_UNSIGNED_INT:
            switch(count)
            {
                case 1: return DXGI_FORMAT_R32_UINT;
                case 2: return DXGI_FORMAT_R32G32_UINT;
                case 3: return DXGI_FORMAT_R32G32B32_UINT;
                default: return DXGI_FORMAT_R32G32B32A32_UINT;
            }
        // DXGI has no 3-component 8/16-bit vertex formats: those widen to 4 components, which is
        // safe as long as the buffer stride keeps the attribute in range
        case SGGDataType::SGG_SHORT:
            if(normalized) return count == 1 ? DXGI_FORMAT_R16_SNORM : (count == 2 ? DXGI_FORMAT_R16G16_SNORM : DXGI_FORMAT_R16G16B16A16_SNORM);
            return count == 1 ? DXGI_FORMAT_R16_SINT : (count == 2 ? DXGI_FORMAT_R16G16_SINT : DXGI_FORMAT_R16G16B16A16_SINT);
        case SGGDataType::SGG_UNSIGNED_SHORT:
            if(normalized) return count == 1 ? DXGI_FORMAT_R16_UNORM : (count == 2 ? DXGI_FORMAT_R16G16_UNORM : DXGI_FORMAT_R16G16B16A16_UNORM);
            return count == 1 ? DXGI_FORMAT_R16_UINT : (count == 2 ? DXGI_FORMAT_R16G16_UINT : DXGI_FORMAT_R16G16B16A16_UINT);
        case SGGDataType::SGG_BYTE:
            if(normalized) return count == 1 ? DXGI_FORMAT_R8_SNORM : (count == 2 ? DXGI_FORMAT_R8G8_SNORM : DXGI_FORMAT_R8G8B8A8_SNORM);
            return count == 1 ? DXGI_FORMAT_R8_SINT : (count == 2 ? DXGI_FORMAT_R8G8_SINT : DXGI_FORMAT_R8G8B8A8_SINT);
        case SGGDataType::SGG_UNSIGNED_BYTE:
        case SGGDataType::SGG_BOOL:
            if(normalized) return count == 1 ? DXGI_FORMAT_R8_UNORM : (count == 2 ? DXGI_FORMAT_R8G8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM);
            return count == 1 ? DXGI_FORMAT_R8_UINT : (count == 2 ? DXGI_FORMAT_R8G8_UINT : DXGI_FORMAT_R8G8B8A8_UINT);
        default: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }
}

D3D12_COMPARISON_FUNC SGCore::DX12TypesCaster::sggCompareToDX(SGDepthStencilFunc func) noexcept
{
    switch(func)
    {
        case SGDepthStencilFunc::SGG_NEVER: return D3D12_COMPARISON_FUNC_NEVER;
        case SGDepthStencilFunc::SGG_LESS: return D3D12_COMPARISON_FUNC_LESS;
        case SGDepthStencilFunc::SGG_LEQUAL: return D3D12_COMPARISON_FUNC_LESS_EQUAL;
        case SGDepthStencilFunc::SGG_GREATER: return D3D12_COMPARISON_FUNC_GREATER;
        case SGDepthStencilFunc::SGG_GEQUAL: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
        case SGDepthStencilFunc::SGG_EQUAL: return D3D12_COMPARISON_FUNC_EQUAL;
        case SGDepthStencilFunc::SGG_NOTEQUAL: return D3D12_COMPARISON_FUNC_NOT_EQUAL;
        default: return D3D12_COMPARISON_FUNC_ALWAYS;
    }
}

D3D12_STENCIL_OP SGCore::DX12TypesCaster::sggStencilOpToDX(SGStencilOp op) noexcept
{
    switch(op)
    {
        case SGStencilOp::SGG_KEEP: return D3D12_STENCIL_OP_KEEP;
        case SGStencilOp::SGG_REPLACE: return D3D12_STENCIL_OP_REPLACE;
        case SGStencilOp::SGG_ZERO: return D3D12_STENCIL_OP_ZERO;
        case SGStencilOp::SGG_INCR: return D3D12_STENCIL_OP_INCR_SAT;
        case SGStencilOp::SGG_INCR_WRAP: return D3D12_STENCIL_OP_INCR;
        case SGStencilOp::SGG_DECR: return D3D12_STENCIL_OP_DECR_SAT;
        case SGStencilOp::SGG_DECR_WRAP: return D3D12_STENCIL_OP_DECR;
        default: return D3D12_STENCIL_OP_INVERT;
    }
}

D3D12_BLEND SGCore::DX12TypesCaster::sggBlendFactorToDX(SGBlendingFactor factor) noexcept
{
    switch(factor)
    {
        case SGBlendingFactor::SGG_ZERO: return D3D12_BLEND_ZERO;
        case SGBlendingFactor::SGG_ONE: return D3D12_BLEND_ONE;
        case SGBlendingFactor::SGG_SRC_COLOR: return D3D12_BLEND_SRC_COLOR;
        case SGBlendingFactor::SGG_ONE_MINUS_SRC_COLOR: return D3D12_BLEND_INV_SRC_COLOR;
        case SGBlendingFactor::SGG_DST_COLOR: return D3D12_BLEND_DEST_COLOR;
        case SGBlendingFactor::SGG_ONE_MINUS_DST_COLOR: return D3D12_BLEND_INV_DEST_COLOR;
        case SGBlendingFactor::SGG_SRC_ALPHA: return D3D12_BLEND_SRC_ALPHA;
        case SGBlendingFactor::SGG_ONE_MINUS_SRC_ALPHA: return D3D12_BLEND_INV_SRC_ALPHA;
        case SGBlendingFactor::SGG_DST_ALPHA: return D3D12_BLEND_DEST_ALPHA;
        case SGBlendingFactor::SGG_ONE_MINUS_DST_ALPHA: return D3D12_BLEND_INV_DEST_ALPHA;
        // D3D12 keeps one blend factor register shared by colour and alpha, and the engine sets both
        // from the same constant, so both map onto BLEND_FACTOR
        case SGBlendingFactor::SGG_CONST_COLOR:
        case SGBlendingFactor::SGG_CONST_ALPHA: return D3D12_BLEND_BLEND_FACTOR;
        default: return D3D12_BLEND_INV_BLEND_FACTOR;
    }
}

D3D12_BLEND_OP SGCore::DX12TypesCaster::sggBlendEquationToDX(SGEquation equation) noexcept
{
    switch(equation)
    {
        case SGEquation::SGG_FUNC_ADD: return D3D12_BLEND_OP_ADD;
        case SGEquation::SGG_FUNC_SUBTRACT: return D3D12_BLEND_OP_SUBTRACT;
        case SGEquation::SGG_FUNC_REVERSE_SUBTRACT: return D3D12_BLEND_OP_REV_SUBTRACT;
        case SGEquation::SGG_MIN: return D3D12_BLEND_OP_MIN;
        default: return D3D12_BLEND_OP_MAX;
    }
}

D3D12_CULL_MODE SGCore::DX12TypesCaster::sggFaceTypeToDX(SGFaceType faceType) noexcept
{
    switch(faceType)
    {
        case SGFaceType::SGG_FRONT_FACE: return D3D12_CULL_MODE_FRONT;
        case SGFaceType::SGG_BACK_FACE: return D3D12_CULL_MODE_BACK;
        // the rasterizer state can not cull both faces: GL_FRONT_AND_BACK has no counterpart here,
        // and no engine pass uses it
        default: return D3D12_CULL_MODE_BACK;
    }
}

D3D12_PRIMITIVE_TOPOLOGY SGCore::DX12TypesCaster::sggDrawModeToDX(SGDrawMode drawMode, int patchVerticesCount) noexcept
{
    switch(drawMode)
    {
        case SGDrawMode::SGG_TRIANGLES: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case SGDrawMode::SGG_TRIANGLE_STRIP: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        // no triangle fan (dropped in D3D10) and no quads: the quads of the engine are index buffers
        // of triangles anyway, so both fall back to a triangle list
        case SGDrawMode::SGG_TRIANGLE_FAN:
        case SGDrawMode::SGG_QUADS: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case SGDrawMode::SGG_LINES: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
        case SGDrawMode::SGG_POINTS: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
        case SGDrawMode::SGG_PATCHES:
        {
            const int points = patchVerticesCount < 1 ? 1 : (patchVerticesCount > 32 ? 32 : patchVerticesCount);
            return static_cast<D3D12_PRIMITIVE_TOPOLOGY>(D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST + points - 1);
        }
        default: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE SGCore::DX12TypesCaster::sggDrawModeToTopologyType(SGDrawMode drawMode) noexcept
{
    switch(drawMode)
    {
        case SGDrawMode::SGG_LINES: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
        case SGDrawMode::SGG_POINTS: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
        case SGDrawMode::SGG_PATCHES: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
        default: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    }
}

#endif
