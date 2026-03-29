//
// Created by stuka on 16.02.2026.
//

#include "FrameAnimationsProcessor.h"

#include "AnimationsTree.h"
#include "FrameAnimationNode.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Memory/Assets/GIF.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Utils/Defer.h"

void SGCore::FrameAnimationsProcessor::update(double dt, double fixedDt) noexcept
{
    auto scene = getScene();

    if(!scene) return;

    auto ecsRegistry = scene->getECSRegistry();

    auto framedEntities = ecsRegistry->view<AnimationsTree, Mesh>();

    framedEntities.each([&](AnimationsTree& animationsTree, Mesh& mesh) {
        const auto animationNodes = animationsTree.collectActiveNodesOfType<FrameAnimationNode>();

        for(const auto& animationNode : animationNodes)
        {
            sg_defer [&animationNode]() {
                animationNode->checkIsStateChanged();
            };

            if(!mesh.m_base.getMaterial() || animationNode->getState() != PlayableState::SG_PLAYING) return;

            const auto* sequence = animationNode->getUsedFramesSequence();

            if(!sequence || sequence->m_frames.empty()) return;

            const auto frameIndex = sequence->findFrameByTime(animationNode->m_currentAnimationTime);

            auto& frame = sequence->m_frames[frameIndex];

            mesh.m_base.getMaterial()->replaceTexture(animationNode->m_textureSlot, 0, frame.m_texture);
        }
    });
}
