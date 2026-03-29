//
// Created by stuka on 21.03.2026.
//

#pragma once

#include <vector>
#include <sgcore_export.h>

#include "SGCore/Actions/IAction.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Playable.h"
#include "SGCore/Utils/StaticTypeID.h"

namespace SGCore
{
    struct AnimationNodeConnection;

    struct SGCORE_EXPORT IAnimationNode : Playable
    {
        sg_implement_type_id_base(SGCore::IAnimationNode)

        bool m_isActive = true;
        bool m_isLooping = false;

        float m_currentBlendFactor = 1.0f;

        float m_animationSpeed = 1.0f;

        float m_currentAnimationTime = 0.0f;

        /// Called when animation ends.
        std::vector<Ref<IAction<void(IAnimationNode* thisNode)>>> m_onAnimationEndActions;
        Signal<void(IAnimationNode* thisNode)> onAnimationEndSignal;

        std::vector<Ref<AnimationNodeConnection>> m_connections;

        virtual void tick(double dt, ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept = 0;
        virtual bool isAnimationEnded(ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept = 0;

        void resetTimelineTime() noexcept override;
    };
}
