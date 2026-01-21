//
// Created by stuka on 11.01.2026.
//

#pragma once

#include <glm/vec3.hpp>

#include "SGCore/AI/GOAP/IAction.h"

namespace SGCore::GOAP
{
    struct Goto : IAction
    {
        Goto() noexcept;

        float m_speed = 10.0f;
        float m_distanceErrorRate = 1.0f;

        void calculateCost(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept override;

        IAction* clone() const noexcept override;

    protected:
        Coro::Task<ExecutionResult> executeImpl(ECS::registry_t& registry, ECS::entity_t forEntity, const Plan& plan) noexcept override;
    };
}
