//
// Created by stuka on 03.05.2023.
//

#pragma once

#ifndef NATIVECORE_TIMER_H
#define NATIVECORE_TIMER_H

#include <iostream>
#include <memory>
#include <list>
#include <cstdint>

#include "TimerCallback.h"

namespace SGCore
{
    // TODO: documentation
    class Timer
    {
    public:
        bool m_active = true;
        bool m_cyclic = false;
        
        // ------------------------------------

        Timer() noexcept = default;
        explicit Timer(const bool& cyclic) noexcept : m_cyclic(cyclic) { }
        Timer(const bool& cyclic, const double& frameRate) noexcept
        {
            m_cyclic = cyclic;
            m_targetFrameRate = frameRate;
            m_targetFrameTime = 1.0 / m_targetFrameRate;
        }

        void startFrame();
        //void endFrame();

        void resetTimer() noexcept;

        void firstTimeStart();

        void addCallback(const std::shared_ptr<TimerCallback>& callback);
        void removeCallback(const std::shared_ptr<TimerCallback>& callback);

        [[nodiscard]] std::uint16_t getFramesPerSecond() const noexcept;
        [[nodiscard]] double getRawDeltaTime() const noexcept;
        
        [[nodiscard]] double getTargetFrameRate() const noexcept;
        void setTargetFrameRate(const double& frameRate) noexcept;
        
        [[nodiscard]] double getTargetFrameTime() const noexcept;
        
        [[nodiscard]] double getFixedUpdateCallDeltaTime() const noexcept
        {
            return m_fixedUpdateCallDeltaTime;
        }

    private:
        double m_FPSDeltaTimeAccum = 0.0;
        
        double m_targetFrameRate = 120.0;
        
        double m_targetFrameTime = 1 / m_targetFrameRate;

        double m_fixedUpdateCallDeltaTime = 0.0;
        
        double m_currentTime = 0;

        double m_rawDeltaTime = 0;

        // fixed delta time for update functions
        double m_elapsedTimeForUpdate = 0;

        bool m_firstTime = true;

        uint16_t m_framesPerSecond = 0;
        uint16_t m_framesPerSecondAccum = 0;

        std::list<std::shared_ptr<TimerCallback>> m_callbacks;
    };
}


#endif //NATIVECORE_TIMER_H
