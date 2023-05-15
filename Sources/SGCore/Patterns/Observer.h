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

namespace Core::Observer
{
    class IObservable;

    class IObserver
    {
    public:
        virtual void notify(IObservable*) { };
    };

    class IObservable
    {
    protected:
        std::list<std::shared_ptr<IObserver>> observers;

    public:
        virtual void notifyObservers() { };

        void addObserver(const std::shared_ptr<IObserver>& observer)
        {
            observers.push_back(observer);
        }
        void removeObserver(const std::shared_ptr<IObserver>& observer)
        {
            observers.remove(observer);
        }
    };
}

#endif //NATIVECORE_IOBSERVABLE_H
