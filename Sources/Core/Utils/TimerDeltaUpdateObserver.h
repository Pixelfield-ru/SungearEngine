//
// Created by stuka on 03.05.2023.
//

#ifndef NATIVECORE_TIMERDELTAUPDATEOBSERVER_H
#define NATIVECORE_TIMERDELTAUPDATEOBSERVER_H

#include "../Patterns/Observer.h"

namespace Core::Utils
{
    class TimerDeltaUpdateObserver : public Observer::IObserver
    {
    public:
        void notify(Observer::IObservable*) override;
    };
}


#endif //NATIVECORE_TIMERDELTAUPDATEOBSERVER_H
