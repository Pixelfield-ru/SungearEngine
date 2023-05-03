//
// Created by stuka on 03.05.2023.
//

#include "Timer.h"

void Core::Utils::TimerUpdateObserver::notify(Observer::IObservable* observable)
{
    auto* timer = dynamic_cast<Timer*>(observable);
}
