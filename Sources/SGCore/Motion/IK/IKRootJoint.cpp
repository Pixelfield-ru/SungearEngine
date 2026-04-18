//
// Created by stuka on 12.04.2026.
//

#include "IKRootJoint.h"

#include <iostream>

#include "SGCore/ECS/Registry.h"
#include "SGCore/Transformations/Transform.h"

void SGCore::IKRootJoint::buildChains(const ECS::registry_t& registry) noexcept
{
    std::vector<IKChainNode> rootChain;
    std::vector<std::vector<IKChainNode>> chains;

    buildChainsImpl(registry, registry.get<EntityBaseInfo>(getThisEntity()), chains, rootChain);

    // calculating bones length
    for(auto& chain : chains)
    {
        const auto nodesCounts = std::ssize(chain) - 1;

        for(size_t j = 0; j < nodesCounts; ++j)
        {
            auto& node = chain[j];
            auto& nextNode = chain[j + 1];

            auto& transform = registry.get<Transform>(node.m_jointEntity);
            auto& nextTransform = registry.get<Transform>(nextNode.m_jointEntity);

            node.m_boneLength = glm::distance(transform->m_worldTransform.m_position, nextTransform->m_worldTransform.m_position);

            std::println(std::cout, "bone {} length: {}", j, node.m_boneLength);
        }
    }

    m_cachedChains = std::move(chains);
}

const std::vector<std::vector<SGCore::IKChainNode>>& SGCore::IKRootJoint::getCachedChains() const noexcept
{
    return m_cachedChains;
}

void SGCore::IKRootJoint::buildChainsImpl(const ECS::registry_t& registry,
                                          const EntityBaseInfo::reg_t& currentIKEntityInfo,
                                          std::vector<std::vector<IKChainNode>>& jointsChains,
                                          std::vector<IKChainNode>& currentJointsChain) const noexcept
{
    const auto currentIKEntity = currentIKEntityInfo.getThisEntity();

    if(registry.anyOf<IKJoint, IKRootJoint>(currentIKEntity) && registry.allOf<Transform>(currentIKEntity))
    {
        currentJointsChain.push_back({ currentIKEntity, 0 });

        const auto* ikJoint = registry.tryGet<IKJoint>(currentIKEntity);
        if(ikJoint && ikJoint->m_isEndJoint)
        {
            jointsChains.push_back(std::move(currentJointsChain));
            return;
        }
    }
    for(auto childEntity : currentIKEntityInfo.getChildren())
    {
        auto newChain = currentJointsChain;

        buildChainsImpl(registry, registry.get<EntityBaseInfo>(childEntity), jointsChains, newChain);
    }
}
