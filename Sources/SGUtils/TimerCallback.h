//
// Created by stuka on 06.05.2023.
//

#ifndef NATIVECORE_TIMERCALLBACK_H
#define NATIVECORE_TIMERCALLBACK_H

//#include "../Main/Callbacks.h"
#include <iostream>
#include <functional>

namespace SGUtils
{
    class TimerCallback
    {
    public:
        void setStartFunction(std::function<void()>&& function) noexcept;
        void setFixedUpdateFunction(std::function<void()>&& function) noexcept;
        void setUpdateFunction(std::function<void()>&& function) noexcept;

        void callStartFunction();
        void callFixedUpdateFunction();
        void callUpdateFunction();

    private:
        std::function<void()> m_startFunction;
        std::function<void()> m_fixedUpdateFunction;
        std::function<void()> m_updateFunction;
    };
}

#endif //NATIVECORE_TIMERCALLBACK_H