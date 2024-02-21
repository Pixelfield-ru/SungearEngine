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
void SGUtils::Timer::startFrame()
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

    if(m_elapsedTimeForUpdate >= destDeltaTime)
    {
        for(const auto& callback: m_callbacks)
        {
            callback->callUpdateFunction(m_elapsedTimeForUpdate);
        }

        m_elapsedTimeForUpdate = 0.0;
        m_framesPerTarget++;
    }

    m_elapsedTime = m_current - m_startTime;
    
    if(m_elapsedTime >= m_targetFrameTime)
    {
        if(m_useFixedUpdateCatchUp)
        {
            while(m_elapsedTime >= m_targetFrameTime)
            {
                if(!m_active) break;

                m_lastFixedUpdateCallTime = m_currentFixedUpdateCallTime;
                m_currentFixedUpdateCallTime = glfwGetTime();
                m_fixedUpdateCallDeltaTime = m_currentFixedUpdateCallTime - m_lastFixedUpdateCallTime;
                
                for(const auto& callback: m_callbacks)
                {
                    callback->callFixedUpdateFunction(m_fixedUpdateCallDeltaTime, m_targetFrameTime);
                }

                m_elapsedTime -= m_targetFrameTime;
            }
        }
        else
        {
            m_lastFixedUpdateCallTime = m_currentFixedUpdateCallTime;
            m_currentFixedUpdateCallTime = glfwGetTime();
            m_fixedUpdateCallDeltaTime = m_currentFixedUpdateCallTime - m_lastFixedUpdateCallTime;
            
            for(const auto& callback: m_callbacks)
            {
                callback->callFixedUpdateFunction(m_fixedUpdateCallDeltaTime, m_targetFrameTime);
            }
        }

        reset();

        m_active = m_cyclic;
    }
}

void SGUtils::Timer::reset() noexcept
{
    m_elapsedTime = 0;
    m_startTime = glfwGetTime();
    m_currentFixedUpdateCallTime = glfwGetTime();
    m_lastFixedUpdateCallTime = m_currentFixedUpdateCallTime;
    m_framesPerTarget = 0;
}

void SGUtils::Timer::firstTimeStart()
{
    m_startTime = glfwGetTime();
    //m_current = m_startTime;

    for(const std::shared_ptr<TimerCallback>& callback : m_callbacks)
    {
        callback->callStartFunction();
    }
}

void SGUtils::Timer::addCallback(std::shared_ptr<TimerCallback> callback)
{
    m_callbacks.push_back(std::move(callback));
}

void SGUtils::Timer::removeCallback(const std::shared_ptr<TimerCallback>& callback)
{
    m_callbacks.remove(callback);
}

uint16_t SGUtils::Timer::getFramesPerTarget() const noexcept
{
    return m_framesPerTarget;
}

double SGUtils::Timer::getRawDeltaTime() const noexcept
{
    return m_rawDeltaTime;
}
