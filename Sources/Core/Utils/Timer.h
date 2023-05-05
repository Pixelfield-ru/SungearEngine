//
// Created by stuka on 03.05.2023.
//

#pragma once

#ifndef NATIVECORE_TIMER_H
#define NATIVECORE_TIMER_H

#include "TimerCallback.h"
#include <iostream>
#include <memory>
#include <list>

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

        std::list<std::shared_ptr<TimerCallback>> callbacks;
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

        void firstTimeStart();

        void addCallback(const std::shared_ptr<TimerCallback>& callback);
        void removeCallback(const std::shared_ptr<TimerCallback>& callback);
    };
}


#endif //NATIVECORE_TIMER_H
