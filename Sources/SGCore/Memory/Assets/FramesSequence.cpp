//
// Created by stuka on 16.02.2026.
//

#include "FramesSequence.h"

void SGCore::FramesSequence::sortFrames() noexcept
{
    std::ranges::sort(m_frames, [](const auto& f0, const auto& f1) {
        return f0.m_timeStamp < f1.m_timeStamp;
    });
}

std::int64_t SGCore::FramesSequence::findFrameByTime(float time) const noexcept
{
    const std::int64_t framesCount = std::ssize(m_frames);

    if(framesCount == 1)
    {
        return 0;
    }

    for(std::int64_t i = 0; i < framesCount; ++i)
    {
        const auto& frame = m_frames[i];
        auto* nextFrame = (i + 1) < framesCount ? &m_frames[i + 1] : nullptr;

        if(time >= frame.m_timeStamp)
        {
            if(!nextFrame)
            {
                return i;
            }

            if(time >= nextFrame->m_timeStamp)
            {
                continue;
            }

            return i;
        }
    }

    return framesCount - 1;
}

void SGCore::FramesSequence::calculateDelays(float lastFrameDelay) noexcept
{
    const std::int64_t framesCount = std::ssize(m_frames);

    for(size_t i = 0; i < framesCount; ++i)
    {
        auto& frame = m_frames[i];

        if(const auto* nextFrame = (i + 1) < framesCount ? &m_frames[i + 1] : nullptr)
        {
            frame.m_nextFrameDelay = nextFrame->m_timeStamp - frame.m_timeStamp;
            continue;
        }

        // if current frame is last
        frame.m_nextFrameDelay = lastFrameDelay;
    }
}
