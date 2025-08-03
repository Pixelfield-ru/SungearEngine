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
        static void process() noexcept;

        static void addYieldedCoro(std::coroutine_handle<> coro) noexcept;

        static void addTimerAwaitableCoro(TimerAwaitable task) noexcept;

        static void addTaskAwaitableCoro(TaskAwaitable task) noexcept;

    private:
        static inline std::vector<std::coroutine_handle<>> m_yieldedCoros { };
        static inline std::vector<TimerAwaitable> m_timerAwaitableCoros { };
        static inline std::vector<TaskAwaitable> m_taskAwaitableCoros { };
    };
}

#endif // SUNGEARENGINE_CORO_COROSCHEDULER_H
