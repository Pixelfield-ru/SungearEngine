//
// Created by stuka on 06.05.2023.
//

#ifndef NATIVECORE_TIMERCALLBACK_H
#define NATIVECORE_TIMERCALLBACK_H

//#include "../Main/Callbacks.h"
#include <iostream>
#include <functional>

namespace SGCore
{
    class TimerCallback
    {
    public:
        void setStartFunction(std::function<void()>&& function) noexcept;
        void setFixedUpdateFunction(std::function<void(const double& dt, const double& fixedDt)>&& function) noexcept;
        void setUpdateFunction(std::function<void(const double& dt)>&& function) noexcept;

        void callStartFunction();
        void callFixedUpdateFunction(const double& dt, const double& fixedDt);
        void callUpdateFunction(const double& dt);

    private:
        std::function<void()> m_startFunction;
        std::function<void(const double& dt, const double& fixedDt)> m_fixedUpdateFunction;
        std::function<void(const double& dt)> m_updateFunction;
    };
}

#endif //NATIVECORE_TIMERCALLBACK_H
