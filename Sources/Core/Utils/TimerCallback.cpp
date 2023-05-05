//
// Created by stuka on 06.05.2023.
//

#include "TimerCallback.h"

void Core::Utils::TimerCallback::setStartFunction(const std::function<void()>& function) noexcept
{
    startFunction = function;
}

void Core::Utils::TimerCallback::setUpdateFunction(const std::function<void()>& function) noexcept
{
    updateFunction = function;
}

void Core::Utils::TimerCallback::setDeltaUpdateFunction(const std::function<void(const long double&)>& function) noexcept
{
    deltaUpdateFunction = function;
}

void Core::Utils::TimerCallback::setDestinationReachedFunction(const std::function<void()>& function) noexcept
{
    destinationReachedFunction = function;
}

// ------------------------------------------------------------------

void Core::Utils::TimerCallback::callStartFunction()
{
    if(updateFunction)
    {
        startFunction();
    }
}

void Core::Utils::TimerCallback::callUpdateFunction()
{
    if(updateFunction)
    {
        updateFunction();
    }
}

void Core::Utils::TimerCallback::callDeltaUpdateFunction(const long double& deltaTime)
{
    if(deltaUpdateFunction)
    {
        deltaUpdateFunction(deltaTime);
    }
}

void Core::Utils::TimerCallback::callDestinationReachedFunction()
{
    if(destinationReachedFunction)
    {
        destinationReachedFunction();
    }
}
