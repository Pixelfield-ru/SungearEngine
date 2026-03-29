//
// Created by stuka on 22.03.2026.
//

#include "SkeletalAnimationNode.h"

void SGCore::SkeletalAnimationNode::tick(double dt, ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept
{
    if(!m_skeletalAnimation) return;

    m_currentAnimationTime += dt * m_animationSpeed * m_skeletalAnimation->m_ticksPerSecond;
}

bool SGCore::SkeletalAnimationNode::isAnimationEnded(ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept
{
    if(!m_skeletalAnimation) return true;

    if(m_animationSpeed > 0.0f)
    {
        return m_currentAnimationTime >= m_skeletalAnimation->m_duration;
    }

    return m_currentAnimationTime <= 0.0f;
}

void SGCore::SkeletalAnimationNode::resetTimelineTime() noexcept
{
    if(m_animationSpeed >= 0.0f)
    {
        m_currentAnimationTime = 0.0f;
        return;
    }

    m_currentAnimationTime = m_skeletalAnimation->m_duration;
}
