//
// Created by stuka on 29.05.2025.
//

#include "WhenAnimationEndAction.h"
#include "MotionPlannerNode.h"

bool SGCore::WhenAnimationEndAction::execute() noexcept
{
    if(!m_animationNode || !m_animationNode->m_skeletalAnimation) return false;

    return m_animationNode->m_currentAnimationTime >= m_animationNode->m_skeletalAnimation->m_duration - 2;
}

SGCore::Ref<SGCore::IAction<bool()>> SGCore::WhenAnimationEndAction::copy() noexcept
{
    auto newAction = MakeRef<WhenAnimationEndAction>();
    newAction->m_animationNode = m_animationNode;

    return newAction;
}
