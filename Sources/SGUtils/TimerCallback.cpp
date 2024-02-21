//
// Created by stuka on 06.05.2023.
//

#include "TimerCallback.h"

void SGUtils::TimerCallback::setStartFunction(std::function<void()>&& function) noexcept
{
    m_startFunction = function;
}

void SGUtils::TimerCallback::setFixedUpdateFunction(std::function<void(const double& dt, const double& fixedDt)>&& function) noexcept
{
    m_fixedUpdateFunction = function;
}

void SGUtils::TimerCallback::setUpdateFunction(std::function<void(const double& dt)>&& function) noexcept
{
    m_updateFunction = function;
}

// ------------------------------------------------------------------

void SGUtils::TimerCallback::callStartFunction()
{
    if(m_startFunction)
    {
        m_startFunction();
    }
}

void SGUtils::TimerCallback::callFixedUpdateFunction(const double& dt, const double& fixedDt)
{
    if(m_fixedUpdateFunction)
    {
        m_fixedUpdateFunction(dt, fixedDt);
    }
}

void SGUtils::TimerCallback::callUpdateFunction(const double& dt)
{
    if(m_updateFunction)
    {
        m_updateFunction(dt);
    }
}
