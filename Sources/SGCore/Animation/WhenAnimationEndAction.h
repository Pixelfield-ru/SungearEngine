//
// Created by stuka on 29.05.2025.
//

#pragma once

#include <sgcore_export.h>
#include <entt/entity/entity.hpp>

#include "SGCore/Actions/IAction.h"

namespace SGCore
{
    struct IAnimationNode;

    struct SGCORE_EXPORT WhenAnimationEndAction : IAction<bool()>
    {
        Ref<IAnimationNode> m_animationNode;

        ECS::entity_t m_entity = entt::null;
        Ref<ECS::registry_t> m_inRegistry;

        bool execute() noexcept final;

        Ref<IAction> copy() noexcept final;
    };
}
