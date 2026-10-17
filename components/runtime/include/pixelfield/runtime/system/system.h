//
// Created by stuka on 13.10.2026.
//

#pragma once

#include <system_error>
#include <span>
#include <chrono>

#include "pixelfield/core/structures/type_map.h"
#include "pixelfield/runtime/execution/domain.h"

namespace pf::runtime
{
    struct SystemContext final
    {
        core::TypeMap resources;
    };

    enum class SystemScheduleType : std::uint8_t
    {
        kFrame,
        kFixed,
        kRender,
        kSimulation,
        kBackground,
        kAudio
    };

    struct SystemScheduleConfig final
    {
        std::chrono::nanoseconds interval {};
        std::uint32_t max_catch_up_steps = 1;
    };

    struct ISystem
    {
        virtual ~ISystem() = default;

        /**
         * @return Indices of types that the system reads.
         */
        [[nodiscard]] virtual std::span<const std::uint32_t> reads() const noexcept = 0;
        /**
        * @return Indices of types that the system writes.
        */
        [[nodiscard]] virtual std::span<const std::uint32_t> writes() const noexcept = 0;

        /**
        * Execute system once.
        * @param context System context.
        * @return Result code.
        */
        virtual std::error_code execute(SystemContext& context) = 0;

        /**
        * Initialize system.
        * @param context System context.
        * @return Result code.
        */
        virtual std::error_code initialize(SystemContext& context) = 0;

        [[nodiscard]] ExecutionDomain executionDomain() const noexcept
        {
            return execution_domain_;
        }

        [[nodiscard]] SystemScheduleType schedule() const noexcept
        {
            return schedule_;
        }

    protected:
        ExecutionDomain execution_domain_ = ExecutionDomain::kAny;
        SystemScheduleType schedule_ = SystemScheduleType::kFrame;
    };
}
