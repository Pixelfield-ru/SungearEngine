//
// Created by stuka on 22.03.2026.
//

#include "GotoAnimationNode.h"

#include "SGCore/ECS/Registry.h"
#include "SGCore/Transformations/Transform.h"

void SGCore::GotoAnimationNode::tick(double dt, ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept
{
    Ref<Transform> entityTransform;
    {
        auto* tmpTransform = inRegistry.tryGet<Transform>(entity);
        entityTransform = tmpTransform ? *tmpTransform : nullptr;
    }

    if(!entityTransform) return;

    m_isDestinationReached = false;

    auto& ownTransform = entityTransform->m_ownTransform;
    auto& finalTransform = entityTransform->m_finalTransform;

    const auto direction = glm::normalize(m_destination - finalTransform.m_position);

    glm::vec3 offset = direction;
    if(m_interpolate)
    {
        offset = m_destination - finalTransform.m_position;
    }

    offset *= m_animationSpeed * dt;

    if(m_useBlend)
    {
        offset *= m_currentBlendFactor;
    }

    ownTransform.m_position += offset;

    if(glm::distance(m_destination, finalTransform.m_position) <= m_distanceErrorRate)
    {
        m_isDestinationReached = true;
    }
}

bool SGCore::GotoAnimationNode::isAnimationEnded(ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept
{
    return m_isDestinationReached;
}
