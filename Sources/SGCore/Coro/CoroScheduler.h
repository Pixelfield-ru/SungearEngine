//
// Created by stuka on 18.05.2025.
//

#ifndef SUNGEARENGINE_CORO_COROSCHEDULER_H
#define SUNGEARENGINE_CORO_COROSCHEDULER_H

#include <sgcore_export.h>
#include <vector>

#include "TimerAwaitable.h"
#include "TaskAwaitable.h"

namespace SGCore::Coro
{
    struct SGCORE_EXPORT CoroScheduler
    {
        void process() noexcept;

        void addYieldedCoro(std::coroutine_handle<> coro) noexcept;

        void addTimerAwaitableCoro(TimerAwaitable task) noexcept;

        void addTaskAwaitableCoro(TaskAwaitable task) noexcept;

    private:
        std::vector<std::coroutine_handle<>> m_yieldedCoros { };
        std::vector<TimerAwaitable> m_timerAwaitableCoros { };
        std::vector<TaskAwaitable> m_taskAwaitableCoros { };
    };
}

#endif // SUNGEARENGINE_CORO_COROSCHEDULER_H
