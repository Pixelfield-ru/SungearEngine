//
// Created by stuka on 14.10.2026.
//

#pragma once

#include <array>

#include "pixelfield/runtime/system/system.h"
#include "pixelfield/runtime/frame_time.h"

namespace pf::runtime
{
    struct FrameTimeSystem final : ISystem
    {
        [[nodiscard]] std::span<const std::uint32_t> reads() const noexcept override;
        [[nodiscard]] std::span<const std::uint32_t> writes() const noexcept override;

        std::error_code execute(SystemContext& context) noexcept override;
        std::error_code initialize(SystemContext& context) noexcept override;

    private:
        static inline std::array read_ids_ { core::typeIndex<std::chrono::steady_clock::time_point>() };
        static inline std::array write_ids_ { core::typeIndex<FrameTime>() };

        std::chrono::steady_clock::time_point last_point_ {};
    };
}
