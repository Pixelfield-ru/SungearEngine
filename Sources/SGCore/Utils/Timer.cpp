//
// Created by stuka on 03.05.2023.
//

#include <chrono>
#include <GLFW/glfw3.h>

#include "Timer.h"
#include "TimerCallback.h"

void Core::Utils::Timer::startFrame()
{
    if(!m_active) return;

    if(m_firstTime)
    {
        firstTimeStart();
        m_firstTime = false;
    }

    double last = m_current;
    m_current = glfwGetTime();

    m_rawDeltaTime = m_current - last;
    m_fixedDeltaTime += m_current - last;
    m_elapsedTimeForUpdate += m_current - last;

    // if time to update
    if(m_elapsedTimeForUpdate > 1.0 / m_targetFrameRate)
    {
        for(const std::shared_ptr<TimerCallback>& callback : m_callbacks)
        {
            callback->callUpdateFunction();
        }

        m_framesPerTarget++;

        m_elapsedTimeForUpdate = 0.0f;
    }

    if(m_useFixedDeltaTime)
    {
        // fixing delta update
        while(m_fixedDeltaTime > m_fixedDeltaTimeValue)
        {
            for(const std::shared_ptr<TimerCallback>& callback: m_callbacks)
            {
                callback->callDeltaUpdateFunction(m_fixedDeltaTime);
            }
            m_fixedDeltaTime -= m_fixedDeltaTimeValue;
        }
    }
    else // if delta time is not fixed
    {
        for(const std::shared_ptr<TimerCallback>& callback: m_callbacks)
        {
            callback->callDeltaUpdateFunction(m_rawDeltaTime);
        }
    }

    m_elapsedTime = m_current - m_startTime;

    if(m_elapsedTime > m_target)
    {
        for(const std::shared_ptr<TimerCallback>& callback : m_callbacks)
        {
            callback->callDestinationReachedFunction();
        }

        reset();

        m_active = m_cyclic;
    }
}

void Core::Utils::Timer::reset() noexcept
{
    m_elapsedTime = 0;
    m_startTime = glfwGetTime();
    m_framesPerTarget = 0;
}

void Core::Utils::Timer::firstTimeStart()
{
    m_startTime = glfwGetTime();

    for(const std::shared_ptr<TimerCallback>& callback : m_callbacks)
    {
        callback->callStartFunction();
    }
}

void Core::Utils::Timer::addCallback(std::shared_ptr<TimerCallback> callback)
{
    m_callbacks.push_back(std::move(callback));
}

void Core::Utils::Timer::removeCallback(const std::shared_ptr<TimerCallback>& callback)
{
    m_callbacks.remove(callback);
}

uint16_t Core::Utils::Timer::getFramesPerDestination() const noexcept
{
    return m_framesPerTarget;
}