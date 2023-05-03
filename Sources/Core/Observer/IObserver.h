//
// Created by stuka on 03.05.2023.
//

#pragma once

#ifndef NATIVECORE_IOBSERVER_H
#define NATIVECORE_IOBSERVER_H

//#include "IObservable.h"

namespace Core::Observer
{
    class IObserver
    {
    public:
        virtual void notify(void*) { };
    };
}

#endif //NATIVECORE_IOBSERVER_H
