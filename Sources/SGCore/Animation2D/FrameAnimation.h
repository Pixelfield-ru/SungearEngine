//
// Created by stuka on 16.02.2026.
//

#pragma once

#include <variant>

#include "SGCore/ECS/Component.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Memory/Assets/FramesSequence.h"
#include "SGCore/Utils/Playable.h"

namespace SGCore
{
    struct GIF;

    // todo: make different functions to interpolate between frames
    /**
     * ECS component to animate mesh texture using frame-by-frame animation.\n
     * Takes a mesh from the same entity on which this component is.\n
     * Must be placed in same entity mesh texture of which must be animated.\n
     * Uses material from SGCore::Mesh component to replace textures.
     */
    struct FrameAnimation : ECS::Component<FrameAnimation, const FrameAnimation>, Playable
    {
        /// Texture slot to animate.
        SGTextureSlot m_textureSlot = SGTextureSlot::SGTT_DIFFUSE;

        /**
         * Animation source. Can contain GIF or custom sequence of frames
         */
        std::variant<AssetRef<GIF>, FramesSequence> m_source;

        /**
         * Current time of animation.
         */
        float m_currentTime = 0.0f;

        /**
         * Speed of animation.
         */
        float m_speed = 1.0f;

        /**
         * Is animation plays in reverse order.
         */
        bool m_isReversed = false;

        /**
         * Is animation repeats.
         */
        bool m_isRepeated = false;

    private:
        void resetTimelineTime() noexcept override
        {
            m_currentTime = 0.0f;
        }
    };
}
