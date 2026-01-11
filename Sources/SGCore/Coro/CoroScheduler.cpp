//
// Created by stuka on 18.05.2025.
//

#include "CoroScheduler.h"

void SGCore::Coro::CoroScheduler::process() noexcept
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
