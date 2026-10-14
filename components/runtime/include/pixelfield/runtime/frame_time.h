//
// Created by stuka on 14.10.2026.
//

#pragma once

#include <chrono>

namespace pf::runtime
{
    struct FrameTime final
    {
        std::chrono::steady_clock::duration delta {};
        std::chrono::steady_clock::duration elapsed {};
        float scale = 1.0f;
        bool paused {};
        std::uint64_t frame_index {};
    };
}