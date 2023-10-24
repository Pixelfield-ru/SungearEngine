//
// Created by stuka on 06.05.2023.
//

#include "TimerCallback.h"

void Core::Utils::TimerCallback::setStartFunction(std::function<void()>&& function) noexcept
{
    m_startFunction = function;
}

void Core::Utils::TimerCallback::setFixedUpdateFunction(std::function<void()>&& function) noexcept
{
    m_fixedUpdateFunction = function;
}

void Core::Utils::TimerCallback::setUpdateFunction(std::function<void()>&& function) noexcept
{
    m_updateFunction = function;
}

void Core::Utils::TimerCallback::setDestinationReachedFunction(std::function<void()>&& function) noexcept
{
    m_destinationReachedFunction = function;
}

// ------------------------------------------------------------------

void Core::Utils::TimerCallback::callStartFunction()
{
    if(m_startFunction)
    {
        m_startFunction();
    }
}

void Core::Utils::TimerCallback::callFixedUpdateFunction()
{
    if(m_fixedUpdateFunction)
    {
        m_fixedUpdateFunction();
    }
}

void Core::Utils::TimerCallback::callUpdateFunction()
{
    if(m_updateFunction)
    {
        m_updateFunction();
    }
}

void Core::Utils::TimerCallback::callDestinationReachedFunction()
{
    if(m_destinationReachedFunction)
    {
        m_destinationReachedFunction();
    }
}
