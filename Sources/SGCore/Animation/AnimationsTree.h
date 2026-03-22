//
// Created by stuka on 21.03.2026.
//

#pragma once

#include <vector>

#include "AnimationNodeConnection.h"
#include "IAnimationNode.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Main/CoreMain.h"

namespace SGCore
{
    struct IAnimationNode;

    struct SGCORE_EXPORT AnimationsTree : ECS::Component<AnimationsTree, const AnimationsTree>
    {
        std::vector<Ref<IAnimationNode>> m_rootNodes;

        template<typename NodeT>
        requires(std::is_base_of_v<IAnimationNode, NodeT>)
        [[nodiscard]] std::vector<Ref<NodeT>> collectActiveNodesOfType() const noexcept
        {
            std::vector<Ref<NodeT>> activeNodes;

            for(const auto& rootNode : m_rootNodes)
            {
                collectActiveNodesOfTypeImpl<NodeT>(rootNode, activeNodes);
            }

            return activeNodes;
        }

    private:
        template<typename NodeT>
        requires(std::is_base_of_v<IAnimationNode, NodeT>)
        void collectActiveNodesOfTypeImpl(const Ref<IAnimationNode>& currentNode, std::vector<Ref<NodeT>>& nodes) const noexcept
        {
            const bool isNodeIsPlaying = currentNode->getState() == PlayableState::SG_PLAYING;

            if(!isNodeIsPlaying) return;

            if(NodeT::getTypeIDStatic() == currentNode->getTypeID())
            {
                nodes.push_back(std::static_pointer_cast<NodeT>(currentNode));
            }

            for(const auto& connection : currentNode->m_connections)
            {
                collectActiveNodesOfTypeImpl<NodeT>(connection->m_nextNode, nodes);
            }
        }
    };
}
