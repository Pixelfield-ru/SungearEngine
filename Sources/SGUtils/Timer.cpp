//
// Created by stuka on 03.05.2023.
//

#include <SGUtils/pch.h>

#include "Timer.h"
#include "TimerCallback.h"
#include "Utils.h"

void SGCore::Timer::startFrame()
{
    if(!m_active || (!m_firstTime && !m_cyclic)) return;
    
    if(m_firstTime)
    {
        firstTimeStart();
        m_firstTime = false;
    }
    
    double lastTime = m_currentTime;
    m_currentTime = (double) SGUtils::Utils::getTimeMilliseconds();
    
    m_rawDeltaTime = (m_currentTime - lastTime) / 1000.0;
    
    m_FPSDeltaTimeAccum += m_rawDeltaTime;
    
    m_elapsedTimeForUpdate += m_rawDeltaTime;

    /*std::cout << "m_currentTime: " << m_currentTime << ", last time : "<< lastTime << ", raw dt : " << m_rawDeltaTime << ", elapsed : "
              << m_elapsedTimeForUpdate << ", target : " << m_targetFrameTime << std::endl;*/

    if(m_elapsedTimeForUpdate >= m_targetFrameTime)
    {
        onUpdate(m_elapsedTimeForUpdate, m_targetFrameTime);
        
        m_elapsedTimeForUpdate = 0.0;
        ++m_framesPerSecondAccum;
    }
    
    if(m_FPSDeltaTimeAccum >= 1.0)
    {
        m_framesPerSecond = m_framesPerSecondAccum;
        m_framesPerSecondAccum = 0;
        m_FPSDeltaTimeAccum = 0.0;
    }
    
    // m_active = m_cyclic;
}

void SGCore::Timer::resetTimer() noexcept
{
    m_FPSDeltaTimeAccum = 0;
    m_currentTime = (double) SGUtils::Utils::getTimeMilliseconds();
    m_elapsedTimeForUpdate = 0.0;
}

void SGCore::Timer::firstTimeStart()
{
    //m_currentTime = m_startTime;
    m_currentTime = (double) SGUtils::Utils::getTimeMilliseconds();
    
    onStart();
}

uint16_t SGCore::Timer::getFramesPerSecond() const noexcept
{
    return m_framesPerSecond;
}

double SGCore::Timer::getRawDeltaTime() const noexcept
{
    return m_rawDeltaTime;
}

double SGCore::Timer::getTargetFrameRate() const noexcept
{
    return m_targetFrameRate;
}

double SGCore::Timer::getTargetFrameTime() const noexcept
{
    return m_targetFrameTime;
}

void SGCore::Timer::setTargetFrameRate(const double& frameRate) noexcept
{
    m_targetFrameRate = frameRate;
    m_targetFrameTime = 1.0 / m_targetFrameRate;
}

void SGCore::Timer::setTargetTime(const double& timeSeconds) noexcept
{
    m_targetFrameRate = 1.0 / timeSeconds;
    m_targetFrameTime = timeSeconds;
}
