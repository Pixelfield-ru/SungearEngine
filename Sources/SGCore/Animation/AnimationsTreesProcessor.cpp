//
// Created by stuka on 21.03.2026.
//

#include "AnimationsTreesProcessor.h"

#include "AnimationNodeConnection.h"
#include "AnimationsTree.h"
#include "IAnimationNode.h"
#include "SGCore/Scene/Scene.h"

void SGCore::AnimationsTreesProcessor::update(double dt, double fixedDt) noexcept
{
    auto scene = getScene();

    if(!scene) return;

    auto ecsRegistry = scene->getECSRegistry();

    auto treesView = ecsRegistry->view<AnimationsTree>();

    treesView.each([&](ECS::entity_t entity, AnimationsTree& tree) {
        std::vector<Ref<IAnimationNode>> nodesToActivate;

        for(const auto& rootNode : tree.m_rootNodes)
        {
            if(!rootNode->m_isActive) continue;

            interpolateAndCollectNodes(dt, rootNode, nodesToActivate);
        }

        // ============================= interpolating animations

        for(const auto& node : nodesToActivate)
        {
            if(node->getState() == PlayableState::SG_PLAYING)
            {
                node->tick(dt, entity, *ecsRegistry);
            }

            // if animation has ended
            if(node->isAnimationEnded(entity, *ecsRegistry))
            {
                if(node->m_isLooping)
                {
                    // node->m_currentAnimationTime = 1.0f;
                    // node->m_currentAnimationTime = 0.0f;
                    node->resetTimelineTime();
                }
                else
                {
                    node->setState(PlayableState::SG_STOPPED);

                    for(const auto& action : node->m_onAnimationEndActions)
                    {
                        action->execute(node.get());
                    }

                    node->onAnimationEndSignal(node.get());
                }
            }

            // node->m_isPlaying = !node->m_isPaused;
        }

        // =============================

        /*for(const auto& activeNode : nodesToActivate)
        {
            activeNode->activate();
        }*/
    });
}

void SGCore::AnimationsTreesProcessor::interpolateAndCollectNodes(double dt,
                                                                  const Ref<IAnimationNode>& currentNode,
                                                                  std::vector<Ref<IAnimationNode>>& nodesToActivate) noexcept
{
    bool isNoConnectionsActivated = true;

    for(const auto& connection : currentNode->m_connections)
    {
        if(!connection->m_nextNode) continue;

        const bool isConnectionActive = connection->m_activationAction->execute();

        auto nextNode = connection->m_nextNode;
        // must be previousNode == currentNode
        auto previousNode = connection->m_previousNode.lock();

        if(previousNode &&
           previousNode->m_isActive &&
           nextNode->m_isActive &&
           isConnectionActive)
        {
            if(connection->m_currentBlendTime < connection->m_blendTime)
            {
                connection->m_currentBlendTime += dt * connection->m_blendSpeed;
                connection->m_currentBlendTime = std::min(connection->m_currentBlendTime, connection->m_blendTime);

                const float blendFactor = connection->m_currentBlendTime / connection->m_blendTime;

                nextNode->m_currentBlendFactor = blendFactor;
                previousNode->m_currentBlendFactor = 1.0 - blendFactor;
            }

            if(previousNode->m_currentBlendFactor > 0.0f)
            {
                nodesToActivate.push_back(previousNode);
            }

            isNoConnectionsActivated = false;

            interpolateAndCollectNodes(dt, connection->m_nextNode, nodesToActivate);
        }
        // go backwards from nextNode to previousNode using interpolation
        else if(connection->m_currentBlendTime > 0)
        {
            // next node is still playing but not its children
            connection->m_nextNode->setState(PlayableState::SG_PLAYING);

            connection->m_currentBlendTime -= dt * connection->m_blendSpeed;
            connection->m_currentBlendTime = std::max(connection->m_currentBlendTime, 0.0f);

            const float blendFactor = connection->m_currentBlendTime / connection->m_blendTime;

            connection->m_nextNode->m_currentBlendFactor = blendFactor;
            previousNode->m_currentBlendFactor = 1.0f - blendFactor;

            // we must add next node to nodesToActivate because the next node still
            // needs to be taken into account when interpolating.
            //
            // but we don't need to call interpolateAndCollectNodes for nextNode because
            // we are performing reverse interpolation, i.e. it is assumed that nextNode is already inactive.
            nodesToActivate.push_back(connection->m_nextNode);
        }
    }

    // interpolating only last activated node
    if(isNoConnectionsActivated)
    {
        nodesToActivate.push_back(currentNode);
    }
}
