//
// Created by stuka on 16.11.2026.
//

#pragma once

#include <sgf/runtime/system/system.h>

#include <array>

struct PlatformPollingSystem final : sgf::runtime::ISystem
{
    PlatformPollingSystem()
    {
        execution_domain_ = sgf::runtime::ExecutionDomain::kMain;
    }

    [[nodiscard]] std::span<const std::uint32_t> reads() const noexcept override
    {
        return read_ids_;
    }

    [[nodiscard]] std::span<const std::uint32_t> writes() const noexcept override
    {
        return {};
    }

    std::error_code execute(sgf::runtime::SystemContext& context) override
    {
        auto& scheduler = context.resources.write<sgf::runtime::SystemScheduler>();

        if(sgf::platform::platform().openedWindowsCount() == 0)
        {
            // stop scheduler. stop program
            scheduler.stop();
        }

        sgf::platform::platform().poll();

        return {};
    }

    std::error_code initialize(sgf::runtime::SystemContext& context) override
    {
        context.resources.emplace<std::chrono::steady_clock::time_point>();

        return {};
    }

private:
    static inline std::array read_ids_ { sgf::core::typeIndex<sgf::runtime::SystemScheduler>() };
    static inline std::array write_ids_ { sgf::core::typeIndex<std::chrono::steady_clock::time_point>() };
};