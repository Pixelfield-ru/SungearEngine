//
// Created by stuka on 18.05.2025.
//

#ifndef SUNGEARENGINE_CORO_TIMERAWAITABLE_H
#define SUNGEARENGINE_CORO_TIMERAWAITABLE_H

#include "IAwaitable.h"

#include <chrono>

namespace SGCore::Coro
{
    struct TimerAwaitable : IAwaitable
    {
        explicit TimerAwaitable(std::chrono::system_clock::duration duration) : m_duration(duration)
        {
            m_savedTimePoint = std::chrono::system_clock::now();
        }

        bool await_ready() const noexcept;

        void await_resume() noexcept;

        // returning to caller of this coroutine because return type of await_suspend is void
        void await_suspend(std::coroutine_handle<> thisCoroutine) noexcept;

    private:
        std::chrono::system_clock::duration m_duration;
        std::chrono::system_clock::time_point m_savedTimePoint;
    };
}

template<typename Rep, typename Period>
auto operator co_await(std::chrono::duration<Rep, Period> duration) noexcept
{
    return SGCore::Coro::TimerAwaitable { duration };
}

#endif // SUNGEARENGINE_CORO_TIMERAWAITABLE_H
