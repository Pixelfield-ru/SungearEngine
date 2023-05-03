//
// Created by stuka on 03.05.2023.
//

#ifndef NATIVECORE_TIMERUPDATEOBSERVER_H
#define NATIVECORE_TIMERUPDATEOBSERVER_H

#include "../Patterns/Observer.h"
//#include "Timer.h"

namespace Core::Utils
{
    class TimerUpdateObserver : public Observer::IObserver
    {
    public:
        void notify(Observer::IObservable*) override;
        // TODO: ещё подумать на observer паттерном, подумать над классом обновления цикла отрисовки как класса, наследующимся от IObserver (вроде норм, но хз)
    };
}


#endif //NATIVECORE_TIMERUPDATEOBSERVER_H
