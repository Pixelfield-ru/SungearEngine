//
// Created by stuka on 16.02.2026.
//

#pragma once

#include <variant>

#include "SGCore/ECS/Component.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Memory/Assets/FramesSequence.h"

namespace SGCore
{
    struct GIF;

    // todo: make different functions to interpolate between frames
    struct FrameAnimation : ECS::Component<FrameAnimation, const FrameAnimation>
    {
        /// Texture slot to animate.
        SGTextureSlot m_textureSlot = SGTextureSlot::SGTT_DIFFUSE;

        /// Must be sorted by time points.
        std::variant<AssetRef<GIF>, FramesSequence> m_source;

        float m_currentTime = 0.0f;
        float m_speed = 1.0f;

        bool m_isReversed = false;
        bool m_isRepeated = false;
    };
}
