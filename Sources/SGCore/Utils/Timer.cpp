//
// Created by stuka on 03.05.2023.
//

#include <chrono>
#include <GLFW/glfw3.h>
#include <numeric>

#include "Timer.h"
#include "TimerCallback.h"

// TODO:  FIX rawDeltaTime between two startFrame
// timer.startFrame();
// some code...
// timer.startFrame();
// timer.getRawDeltaTime() == glfwGetTime() ! NOT CORRECT !
void SGCore::Timer::startFrame()
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
    m_elapsedTimeForUpdate += m_rawDeltaTime;
    // m_elapsedTimeForUpdate = std::min(m_elapsedTimeForUpdate, 0.5);
    if(m_elapsedTimeForUpdate >= 1.0)
    {
        m_elapsedTimeForUpdate = 0.0;
    }

    double destDeltaTime = 1.0 / m_targetFrameRate;

    if(m_useFixedUpdate)
    {
        while(m_elapsedTimeForUpdate >= destDeltaTime)
        {
            for(const auto& callback : m_callbacks)
            {
                callback->callFixedUpdateFunction();
            }

            m_elapsedTimeForUpdate -= destDeltaTime;
            m_framesPerTarget++;
        }
    }
    else
    {
        if(m_elapsedTimeForUpdate >= destDeltaTime)
        {
            for(const auto& callback : m_callbacks)
            {
                callback->callUpdateFunction();
            }

            m_elapsedTimeForUpdate = 0.0;
            m_framesPerTarget++;
        }
    }

    m_elapsedTime = m_current - m_startTime;

    if(m_elapsedTime >= m_target)
    {
        for(const auto& callback : m_callbacks)
        {
            callback->callDestinationReachedFunction();
        }

        reset();

        m_active = m_cyclic;
    }
}

void SGCore::Timer::reset() noexcept
{
    m_elapsedTime = 0;
    m_startTime = glfwGetTime();
    m_framesPerTarget = 0;
}

void SGCore::Timer::firstTimeStart()
{
    m_startTime = glfwGetTime();
    //m_current = m_startTime;

    for(const std::shared_ptr<TimerCallback>& callback : m_callbacks)
    {
        callback->callStartFunction();
    }
}

void SGCore::Timer::addCallback(std::shared_ptr<TimerCallback> callback)
{
    m_callbacks.push_back(std::move(callback));
}

void SGCore::Timer::removeCallback(const std::shared_ptr<TimerCallback>& callback)
{
    m_callbacks.remove(callback);
}

uint16_t SGCore::Timer::getFramesPerDestination() const noexcept
{
    return m_framesPerTarget;
}

double SGCore::Timer::getRawDeltaTime() const noexcept
{
    return m_rawDeltaTime;
}
