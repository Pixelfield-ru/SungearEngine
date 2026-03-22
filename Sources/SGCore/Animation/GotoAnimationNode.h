//
// Created by stuka on 22.03.2026.
//

#pragma once

#include <glm/vec3.hpp>

#include "IAnimationNode.h"

namespace SGCore
{
    struct SGCORE_EXPORT GotoAnimationNode : IAnimationNode
    {
        sg_implement_type_id(SGCore::GotoAnimationNode)

        glm::vec3 m_destination {};
        float m_distanceErrorRate = 1.0f;
        bool m_useBlend = false;
        bool m_interpolate = false;

        void tick(double dt, ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept final;
        bool isAnimationEnded(ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept final;

    private:
        bool m_isDestinationReached = false;
    };
}