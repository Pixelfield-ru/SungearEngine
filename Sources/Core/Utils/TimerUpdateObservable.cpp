//
// Created by stuka on 03.05.2023.
//

#include "TimerUpdateObservable.h"

#include "Timer.h"

void Core::Utils::TimerUpdateObservable::notify(void* timerVoid)
{
    Timer* timer = static_cast<Timer*>(timerVoid);
    timer->a;
}
