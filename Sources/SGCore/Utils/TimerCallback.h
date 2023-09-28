//
// Created by stuka on 06.05.2023.
//

#ifndef NATIVECORE_TIMERCALLBACK_H
#define NATIVECORE_TIMERCALLBACK_H

//#include "../Main/Callbacks.h"
#include <iostream>
#include <functional>

namespace Core::Utils
{
    class TimerCallback
    {
    private:
        std::function<void()> startFunction;
        std::function<void()> fixedUpdateFunction;
        std::function<void(const long double&)> deltaUpdateFunction;
        std::function<void()> destinationReachedFunction;

    public:
        void setStartFunction(const std::function<void()>&) noexcept;
        void setFixedUpdateFunction(const std::function<void()>&) noexcept;
        void setDeltaUpdateFunction(const std::function<void(const long double&)>&) noexcept;
        void setDestinationReachedFunction(const std::function<void()>&) noexcept;

        void callStartFunction();
        void callFixedUpdateFunction();
        void callDeltaUpdateFunction(const long double&);
        void callDestinationReachedFunction();
    };
}

#endif //NATIVECORE_TIMERCALLBACK_H
