//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <cstdint>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/ISwapchain.h"
#include "SGCore/Main/CoreGlobals.h"
#include "DX12Context.h"
#include "DX12Texture.h"

struct GLFWwindow;

namespace SGCore
{
    class DX12Device;

    /**
     * The DXGI swapchain over the GLFW window (HWND through glfwGetWin32Window). Flip-model,
     * vsynced, and shaped exactly like VulkanSwapchain so both explicit backends behave the same:
     * a back buffer is taken lazily by the first render pass that targets the window
     * (ensureAcquired()), and present() transitions it to PRESENT and hands it over.
     *
     * There is no acquire semaphore here — DXGI hands the index out synchronously — so the frames
     * in flight are bounded by waiting on the fence value of the submission that last presented this
     * buffer, which is why the buffer count equals the frames in flight.
     */
    class SGCORE_EXPORT DX12Swapchain final : public ISwapchain
    {
    public:
        static constexpr std::uint32_t frames_in_flight = 2;
        static constexpr DXGI_FORMAT swapchain_format = DXGI_FORMAT_R8G8B8A8_UNORM;

        DX12Swapchain(DX12Device& device, GLFWwindow* window) noexcept;
        ~DX12Swapchain() override;

        [[nodiscard]] bool isValid() const noexcept { return m_swapchain != nullptr && !m_images.empty(); }

        // ---- ISwapchain
        bool beginFrame() noexcept override;
        void present() noexcept override;
        [[nodiscard]] std::uint32_t getWidth() const noexcept override { return m_width; }
        [[nodiscard]] std::uint32_t getHeight() const noexcept override { return m_height; }
        [[nodiscard]] std::uint32_t getFrameIndex() const noexcept override { return m_bufferIndex; }
        [[nodiscard]] std::uint64_t getFrameNumber() const noexcept override { return m_frameNumber; }

        // ---- backend
        /// Takes the current back buffer unless one is already held for this frame, waiting until the
        /// submission that last presented it has finished. False when the swapchain is unusable.
        bool ensureAcquired() noexcept;
        [[nodiscard]] bool isAcquired() const noexcept { return m_acquired; }
        /// The back buffer being rendered into (nullptr when none is held).
        [[nodiscard]] Ref<DX12Texture> getCurrentTexture() const noexcept;
        [[nodiscard]] DXGI_FORMAT getFormat() const noexcept { return swapchain_format; }

        /// (Re)creates the swapchain for the current window size; also used after a resize.
        bool recreate() noexcept;

    private:
        void releaseImages() noexcept;
        [[nodiscard]] bool queryWindowSize(std::uint32_t& outWidth, std::uint32_t& outHeight) const noexcept;

        DX12Device& m_device;
        GLFWwindow* m_window { };
        HWND m_hwnd { };

        DX12Ptr<IDXGISwapChain4> m_swapchain;
        std::vector<Ref<DX12Texture>> m_images;
        /// Id of the submission that last presented the buffer with the same index.
        std::vector<std::uint64_t> m_lastSubmissions;

        std::uint32_t m_width { };
        std::uint32_t m_height { };
        std::uint32_t m_bufferIndex { };
        std::uint64_t m_frameNumber { };

        bool m_acquired { };
        bool m_needsRecreate { };
    };
}

#endif
