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
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    // TODO: documentation
    class Timer
    {
    public:
        bool m_active = true;
        bool m_cyclic = false;
        bool m_useFixedUpdateCatchUp = true;

        double m_target = 0;

        double m_targetFrameRate = 120.0;

        // ------------------------------------

        Timer() noexcept = default;
        explicit Timer(const bool& cyclic) noexcept : m_cyclic(cyclic) { }
        Timer(const bool& cyclic, const double& destination) noexcept : m_cyclic(cyclic), m_target(destination) { }

        void startFrame();
        //void endFrame();

        void reset() noexcept;

        void firstTimeStart();

        void addCallback(Ref<TimerCallback> callback);
        void removeCallback(const Ref<TimerCallback>& callback);

        [[nodiscard]] std::uint16_t getFramesPerDestination() const noexcept;
        [[nodiscard]] double getRawDeltaTime() const noexcept;

    private:
        double m_current = 0;
        double m_startTime = 0;

        double m_elapsedTime = 0;

        double m_rawDeltaTime = 0;

        // fixed delta time for update functions
        double m_elapsedTimeForUpdate = 0;

        bool m_firstTime = true;

        uint16_t m_framesPerTarget = 0;

        std::list<Ref<TimerCallback>> m_callbacks;
    };
}


#endif //NATIVECORE_TIMER_H
