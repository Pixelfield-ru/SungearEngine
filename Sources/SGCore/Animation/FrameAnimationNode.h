//
// Created by stuka on 22.03.2026.
//

#pragma once

#include <variant>

#include "IAnimationNode.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Memory/Assets/GIF.h"

namespace SGCore
{
    struct FrameAnimationNode : IAnimationNode
    {
        sg_implement_type_id(SGCore::FrameAnimationNode)

        /// Texture slot to animate.
        SGTextureSlot m_textureSlot = SGTextureSlot::SGTT_DIFFUSE;

        /**
         * Animation source. Can contain GIF or custom sequence of frames
         */
        std::variant<AssetRef<GIF>, FramesSequence> m_source;

        void tick(double dt, ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept final;
        bool isAnimationEnded(ECS::entity_t entity, ECS::registry_t& inRegistry) noexcept final;
    };
}
