//
// Created by stuka on 22.03.2026.
//

#pragma once

#include "IAnimationNode.h"
#include "SGCore/Memory/Assets/SkeletalAnimationAsset.h"

namespace SGCore
{
    struct SGCORE_EXPORT SkeletalAnimationNode : IAnimationNode
    {
        sg_implement_type_id(SGCore::SkeletalAnimationNode)

        AssetRef<SkeletalAnimationAsset> m_skeletalAnimation;

        void tick(double dt, ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept final;
        bool isAnimationEnded(ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept final;
    };
}
