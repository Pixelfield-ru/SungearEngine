//
// Created by stuka on 22.03.2026.
//

#include "FrameAnimationNode.h"

void SGCore::FrameAnimationNode::setSource(AssetRef<GIF> gif) noexcept
{
    m_source = std::move(gif);

    m_usedFramesSequence = &gif->m_sequence;
}

void SGCore::FrameAnimationNode::setSource(FramesSequence sequence) noexcept
{
    m_source = std::move(sequence);

    m_usedFramesSequence = &std::get<FramesSequence>(m_source);
}

SGCore::FramesSequence* SGCore::FrameAnimationNode::getUsedFramesSequence() const noexcept
{
    return m_usedFramesSequence;
}

void SGCore::FrameAnimationNode::tick(double dt, ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept
{
    m_currentAnimationTime += dt * m_animationSpeed;
}

bool SGCore::FrameAnimationNode::isAnimationEnded(ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept
{
    if(!m_usedFramesSequence || m_usedFramesSequence->m_frames.empty()) return true;

    const auto& lastFrame = *m_usedFramesSequence->m_frames.rbegin();

    if(m_animationSpeed > 0.0f)
    {
        return m_currentAnimationTime >= (lastFrame.m_timeStamp + lastFrame.m_nextFrameDelay);
    }

    return m_currentAnimationTime <= 0.0f;
}

void SGCore::FrameAnimationNode::resetTimelineTime() noexcept
{
    if(m_animationSpeed >= 0.0f)
    {
        m_currentAnimationTime = 0.0f;
        return;
    }

    if(!m_usedFramesSequence || m_usedFramesSequence->m_frames.empty()) return;

    const auto& lastFrame = *m_usedFramesSequence->m_frames.rbegin();

    m_currentAnimationTime = lastFrame.m_timeStamp + lastFrame.m_nextFrameDelay;
}
