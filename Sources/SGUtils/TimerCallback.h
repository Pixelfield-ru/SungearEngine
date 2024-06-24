//
// Created by stuka on 06.05.2023.
//

#ifndef NATIVECORE_TIMERCALLBACK_H
#define NATIVECORE_TIMERCALLBACK_H

//#include "../Main/Callbacks.h"
#include <SGUtils/pch.h>

namespace SGCore
{
    class TimerCallback
    {
    public:
        void setStartFunction(std::function<void()>&& function) noexcept;
        void setUpdateFunction(std::function<void(const double& dt, const double& fixedDt)>&& function) noexcept;

        void callStartFunction();
        void callUpdateFunction(const double& dt, const double& fixedDt);

    private:
        std::function<void()> m_startFunction;
        std::function<void(const double& dt, const double& fixedDt)> m_updateFunction;
    };
}

#endif //NATIVECORE_TIMERCALLBACK_H
