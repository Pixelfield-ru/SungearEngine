//
// Created by stuka on 06.05.2023.
//

#include "TimerCallback.h"

void SGUtils::TimerCallback::setStartFunction(std::function<void()>&& function) noexcept
{
    m_startFunction = function;
}

void SGUtils::TimerCallback::setFixedUpdateFunction(std::function<void()>&& function) noexcept
{
    m_fixedUpdateFunction = function;
}

void SGUtils::TimerCallback::setUpdateFunction(std::function<void()>&& function) noexcept
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

void SGUtils::TimerCallback::callFixedUpdateFunction()
{
    if(m_fixedUpdateFunction)
    {
        m_fixedUpdateFunction();
    }
}

void SGUtils::TimerCallback::callUpdateFunction()
{
    if(m_updateFunction)
    {
        m_updateFunction();
    }
}
