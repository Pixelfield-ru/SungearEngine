//
// Created by stuka on 03.05.2023.
//

#include "Timer.h"

void Core::Utils::Timer::notifyObservers()
{
    for(std::shared_ptr<Observer::IObserver> observer : observers)
    {
        observer->notify(this);
    }
}