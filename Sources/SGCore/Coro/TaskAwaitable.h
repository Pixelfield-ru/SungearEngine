//
// Created by stuka on 18.05.2025.
//

#ifndef SUNGEARENGINE_CORO_TASKAWAITABLE_H
#define SUNGEARENGINE_CORO_TASKAWAITABLE_H

#include "IAwaitable.h"

namespace SGCore::Coro
{
    struct TaskAwaitable : IAwaitable
    {
        explicit TaskAwaitable(std::coroutine_handle<> calledCoro)
        {
            m_calledCoro = calledCoro;
        }

        bool await_ready() const noexcept;

        void await_resume() noexcept;

        void await_suspend(std::coroutine_handle<> thisCoroutine);
    };

    template<typename>
    struct Task;
}

template<typename T>
auto operator co_await(const SGCore::Coro::Task<T>& otherTask)
{
    return SGCore::Coro::TaskAwaitable { otherTask.getHandle() };
}

namespace SGCore::Coro
{
    /// Dummy function to return to caller
    static Task<void> returnToCaller() noexcept;
}

#endif // SUNGEARENGINE_CORO_TASKAWAITABLE_H
