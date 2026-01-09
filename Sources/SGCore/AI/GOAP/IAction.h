//
// Created by stuka on 10.01.2026.
//

#pragma once

#include "SGCore/Coro/Task.h"

namespace SGCore::GOAP
{
    struct IAction
    {
        virtual ~IAction() = default;

        virtual Coro::Task<> execute() noexcept = 0;
        virtual void calculateCost() noexcept = 0;

        virtual IAction* clone() const noexcept = 0;

        float getCost() const noexcept;

    protected:
        float m_cost = 0.0f;
    };
}
