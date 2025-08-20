//
// Created by stuka on 21.08.2025.
//

#include "IKResolver.h"

#include "IKRootJoint.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/Transform.h"

void SGCore::IKResolver::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto scene = getScene();

    if(!scene) return;

    auto registry = scene->getECSRegistry();

    auto ikRootsView = registry->view<EntityBaseInfo, IKRootJoint, Transform>();

    std::vector<std::vector<ECS::entity_t>> jointsChains;

    ikRootsView.each([&registry, &jointsChains](const EntityBaseInfo::reg_t& entityBaseInfo, const IKRootJoint::reg_t& rootJoint, const Transform::reg_t& transform) {
        jointsChains.emplace_back();

        // collecting all joints chains from root to end effector
        collectJoints(*registry, entityBaseInfo, jointsChains, *jointsChains.begin());

        // processing joints chains
        // skipping first chain because it contains only root joint
        for(size_t i = 1; i < jointsChains.size(); ++i)
        {
            const auto& chain = jointsChains[i];
        }

        jointsChains.clear();
    });
}

void SGCore::IKResolver::collectJoints(const ECS::registry_t& inRegistry,
                                       const EntityBaseInfo::reg_t& currentIKEntityInfo,
                                       std::vector<std::vector<ECS::entity_t>>& jointsChains,
                                       std::vector<ECS::entity_t>& currentJointsChain) noexcept
{
    if(inRegistry.anyOf<IKJoint, IKRootJoint>(currentIKEntityInfo.getThisEntity()))
    {
        currentJointsChain.push_back(currentIKEntityInfo.getThisEntity());
    }

    for(auto childEntity : currentIKEntityInfo.getChildren())
    {
        std::vector<ECS::entity_t> newChain = currentJointsChain;

        collectJoints(inRegistry, inRegistry.get<EntityBaseInfo>(childEntity), jointsChains, newChain);

        jointsChains.push_back(std::move(newChain));
    }
}

// - 1 chain
// -- 2 chain
// ---
// -- 3 chain