//
// Created by stuka on 16.02.2026.
//

#include "FrameAnimationsProcessor.h"

#include "FrameAnimation.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Memory/Assets/GIF.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Utils/Defer.h"

void SGCore::FrameAnimationsProcessor::update(const double& dt, const double& fixedDt) noexcept
{
    auto scene = getScene();

    if(!scene) return;

    auto ecsRegistry = scene->getECSRegistry();

    auto framedEntities = ecsRegistry->view<FrameAnimation, Mesh>();

    framedEntities.each([&](auto entity, FrameAnimation& animation, Mesh& mesh) {
        sg_defer [&animation]() {
            animation.checkIsStateChanged();
        };

        if(!mesh.m_base.getMaterial() || animation.getState() != PlayableState::SG_PLAYING) return;

        FramesSequence* sequence {};
        if(auto* gif = std::get_if<AssetRef<GIF>>(&animation.m_source))
        {
            if(!*gif) return;

            sequence = &(*gif)->m_sequence;
        }
        else if(auto* seq = std::get_if<FramesSequence>(&animation.m_source))
        {
            sequence = seq;
        }

        if(!sequence || sequence->m_frames.empty()) return;

        auto& lastFrame = *sequence->m_frames.rbegin();

        if(animation.m_currentTime > (lastFrame.m_timeStamp + lastFrame.m_nextFrameDelay) || animation.m_currentTime < 0.0f)
        {
            animation.m_currentTime = !animation.m_isReversed ? 0.0f : (lastFrame.m_timeStamp + lastFrame.m_nextFrameDelay);
            if(!animation.m_isRepeated)
            {
                animation.setState(PlayableState::SG_STOPPED);
            }
            return;
        }

        const auto frameIndex = sequence->findFrameByTime(animation.m_currentTime);

        auto& frame = sequence->m_frames[frameIndex];

        mesh.m_base.getMaterial()->replaceTexture(animation.m_textureSlot, 0, frame.m_texture);

        animation.m_currentTime += dt * animation.m_speed * (animation.m_isReversed ? -1.0f : 1.0f);
    });
}
