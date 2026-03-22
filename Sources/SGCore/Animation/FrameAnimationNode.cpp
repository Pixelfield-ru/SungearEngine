//
// Created by stuka on 22.03.2026.
//

#include "FrameAnimationNode.h"

void SGCore::FrameAnimationNode::tick(double dt, ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept
{
    m_currentAnimationTime += dt * m_animationSpeed;
}

bool SGCore::FrameAnimationNode::isAnimationEnded(ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept
{
    FramesSequence* sequence {};
    if(auto* gif = std::get_if<AssetRef<GIF>>(&m_source))
    {
        if(!*gif) return true;

        sequence = &(*gif)->m_sequence;
    }
    else if(auto* seq = std::get_if<FramesSequence>(&m_source))
    {
        sequence = seq;
    }

    if(!sequence || sequence->m_frames.empty()) return true;

    const auto& lastFrame = *sequence->m_frames.rbegin();

    return m_currentAnimationTime >= (lastFrame.m_timeStamp + lastFrame.m_nextFrameDelay);
}
