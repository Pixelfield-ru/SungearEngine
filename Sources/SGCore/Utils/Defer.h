//
// Created by stuka on 15.02.2026.
//

#pragma once

#include <functional>

#include "Macroses.h"

#define sg_defer SGCore::Defer SG_CONCAT(__sgcore_defer_, __LINE__) =

namespace SGCore
{
    struct Defer
    {
        ~Defer() noexcept
        {
            m_deferBody();
        }

        Defer(std::function<void()> deferBody) noexcept
        {
            m_deferBody = std::move(deferBody);
        }

        template<typename Func>
        requires(std::is_invocable_r_v<void, Func>)
        Defer(Func&& deferBody) noexcept
        {
            m_deferBody = std::forward<Func>(deferBody);
        }

        Defer(const Defer&) = delete;
        Defer(Defer&&) = delete;

        Defer& operator=(std::function<void()> deferBody) noexcept
        {
            m_deferBody = std::move(deferBody);
            return *this;
        }

        template<typename Func>
        requires(std::is_invocable_r_v<void, Func>)
        Defer& operator=(Func&& deferBody) noexcept
        {
            m_deferBody = std::forward<Func>(deferBody);
            return *this;
        }

        Defer& operator=(const Defer&) noexcept = delete;
        Defer& operator=(Defer&&) noexcept = delete;

    private:
        std::function<void()> m_deferBody;
    };
}