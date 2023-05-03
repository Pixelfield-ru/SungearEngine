//
// Created by stuka on 03.05.2023.
//

#ifndef NATIVECORE_TIMERUPDATEOBSERVABLE_H
#define NATIVECORE_TIMERUPDATEOBSERVABLE_H

#include "../Observer/IObserver.h"
#include "../Observer/IObservable.h"

namespace Core::Utils
{
    class TimerUpdateObservable : Observer::IObserver
    {
    public:
        void notify(void*) override;
        // TODO: ещё подумать на observer паттерном, подумать над классом обновления цикла отрисовки как класса, наследующимся от IObserver (вроде норм, но хз)
    };
}


#endif //NATIVECORE_TIMERUPDATEOBSERVABLE_H
