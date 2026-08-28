//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <cstdint>
#include <memory>
#include <string>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IGPUObject.h"
#include "SGCore/Main/CoreGlobals.h"
#include "DX12Context.h"

namespace SGCore
{
    struct DX12TextureDesc
    {
        std::uint32_t m_width = 1;
        std::uint32_t m_height = 1;
        DXGI_FORMAT m_format = DXGI_FORMAT_R8G8B8A8_UNORM;
        std::uint32_t m_mipLevels = 1;
        std::uint32_t m_arrayLayers = 1;
        std::uint32_t m_samples = 1;
        /// Usable as a colour render target / as a depth stencil target. A depth target is created
        /// with the typeless format so that it can be sampled as well.
        bool m_renderTarget { };
        bool m_depthStencil { };
        D3D12_FILTER m_filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        D3D12_TEXTURE_ADDRESS_MODE m_addressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        /// Cube map: six array layers behind a TEXTURECUBE view (skybox, reflections).
        bool m_cube { };
        std::string m_debugName;
    };

    /**
     * An ID3D12Resource used as an image, with the CPU-side resource state tracker and its views.
     * The DX12 twin of VulkanTexture: recordTransition() is the barrier VulkanTexture emits for a
     * layout change, and the resource state plays the role of the layout.
     *
     * Shader resource and sampler descriptors are not kept here: they are written straight into the
     * shader-visible heap of the draw that needs them (see DX12CommandList::materializeSet), so a
     * texture only owns its RTV / DSV slots.
     */
    class SGCORE_EXPORT DX12Texture final : public IGPUObject
    {
    public:
        DX12Texture(const DX12Texture&) = delete;
        DX12Texture& operator=(const DX12Texture&) = delete;
        ~DX12Texture() override;

        /// Creates an owned image on the default heap, in the COMMON state.
        [[nodiscard]] static Ref<DX12Texture> create(const std::shared_ptr<DX12Context>& context, const DX12TextureDesc& desc) noexcept;

        /// Wraps a resource somebody else owns (a swapchain buffer). The state must be the one the
        /// resource is actually in.
        [[nodiscard]] static Ref<DX12Texture> wrap(const std::shared_ptr<DX12Context>& context, ID3D12Resource* resource,
                                                   DXGI_FORMAT format, std::uint32_t width, std::uint32_t height,
                                                   D3D12_RESOURCE_STATES state, const std::string& debugName) noexcept;

        /// View descriptions the command list writes into the shader-visible heap of a draw.
        [[nodiscard]] static D3D12_SAMPLER_DESC defaultSamplerDesc() noexcept;
        /// A defined null view for a binding nothing was bound to.
        [[nodiscard]] static const D3D12_SHADER_RESOURCE_VIEW_DESC& nullSRVDesc() noexcept;
        [[nodiscard]] D3D12_SHADER_RESOURCE_VIEW_DESC getSRVDesc() const noexcept;
        [[nodiscard]] D3D12_SAMPLER_DESC getSamplerDesc() const noexcept;

        /// Records a transition barrier and updates the tracker. No-op when already in newState.
        void recordTransition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState) noexcept;

        /// The render target view, created on first use. Zero handle for a depth format.
        [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getRTV() noexcept;
        /// The depth stencil view, created on first use. Zero handle for a colour format.
        [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getDSV() noexcept;

        [[nodiscard]] ID3D12Resource* getResource() const noexcept { return m_resource.Get(); }
        [[nodiscard]] DXGI_FORMAT getFormat() const noexcept { return m_format; }
        [[nodiscard]] std::uint32_t getWidth() const noexcept { return m_width; }
        [[nodiscard]] std::uint32_t getHeight() const noexcept { return m_height; }
        [[nodiscard]] D3D12_RESOURCE_STATES getState() const noexcept { return m_state; }
        [[nodiscard]] bool isDepth() const noexcept;
        [[nodiscard]] std::uint32_t getMipLevels() const noexcept { return m_mipLevels; }
        [[nodiscard]] bool isCube() const noexcept { return m_cube; }
        [[nodiscard]] const std::shared_ptr<DX12Context>& getContext() const noexcept { return m_context; }

        /// Forces the tracker; for a resource whose state changed outside this object.
        void setState(D3D12_RESOURCE_STATES state) noexcept { m_state = state; }

    private:
        DX12Texture() = default;

        std::shared_ptr<DX12Context> m_context;
        DX12Ptr<ID3D12Resource> m_resource;
        DXGI_FORMAT m_format = DXGI_FORMAT_UNKNOWN;
        std::uint32_t m_width = 1;
        std::uint32_t m_height = 1;
        D3D12_RESOURCE_STATES m_state = D3D12_RESOURCE_STATE_COMMON;
        std::uint32_t m_rtvIndex = DX12DescriptorHeap::invalid_index;
        std::uint32_t m_dsvIndex = DX12DescriptorHeap::invalid_index;
        std::uint32_t m_mipLevels = 1;
        std::uint32_t m_arrayLayers = 1;
        bool m_cube { };
        D3D12_FILTER m_filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        D3D12_TEXTURE_ADDRESS_MODE m_addressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    };
}

#endif
