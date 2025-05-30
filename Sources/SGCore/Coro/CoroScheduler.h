//
// Created by stuka on 18.05.2025.
//

#ifndef SUNGEARENGINE_CORO_COROSCHEDULER_H
#define SUNGEARENGINE_CORO_COROSCHEDULER_H

#include <sgcore_export.h>
#include <coroutine>
#include <vector>

namespace SGCore::Coro
{
    struct TimerAwaitable;
    struct TaskAwaitable;

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

// including here to avoid cyclic includes (CoroScheduler includes TaskAwaitable, TaskAwaitable includes Task, Task includes CoroScheduler)

#include "TimerAwaitable.h"
#include "TaskAwaitable.h"

inline void SGCore::Coro::CoroScheduler::process() noexcept
{
    {
        auto lastVec = m_yieldedCoros;
        m_yieldedCoros.clear();

        for(auto& awaitable : lastVec)
        {
            if(!awaitable.done())
            {
                awaitable.resume();
            }
        }
    }

    {
        std::vector<std::coroutine_handle<>> corosToResume { };

        auto it = m_timerAwaitableCoros.begin();
        while(it != m_timerAwaitableCoros.end())
        {
            auto& awaitable = *it;
            if(awaitable.await_ready())
            {
                auto coro = awaitable.getOwnerCoro();
                it = m_timerAwaitableCoros.erase(it);
                corosToResume.push_back(coro);
                continue;
            }

            ++it;
        }

        for(auto& coro : corosToResume) coro.resume();
    }

    {
        std::vector<std::coroutine_handle<>> corosToResume { };

        auto it = m_taskAwaitableCoros.begin();
        while(it != m_taskAwaitableCoros.end())
        {
            auto& awaitable = *it;
            if(awaitable.await_ready())
            {
                auto coro = awaitable.getOwnerCoro();
                it = m_taskAwaitableCoros.erase(it);
                corosToResume.push_back(coro);
                continue;
            }

            ++it;
        }

        for(auto& coro : corosToResume) coro.resume();
    }
}

inline void SGCore::Coro::CoroScheduler::addYieldedCoro(std::coroutine_handle<> coro) noexcept
{
    m_yieldedCoros.push_back(coro);
}

inline void SGCore::Coro::CoroScheduler::addTimerAwaitableCoro(TimerAwaitable task) noexcept
{
    m_timerAwaitableCoros.push_back(task);
}

inline void SGCore::Coro::CoroScheduler::addTaskAwaitableCoro(TaskAwaitable task) noexcept
{
    m_taskAwaitableCoros.push_back(task);
}

#endif // SUNGEARENGINE_CORO_COROSCHEDULER_H
