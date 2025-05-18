//
// Created by stuka on 18.05.2025.
//

#ifndef SUNGEARENGINE_CORO_IAWAITABLE_H
#define SUNGEARENGINE_CORO_IAWAITABLE_H

#include <coroutine>

namespace SGCore::Coro
{
    struct IAwaitable
    {
        auto getOwnerCoro() const noexcept
        {
            return m_ownerCoro;
        }

        auto getCalledCoro() const noexcept
        {
            return m_calledCoro;
        }

    protected:
        std::coroutine_handle<> m_ownerCoro;
        std::coroutine_handle<> m_calledCoro;
    };
}

#endif // SUNGEARENGINE_CORO_IAWAITABLE_H
