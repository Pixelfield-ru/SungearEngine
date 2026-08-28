//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include "SGCore/Graphics/RHI/ISwapchain.h"

namespace SGCore
{
    /// The window's default framebuffer. One frame in flight: GL serializes for us.
    class GL46Swapchain final : public ISwapchain
    {
    public:
        bool beginFrame() noexcept override;
        void present() noexcept override;

        [[nodiscard]] std::uint32_t getWidth() const noexcept override;
        [[nodiscard]] std::uint32_t getHeight() const noexcept override;
        [[nodiscard]] std::uint32_t getFrameIndex() const noexcept override { return 0; }
        [[nodiscard]] std::uint64_t getFrameNumber() const noexcept override { return m_frameNumber; }

    private:
        std::uint64_t m_frameNumber { };
    };
}
