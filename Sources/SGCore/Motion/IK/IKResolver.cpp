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
    // in chain
    std::vector<Transform::reg_t> jointsTransforms;
    // in chain
    std::vector<float> bonesLengths;

    ikRootsView.each([&registry, &jointsChains, &jointsTransforms, &bonesLengths](const EntityBaseInfo::reg_t& entityBaseInfo, const IKRootJoint::reg_t& rootJoint, const Transform::reg_t& transform) {
        jointsChains.emplace_back();

        // collecting all joints chains from root to end effector
        collectJoints(*registry, entityBaseInfo, jointsChains, *jointsChains.begin());

        // processing joints chains
        // skipping first chain because it contains only root joint
        for(size_t i = 1; i < jointsChains.size(); ++i)
        {
            const auto& chain = jointsChains[i];

            if(chain.size() < 2) continue;

            jointsTransforms.clear();
            bonesLengths.clear();

            const float tolerance = 0.001;

            const auto jointsCount = chain.size();

            // collecting transforms
            for(auto jointEntity : chain)
            {
                auto jointTransform = registry->get<Transform>(jointEntity);

                jointsTransforms.push_back(jointTransform);
            }

            // calculating bones lengths
            for(size_t j = 0; j < jointsCount - 1; ++j)
            {
                const float boneLength = glm::distance(jointsTransforms[j]->m_finalTransform.m_position, jointsTransforms[j + 1]->m_finalTransform.m_position);
                bonesLengths.push_back(boneLength);
            }

            // classic FABRIK algorithm =======================================

            if(!registry->get<IKJoint>(chain[jointsCount - 1]).m_targetPosition) continue;

            const auto targetPos = *registry->get<IKJoint>(chain[jointsCount - 1]).m_targetPosition;

            jointsTransforms[jointsCount - 1]->m_ownTransform.m_position = glm::inverse(jointsTransforms[jointsCount - 1]->m_finalTransform.m_animatedModelMatrix) * glm::vec4(targetPos, 1.0);

            // backward
            for(std::int64_t j = jointsCount - 2; j >= 0; --j)
            {
                const float d = glm::distance(jointsTransforms[j + 1]->m_finalTransform.m_position, jointsTransforms[j]->m_finalTransform.m_position);
                const float lambda = bonesLengths[j] / d;

                const glm::vec3 globalPosition = (1 - lambda) * jointsTransforms[j + 1]->m_finalTransform.m_position + lambda * jointsTransforms[j]->m_finalTransform.m_position;
                const glm::vec3 localPosition = glm::inverse(jointsTransforms[j]->m_finalTransform.m_animatedModelMatrix) * glm::vec4(globalPosition, 1.0);

                jointsTransforms[j]->m_ownTransform.m_position = localPosition;
            }

            // forward
            // joints[0] = rootPosition;
            for(std::int64_t j = 0; j < jointsCount - 1; ++j)
            {
                const float d = glm::distance(jointsTransforms[j + 1]->m_finalTransform.m_position, jointsTransforms[j]->m_finalTransform.m_position);
                float lambda = bonesLengths[j] / d;

                const glm::vec3 globalPosition = (1 - lambda) * jointsTransforms[j]->m_finalTransform.m_position + lambda * jointsTransforms[j + 1]->m_finalTransform.m_position;
                const glm::vec3 localPosition = glm::inverse(jointsTransforms[j]->m_finalTransform.m_animatedModelMatrix) * glm::vec4(globalPosition, 1.0);

                jointsTransforms[j + 1]->m_ownTransform.m_position = localPosition;
            }

            if(glm::distance2(jointsTransforms[jointsCount - 1]->m_finalTransform.m_position, targetPos) < tolerance * tolerance)
            {
                registry->get<IKJoint>(chain[jointsCount - 1]).m_targetPosition = std::nullopt;
            }
         }

        jointsChains.clear();
    });
}

void SGCore::IKResolver::collectJoints(const ECS::registry_t& inRegistry,
                                       const EntityBaseInfo::reg_t& currentIKEntityInfo,
                                       std::vector<std::vector<ECS::entity_t>>& jointsChains,
                                       std::vector<ECS::entity_t>& currentJointsChain) noexcept
{
    const auto currentIKEntity = currentIKEntityInfo.getThisEntity();

    if(inRegistry.anyOf<IKJoint, IKRootJoint>(currentIKEntity) && inRegistry.allOf<Transform>(currentIKEntity))
    {
        currentJointsChain.push_back(currentIKEntity);
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