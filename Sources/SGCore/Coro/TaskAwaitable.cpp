//
// Created by stuka on 18.05.2025.
//

#include "TaskAwaitable.h"

#include "CoroScheduler.h"

bool SGCore::Coro::TaskAwaitable::await_ready() const noexcept
{
    return m_calledCoro.done();
}

void SGCore::Coro::TaskAwaitable::await_resume() noexcept
{

}

void SGCore::Coro::TaskAwaitable::await_suspend(std::coroutine_handle<> thisCoroutine)
{
    m_ownerCoro = thisCoroutine;
    CoroScheduler::addTaskAwaitableCoro(*this);
}
