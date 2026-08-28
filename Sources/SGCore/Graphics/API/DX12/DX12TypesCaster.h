//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <cstdint>

#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Graphics/API/PixelConversion.h"
#include "DX12Common.h"

namespace SGCore
{
    /// Engine enum -> D3D12 / DXGI conversions. The DX12 twin of VulkanTypesCaster, kept in the same
    /// shape so a mapping decided once (widening packed RGB to RGBA, depth formats, blend factors)
    /// stays identical on both explicit backends.
    struct DX12TypesCaster
    {
        /// Format of a texture / attachment. Formats DXGI has no packed equivalent for (RGB8,
        /// RGB16...) are widened to their RGBA counterparts, exactly as on Vulkan.
        [[nodiscard]] static DXGI_FORMAT sggInternalFormatToDXGI(SGGColorInternalFormat format) noexcept;

        /// Format for a typed (texel) buffer SRV. Must not widen: the elements of a texture buffer are
        /// packed tight, and a widened format would read every one at the wrong stride. Only the
        /// 32-bit three-component formats exist in DXGI; narrower ones fall back to the widened
        /// mapping, which is wrong for them — no engine texture buffer uses those.
        [[nodiscard]] static DXGI_FORMAT sggInternalFormatToDXGITexel(SGGColorInternalFormat format) noexcept;

        /// True when sggInternalFormatToDXGI() widened a 3-channel format to 4 channels.
        [[nodiscard]] static bool needsAlphaExpansion(SGGColorInternalFormat format) noexcept;

        /// Bytes one texel occupies; 0 for formats without a plain byte size.
        [[nodiscard]] static std::uint32_t formatTexelSize(DXGI_FORMAT format) noexcept;

        /// Channel layout of a plain DXGI format, the input of PixelConversion: image<->CPU copies
        /// here are raw memory, so anything moving pixels in or out in a different data type has to
        /// convert them itself. Packed and compressed formats report UNKNOWN.
        [[nodiscard]] static ChannelLayout formatLayout(DXGI_FORMAT format) noexcept;

        [[nodiscard]] static bool isDepthFormat(DXGI_FORMAT format) noexcept;
        /// Typeless format a depth resource must be created with to be readable as a texture, plus
        /// the view formats. Returns the format itself when it is not a depth one.
        [[nodiscard]] static DXGI_FORMAT depthToTypeless(DXGI_FORMAT format) noexcept;
        [[nodiscard]] static DXGI_FORMAT depthToShaderView(DXGI_FORMAT format) noexcept;

        /// Vertex attribute format from the engine's (type, components, normalized) triple.
        [[nodiscard]] static DXGI_FORMAT vertexAttributeFormat(SGGDataType type, std::uint32_t components, bool normalized) noexcept;

        [[nodiscard]] static D3D12_COMPARISON_FUNC sggCompareToDX(SGDepthStencilFunc func) noexcept;
        [[nodiscard]] static D3D12_STENCIL_OP sggStencilOpToDX(SGStencilOp op) noexcept;
        [[nodiscard]] static D3D12_BLEND sggBlendFactorToDX(SGBlendingFactor factor) noexcept;
        [[nodiscard]] static D3D12_BLEND_OP sggBlendEquationToDX(SGEquation equation) noexcept;
        [[nodiscard]] static D3D12_CULL_MODE sggFaceTypeToDX(SGFaceType faceType) noexcept;
        [[nodiscard]] static D3D12_PRIMITIVE_TOPOLOGY sggDrawModeToDX(SGDrawMode drawMode, int patchVerticesCount) noexcept;
        [[nodiscard]] static D3D12_PRIMITIVE_TOPOLOGY_TYPE sggDrawModeToTopologyType(SGDrawMode drawMode) noexcept;
    };
}

#endif
