//
// Created by stuka on 18.05.2025.
//

#include "CoroScheduler.h"

#include <stack>

void SGCore::Coro::CoroScheduler::process() noexcept
{
    {
        auto lastVec = m_yieldedCoros;
        m_yieldedCoros.clear();

        for(auto& awaitable : lastVec)
        {
            if(awaitable.done()) continue;

            awaitable.resume();
        }
    }

    {
        std::vector<std::coroutine_handle<>> corosToResume { };

        for(std::ptrdiff_t i = static_cast<std::ptrdiff_t>(m_timerAwaitableCoros.size()) - 1; i >= 0; --i)
        {
            auto& awaitable = m_timerAwaitableCoros[i];

            if (awaitable.await_ready())
            {
                auto coro = awaitable.getOwnerCoro();

                m_timerAwaitableCoros.erase(m_timerAwaitableCoros.begin() + i);

                corosToResume.push_back(coro);
            }
        }

        for(auto& coro : corosToResume) coro.resume();
    }

    {
        std::vector<std::coroutine_handle<>> corosToResume { };

        for(std::ptrdiff_t i = static_cast<std::ptrdiff_t>(m_taskAwaitableCoros.size()) - 1; i >= 0; --i)
        {
            auto& awaitable = m_taskAwaitableCoros[i];

            if (awaitable.await_ready())
            {
                auto coro = awaitable.getOwnerCoro();

                m_taskAwaitableCoros.erase(m_taskAwaitableCoros.begin() + i);

                corosToResume.push_back(coro);
            }
        }

        for(auto& coro : corosToResume) coro.resume();
    }
}

void SGCore::Coro::CoroScheduler::addYieldedCoro(std::coroutine_handle<> coro) noexcept
{
    m_yieldedCoros.push_back(coro);
}

void SGCore::Coro::CoroScheduler::addTimerAwaitableCoro(TimerAwaitable task) noexcept
{
    m_timerAwaitableCoros.push_back(task);
}

void SGCore::Coro::CoroScheduler::addTaskAwaitableCoro(TaskAwaitableBase task) noexcept
{
    m_taskAwaitableCoros.push_back(task);
}
