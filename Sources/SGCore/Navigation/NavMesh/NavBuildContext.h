//
// Created by stuka on 21.06.2026.
//

#pragma once

#include <array>
#include <chrono>

#include <Recast.h>

namespace SGCore
{
    struct NavBuildContext final : rcContext
    {
        void doResetLog() override;
        void doLog(rcLogCategory category, const char* msg, int len) override;
        void doResetTimers() override;
        void doStartTimer(rcTimerLabel label) override;
        void doStopTimer(rcTimerLabel label) override;
        [[nodiscard]] int doGetAccumulatedTime(rcTimerLabel label) const override;

    private:
        std::array<std::chrono::high_resolution_clock::time_point, RC_MAX_TIMERS> m_timers {};
        std::array<std::uint64_t, RC_MAX_TIMERS> m_accum {};
    };
}
