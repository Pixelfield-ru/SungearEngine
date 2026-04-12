//
// Created by stuka on 21.08.2025.
//

#pragma once

#include "IKJoint.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Scene/EntityBaseInfo.h"

namespace SGCore
{
    struct IKChainNode
    {
        ECS::entity_t m_jointEntity = entt::null;
        float m_boneLength {};
    };

    struct SGCORE_EXPORT IKRootJoint : ECS::Component<IKRootJoint, const IKRootJoint>
    {
        void buildChains(const ECS::registry_t& registry) noexcept;

        [[nodiscard]] const std::vector<std::vector<IKChainNode>>& getCachedChains() const noexcept;

    private:
        bool m_dummy = true;

        std::vector<std::vector<IKChainNode>> m_cachedChains;

        void buildChainsImpl(const ECS::registry_t& registry,
                             const EntityBaseInfo::reg_t& currentIKEntityInfo,
                             std::vector<std::vector<IKChainNode>>& jointsChains,
                             std::vector<IKChainNode>& currentJointsChain) const noexcept;
    };
}
