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
        glm::vec3 m_targetPosition {};
        float m_speed = 1.0f;
        float m_distanceErrorRate = 1.0f;

        void calculateCost(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept override;

        IAction* clone() const noexcept override;

    protected:
        Coro::Task<> executeImpl(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept override;
    };
}
