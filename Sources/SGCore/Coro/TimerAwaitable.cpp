//
// Created by stuka on 18.05.2025.
//

#include "TimerAwaitable.h"

#include "CoroScheduler.h"

#include "CoroUtils.h"

bool SGCore::Coro::TimerAwaitable::await_ready() const noexcept
{
    return std::chrono::system_clock::now() >= m_savedTimePoint + m_duration;
}

void SGCore::Coro::TimerAwaitable::await_resume() noexcept
{

}

void SGCore::Coro::TimerAwaitable::await_suspend(std::coroutine_handle<> thisCoroutine) noexcept
{
    m_ownerCoro = thisCoroutine;
    CoroUtils::assumeCurrentThread()->m_coroScheduler.addTimerAwaitableCoro(*this);
}


