//
// Created by stuka on 18.05.2025.
//

#include "TaskAwaitable.h"

#include "CoroScheduler.h"
#include "Task.h"

#include "CoroUtils.h"

bool SGCore::Coro::TaskAwaitableBase::await_ready() const noexcept
{
    return m_calledCoro.done();
}

void SGCore::Coro::TaskAwaitableBase::await_suspend(std::coroutine_handle<> thisCoroutine)
{
    m_ownerCoro = thisCoroutine;
    CoroUtils::assumeCurrentThread()->m_coroScheduler.addTaskAwaitableCoro(*this);
}

SGCore::Coro::Task<bool> SGCore::Coro::returnToCaller() noexcept
{
    co_yield true;

    co_return true;
}
