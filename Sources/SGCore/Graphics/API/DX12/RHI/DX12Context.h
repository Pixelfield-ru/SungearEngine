//
// Created by 8bitniksis on 18.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <cstdint>
#include <memory>
#include <string>

#include <sgcore_export.h>

#include "SGCore/Graphics/API/DX12/DX12Common.h"
#include "DX12DescriptorHeap.h"

namespace SGCore
{
    /// Everything that exists exactly once per process for the DX12 backend: the DXGI factory, the
    /// adapter, the device and the direct queue. Mirrors VulkanContext, including its lifetime rule —
    /// it is owned through a shared_ptr and GPU-owning objects keep a shared reference, because
    /// assets outlive the renderer in static destruction.
    struct SGCORE_EXPORT DX12Context : std::enable_shared_from_this<DX12Context>
    {
        DX12Ptr<IDXGIFactory6> m_factory;
        DX12Ptr<IDXGIAdapter4> m_adapter;
        DX12Ptr<ID3D12Device5> m_device;
        DX12Ptr<ID3D12CommandQueue> m_directQueue;
        DX12Ptr<ID3D12InfoQueue> m_infoQueue;
        /// The queue the message callback is registered on (ID3D12InfoQueue1, Windows 10 2004+).
        DX12Ptr<ID3D12InfoQueue1> m_callbackQueue;
        DWORD m_messageCallbackCookie { };
        /// DXGI keeps its own message queue: swapchain and factory errors never reach the D3D12 one.
        DX12Ptr<IDXGIInfoQueue> m_dxgiInfoQueue;

        /// CPU-only view heaps. They live in the context rather than in the device because a texture
        /// owns its slots for as long as it exists, and textures (assets) outlive the renderer — the
        /// same reason VulkanTexture holds a shared_ptr to VulkanContext.
        DX12DescriptorHeap m_rtvHeap;
        DX12DescriptorHeap m_dsvHeap;

        std::string m_adapterName;
        /// Feature level the device was created with; the design fixes 12.0 as the minimum.
        D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_12_0;
        bool m_debugLayerEnabled { };

        /// Debug layer in Debug builds, overridable with SG_DX_VALIDATION=1/0 exactly like
        /// SG_VK_VALIDATION does for Vulkan.
        bool create(bool enableDebugLayer) noexcept;
        void destroy() noexcept;

        [[nodiscard]] bool isReady() const noexcept { return m_device != nullptr; }
        [[nodiscard]] const std::string& getAdapterName() const noexcept { return m_adapterName; }

        /// Names the object for the debug layer and PIX captures (counterpart of setObjectName()).
        void setObjectName(ID3D12Object* object, const std::string& name) const noexcept;

        /// Drains both debug message queues (D3D12 and DXGI) into the engine log. The debug layer
        /// has no callback like VK_EXT_debug_utils, so messages are polled after submissions.
        void drainDebugMessages() noexcept;

    private:
        void drainDXGIMessages() noexcept;

        /// Logs a message the moment the debug layer produces it (ID3D12InfoQueue1 callback).
        static void CALLBACK debugMessageCallback(D3D12_MESSAGE_CATEGORY category, D3D12_MESSAGE_SEVERITY severity,
                                                  D3D12_MESSAGE_ID id, LPCSTR description, void* context);

        [[nodiscard]] bool pickAdapter() noexcept;
    };
}

#endif
