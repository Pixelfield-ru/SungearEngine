//
// Created by stuka on 16.02.2026.
//

#pragma once

#include <vector>

#include "Frame.h"

namespace SGCore
{
    struct FramesSequence
    {
        std::vector<Frame> m_frames;

        void sortFrames() noexcept;

        std::int64_t findFrameByTime(float time) const noexcept;
        void calculateDelays(float lastFrameDelay = 0.0f) noexcept;
    };
}