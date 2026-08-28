//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <memory>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IGPUBuffer.h"
#include "DX12Context.h"

namespace SGCore
{
    class DX12Device;

    /**
     * An ID3D12Resource used as a buffer. Host-visible buffers live on the UPLOAD heap and stay
     * mapped for their whole life; device-local ones live on the DEFAULT heap and are filled through
     * a staging copy, exactly as VulkanGPUBuffer does with VMA.
     *
     * Unlike images, buffers carry no state tracker: D3D12 promotes a buffer out of COMMON into
     * whatever state a command needs and decays it back when the submission ends. The one place
     * that still needs an explicit barrier is a copy followed, in the same submission, by a read —
     * see DX12CommandList::uploadData().
     */
    class SGCORE_EXPORT DX12GPUBuffer final : public IGPUBuffer
    {
    public:
        DX12GPUBuffer(DX12Device& device, const GPUBufferDesc& desc) noexcept;
        ~DX12GPUBuffer() override;

        [[nodiscard]] void* map(std::uint64_t offset, std::uint64_t size) noexcept override;
        void unmap() noexcept override;
        bool write(const void* data, std::uint64_t size, std::uint64_t offset = 0) noexcept override;

        [[nodiscard]] ID3D12Resource* getResource() const noexcept { return m_resource.Get(); }
        [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS getGPUAddress() const noexcept;
        [[nodiscard]] bool isHostVisible() const noexcept { return m_mapped != nullptr; }
        [[nodiscard]] bool isValid() const noexcept { return m_resource != nullptr; }

        /// Marks the buffer as a typed (texel) buffer — what a GLSL samplerBuffer becomes in HLSL
        /// (Buffer<float4>). The format must not widen 3-channel data: the elements are packed tight.
        void setTexelFormat(DXGI_FORMAT format, std::uint32_t elementsCount) noexcept
        {
            m_texelFormat = format;
            m_texelElements = elementsCount;
        }
        [[nodiscard]] bool isTexelBuffer() const noexcept { return m_texelFormat != DXGI_FORMAT_UNKNOWN; }
        [[nodiscard]] D3D12_SHADER_RESOURCE_VIEW_DESC getTexelSRVDesc() const noexcept;
        [[nodiscard]] void* getMappedPointer() const noexcept { return m_mapped; }
        /// Allocated size, which is the requested one rounded up for constant buffer alignment.
        [[nodiscard]] std::uint64_t getAllocatedSize() const noexcept { return m_allocatedSize; }

    private:
        DX12Device& m_device;
        std::shared_ptr<DX12Context> m_context;
        DX12Ptr<ID3D12Resource> m_resource;
        DXGI_FORMAT m_texelFormat = DXGI_FORMAT_UNKNOWN;
        std::uint32_t m_texelElements { };
        void* m_mapped { };
        std::uint64_t m_allocatedSize { };
    };
}

#endif
