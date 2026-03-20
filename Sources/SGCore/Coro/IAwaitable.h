//
// Created by stuka on 18.05.2025.
//

#pragma once

#include <coroutine>
#include <sgcore_export.h>

namespace SGCore::Coro
{
    struct SGCORE_EXPORT IAwaitable
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
