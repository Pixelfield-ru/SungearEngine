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

    auto& localTransform = entityTransform->m_localTransform;
    auto& worldTransform = entityTransform->m_worldTransform;

    const auto direction = glm::normalize(m_destination - worldTransform.m_position);

    glm::vec3 offset = direction;
    if(m_interpolate)
    {
        offset = m_destination - worldTransform.m_position;
    }

    offset *= m_animationSpeed * dt;

    if(m_useBlend)
    {
        offset *= m_currentBlendFactor;
    }

    localTransform.m_position += offset;

    if(glm::distance(m_destination, worldTransform.m_position) <= m_distanceErrorRate)
    {
        m_isDestinationReached = true;
    }
}

bool SGCore::GotoAnimationNode::isAnimationEnded(ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept
{
    return m_isDestinationReached;
}
