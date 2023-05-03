//
// Created by stuka on 03.05.2023.
//

#pragma once

#ifndef NATIVECORE_TIMER_H
#define NATIVECORE_TIMER_H

#include "../Patterns/Observer.h"

#include "TimerUpdateObserver.h"
#include "TimerDeltaUpdateObserver.h"

namespace Core::Utils
{
    class Timer : public Observer::IObservable
    {
    private:
        long double m_current = 0L;
        long double m_startTime = 0L;

        long double m_progress = 0L;

        long double m_deltaTime = 0L;

        bool m_firstTime;
    public:
        bool m_active = true;
        bool m_cyclic = false;

        long double m_destination = 0L;

        // ------------------------------------

        Timer() noexcept = default;
        Timer(const bool& cyclic, const long double& destination) noexcept : m_cyclic(cyclic), m_destination(destination) { }

        void notifyObservers() override;

        void startFrame() noexcept;

        void firstTimeStart();

        void endFrame();
    };
}


#endif //NATIVECORE_TIMER_H
