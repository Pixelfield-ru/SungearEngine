//
// Created by stuka on 10.01.2026.
//

#pragma once
#include "IAction.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::GOAP
{
    struct Plan
    {
        std::vector<Ref<IAction>> m_actions;

        Coro::Task<bool> execute(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept;

        void calculateCost(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept;
        float getCost() const noexcept;

        bool isExecuting() const noexcept;

    private:
        bool m_isExecuting = false;

        float m_cost = 0.0f;
    };
}
