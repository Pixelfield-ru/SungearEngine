//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <sgcore_export.h>

#include "IGPUObject.h"

namespace SGCore
{
    /// Window presentation surface with frames in flight. On GL: the default framebuffer and
    /// glfwSwapBuffers; on Vulkan/DX12: the swapchain, acquire/present and per-frame fences.
    class SGCORE_EXPORT ISwapchain : public IGPUObject
    {
    public:
        /// Waits for the frame that used this slot framesInFlight ago and acquires the backbuffer.
        virtual bool beginFrame() noexcept = 0;
        /// Presents the current backbuffer and advances the frame index.
        virtual void present() noexcept = 0;

        [[nodiscard]] virtual std::uint32_t getWidth() const noexcept = 0;
        [[nodiscard]] virtual std::uint32_t getHeight() const noexcept = 0;
        /// Index of the frame slot in [0; framesInFlight).
        [[nodiscard]] virtual std::uint32_t getFrameIndex() const noexcept = 0;
        /// Monotonic frame counter since creation.
        [[nodiscard]] virtual std::uint64_t getFrameNumber() const noexcept = 0;
    };
}
