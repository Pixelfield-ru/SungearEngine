//
// Created by stuka on 06.05.2023.
//

#include "TimerCallback.h"

void SGCore::TimerCallback::setStartFunction(std::function<void()>&& function) noexcept
{
    m_startFunction = function;
}

void SGCore::TimerCallback::setFixedUpdateFunction(std::function<void()>&& function) noexcept
{
    m_fixedUpdateFunction = function;
}

void SGCore::TimerCallback::setUpdateFunction(std::function<void()>&& function) noexcept
{
    m_updateFunction = function;
}

// ------------------------------------------------------------------

void SGCore::TimerCallback::callStartFunction()
{
    if(m_startFunction)
    {
        m_startFunction();
    }
}

void SGCore::TimerCallback::callFixedUpdateFunction()
{
    if(m_fixedUpdateFunction)
    {
        m_fixedUpdateFunction();
    }
}

void SGCore::TimerCallback::callUpdateFunction()
{
    if(m_updateFunction)
    {
        m_updateFunction();
    }
}
