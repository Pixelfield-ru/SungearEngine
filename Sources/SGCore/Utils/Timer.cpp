//
// Created by stuka on 03.05.2023.
//

#include <chrono>
#include <GLFW/glfw3.h>

#include "Timer.h"
#include "TimerCallback.h"

void Core::Utils::Timer::startFrame() noexcept
{
    if(!m_active) return;

    if(m_firstTime)
    {
        firstTimeStart();
        m_firstTime = false;
    }

    m_current = glfwGetTime();
}

void Core::Utils::Timer::endFrame()
{
    if(!m_active) return;

    for(const std::shared_ptr<TimerCallback>& callback : callbacks)
    {
        callback->callUpdateFunction();
    }

    long double last = m_current;
    m_current = glfwGetTime();

    m_deltaTime = m_current - last;

    for(const std::shared_ptr<TimerCallback>& callback : callbacks)
    {
        callback->callDeltaUpdateFunction(m_deltaTime);
    }

    m_progress = m_current - m_startTime;

    m_framesPerDestination++;

    if(m_progress > m_destination)
    {
        for(const std::shared_ptr<TimerCallback>& callback : callbacks)
        {
            callback->callDestinationReachedFunction();
        }

        reset();

        m_active = m_cyclic;
    }
}

void Core::Utils::Timer::reset() noexcept
{
    m_progress = 0;
    m_startTime = glfwGetTime();
    m_framesPerDestination = 0;
}

void Core::Utils::Timer::firstTimeStart()
{
    m_startTime = glfwGetTime();

    for(const std::shared_ptr<TimerCallback>& callback : callbacks)
    {
        callback->callStartFunction();
    }
}

void Core::Utils::Timer::addCallback(const std::shared_ptr<TimerCallback>& callback)
{
    callbacks.push_back(callback);
}

void Core::Utils::Timer::removeCallback(const std::shared_ptr<TimerCallback>& callback)
{
    callbacks.remove(callback);
}

const uint16_t& Core::Utils::Timer::getFramesPerDestination() const noexcept
{
    return m_framesPerDestination;
}