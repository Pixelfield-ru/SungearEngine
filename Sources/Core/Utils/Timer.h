//
// Created by stuka on 03.05.2023.
//

#ifndef NATIVECORE_TIMER_H
#define NATIVECORE_TIMER_H

#include "../Observer/IObserver.h"
#include "../Observer/IObservable.h"

namespace Core::Utils
{
    class Timer : protected Observer::IObservable
    {
    public:
        int a;
        void notifyObservers() override;
    };
}


#endif //NATIVECORE_TIMER_H
