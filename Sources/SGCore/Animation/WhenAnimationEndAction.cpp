//
// Created by stuka on 29.05.2025.
//

#include "WhenAnimationEndAction.h"

#include "IAnimationNode.h"
#include "SGCore/ECS/Registry.h"

bool SGCore::WhenAnimationEndAction::execute() noexcept
{
    if(!m_inRegistry) return false;

    if(!m_inRegistry->valid(m_entity)) return false;

    return m_animationNode->isAnimationEnded(m_entity, *m_inRegistry);
}

SGCore::Ref<SGCore::IAction<bool()>> SGCore::WhenAnimationEndAction::copy() noexcept
{
    auto newAction = MakeRef<WhenAnimationEndAction>();
    newAction->m_animationNode = m_animationNode;

    return newAction;
}
