//
// Created by stuka on 03.05.2023.
//

#include "TimerDeltaUpdateObserver.h"
#include "Timer.h"

void Core::Utils::TimerDeltaUpdateObserver::notify(Observer::IObservable* observable)
{
    auto* timer = dynamic_cast<Timer*>(observable);
}

