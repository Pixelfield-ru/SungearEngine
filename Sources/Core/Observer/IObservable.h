//
// Created by stuka on 03.05.2023.
//

#pragma once

#ifndef NATIVECORE_IOBSERVABLE_H
#define NATIVECORE_IOBSERVABLE_H

#include <iostream>
#include <list>
#include <memory>
#include <algorithm>

#include "IObserver.h"

namespace Core::Observer
{
    class IObservable
    {
    protected:
        std::list<std::shared_ptr<IObserver>> observers;

    public:
        virtual void notifyObservers() { };

        virtual void addObserver(IObserver* observer)
        {
            observers.push_back(std::shared_ptr<IObserver>(observer));
        }
        // TODO: потом сделать
        /*
        virtual void removeObserver(IObserver* observer)
        {

        }
         */
    };
}

#endif //NATIVECORE_IOBSERVABLE_H
