//
// Created by stuka on 18.05.2025.
//

#ifndef SUNGEARENGINE_CORO_TASKAWAITABLE_H
#define SUNGEARENGINE_CORO_TASKAWAITABLE_H

#include "IAwaitable.h"

namespace SGCore::Coro
{
    template<typename>
    struct Task;

    struct TaskAwaitableBase : IAwaitable
    {
        explicit TaskAwaitableBase(std::coroutine_handle<> calledCoro)
        {
            m_calledCoro = calledCoro;
        }

        bool await_ready() const noexcept;

        void await_suspend(std::coroutine_handle<> thisCoroutine);
    };

    template<typename T>
    struct TaskAwaitable : TaskAwaitableBase
    {
        explicit TaskAwaitable(const Task<T>& calledTask) : TaskAwaitableBase(calledTask.getHandle())
        {
            m_calledTask = &calledTask;
        }

        T await_resume() noexcept
        {
            if constexpr(std::is_same_v<T, void>)
            {
                return;
            }
            else
            {
                return m_calledTask->getHandle().promise().get_value();
            }
        }

    private:
        const Task<T>* m_calledTask {};
    };
}

template<typename T>
auto operator co_await(const SGCore::Coro::Task<T>& otherTask)
{
    return SGCore::Coro::TaskAwaitable<T>(otherTask);
}

namespace SGCore::Coro
{
    /// Dummy function to return to caller
    Task<bool> returnToCaller() noexcept;
}

#endif // SUNGEARENGINE_CORO_TASKAWAITABLE_H
