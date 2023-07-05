//
// Created by stuka on 03.05.2023.
//

#pragma once

#ifndef NATIVECORE_TIMER_H
#define NATIVECORE_TIMER_H

#include <iostream>
#include <memory>
#include <list>

#include "TimerCallback.h"

namespace Core::Utils
{
    class Timer
    {
    private:
        double m_current = 0;
        double m_startTime = 0;

        double m_elapsedTime = 0;

        double m_rawDeltaTime = 0;

        // fixed delta time for update functions
        double m_elapsedTimeForUpdate = 0;
        // fixed delta time for delta update functions
        double m_fixedDeltaTime = 0;

        bool m_firstTime = true;

        uint16_t m_framesPerTarget = 0;

        std::list<std::shared_ptr<TimerCallback>> m_callbacks;
    public:
        bool m_active = true;
        bool m_cyclic = false;

        double m_target = 0;

        // 7 ms
        double m_fixedDeltaTimeValue = 7.0 / 1000.0;

        double m_targetFrameRate = 120.0;

        // if true:
        // 1) determinism. No matter your FPS, the physics will always run with the same exact steps
        // 2) robustness. Event if you have a long disk access, or some streaming delays, or anything that makes a frame longer, your objects won't jump into outer space.
        // 3) online-play readiness. When we will do online multi-player, we'll have to pick an approach.
        // For many techniques like input-passing, the small steps will be significantly easier to make online.
        bool m_useFixedDeltaTime = true;

        // ------------------------------------

        Timer() noexcept = default;
        explicit Timer(const bool& cyclic) noexcept : m_cyclic(cyclic) { }
        Timer(const bool& cyclic, const double& destination) noexcept : m_cyclic(cyclic), m_target(destination) { }

        void startFrame();

        void reset() noexcept;

        void firstTimeStart();

        void addCallback(std::shared_ptr<TimerCallback> callback);
        void removeCallback(const std::shared_ptr<TimerCallback>& callback);

        [[nodiscard]] std::uint16_t getFramesPerDestination() const noexcept;
    };
}


#endif //NATIVECORE_TIMER_H
