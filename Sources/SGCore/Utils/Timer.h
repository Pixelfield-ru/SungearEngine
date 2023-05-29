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
        long double m_current = 0L;
        long double m_startTime = 0L;

        long double m_progress = 0L;

        long double m_deltaTime = 0L;

        bool m_firstTime = true;

        uint16_t m_framesPerDestination = 0;

        std::list<std::shared_ptr<TimerCallback>> m_callbacks;
    public:
        bool m_active = true;
        bool m_cyclic = false;

        long double m_destination = 0L;

        // ------------------------------------

        Timer() noexcept = default;
        explicit Timer(const bool& cyclic) noexcept : m_cyclic(cyclic) { }
        Timer(const bool& cyclic, const long double& destination) noexcept : m_cyclic(cyclic), m_destination(destination) { }

        void startFrame() noexcept;

        void endFrame();

        void reset() noexcept;

        void firstTimeStart();

        void addCallback(std::shared_ptr<TimerCallback> callback);
        void removeCallback(const std::shared_ptr<TimerCallback>& callback);

        const std::uint16_t& getFramesPerDestination() const noexcept;
    };
}


#endif //NATIVECORE_TIMER_H
