//
// Created by stuka on 21.08.2025.
//

#include "IKResolver.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "IKRootJoint.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Transformations/TransformUtils.h"

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
        // jointsChains.emplace_back();
        std::vector<ECS::entity_t> rootChain;

        // collecting all joints chains from root to end effector
        collectJoints(*registry, entityBaseInfo, jointsChains, rootChain);
        // collectJoints(*registry, entityBaseInfo, jointsChains, *jointsChains.begin());

        // processing joints chains
        for(size_t i = 0; i < jointsChains.size(); ++i)
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

            float totalChainLength = 0;

            // calculating bones lengths
            for(size_t j = 0; j < jointsCount - 1; ++j)
            {
                const float boneLength = glm::distance(jointsTransforms[j]->m_finalTransform.m_position, jointsTransforms[j + 1]->m_finalTransform.m_position);
                bonesLengths.push_back(boneLength);
                totalChainLength += boneLength;
            }

            // classic FABRIK algorithm =======================================

            const auto& effectorTransform = jointsTransforms[jointsCount - 1];
            auto& effectorJoint = registry->get<IKJoint>(chain[jointsCount - 1]);

            if(!effectorJoint.m_targetPosition) continue;

            const auto targetPos = *effectorJoint.m_targetPosition;
            const glm::vec3 targetGlobalPos = jointsTransforms[jointsCount - 2]->m_finalTransform.m_animatedModelMatrix * glm::vec4(*effectorJoint.m_targetPosition, 1.0f);

            // ====================================================

            const auto rootPos = jointsTransforms[0]->m_ownTransform.m_position;
            const auto rootGlobalPos = jointsTransforms[0]->m_finalTransform.m_position;

            const float distToTarget = glm::distance(rootGlobalPos, targetGlobalPos);
            if(distToTarget > totalChainLength)
            {
                /*glm::vec3 direction = glm::normalize(m_target - root);
                for (size_t i = 1; i < m_points.size(); i++) {
                    m_points[i].position = m_points[i-1].position + direction * m_lengths[i-1];
                }*/

                // if(glm::distance2(jointsTransforms[jointsCount - 1]->m_ownTransform.m_position, targetPos) < tolerance * tolerance)
                {
                    effectorJoint.m_targetPosition = std::nullopt;
                }

                continue;
            }

            // ====================================================

            // jointsTransforms[jointsCount - 1]->m_ownTransform.m_position = glm::inverse(jointsTransforms[jointsCount - 1]->m_finalTransform.m_animatedModelMatrix) * glm::vec4(targetPos, 1.0);
            effectorTransform->m_ownTransform.m_position = targetPos;
            // effectorTransform->m_finalTransform.m_position = targetGlobalPos;
            {
                const auto& parentTransform = registry->tryGet<Transform>(registry->get<EntityBaseInfo>(chain[jointsCount - 1]).getParent());
                TransformUtils::calculateTransform(*effectorTransform, parentTransform ? parentTransform->get() : nullptr);
            }

            // backward
            for(std::int64_t j = jointsCount - 2; j >= 0; --j)
            {
                glm::vec3 dir = glm::normalize(jointsTransforms[j]->m_finalTransform.m_position - jointsTransforms[j + 1]->m_finalTransform.m_position);
                const glm::vec3 globalPos = jointsTransforms[j + 1]->m_finalTransform.m_position + dir * bonesLengths[j];

                glm::vec3 localPos = glm::vec4(globalPos, 0.0);
                if(j - 1 >= 0)
                {
                    localPos = glm::inverse(jointsTransforms[j - 1]->m_finalTransform.m_animatedModelMatrix) * glm::vec4(globalPos, 0.0);
                }

                jointsTransforms[j]->m_ownTransform.m_position = localPos;

                {
                    const auto& parentTransform = registry->tryGet<Transform>(registry->get<EntityBaseInfo>(chain[j]).getParent());
                    TransformUtils::calculateTransform(*jointsTransforms[j], parentTransform ? parentTransform->get() : nullptr);
                }

                // jointsTransforms[j]->m_finalTransform.m_position = globalPos;
                // jointsTransforms[j]->m_ownTransform.m_position = globalPos;

                /*const float d = glm::distance(jointsTransforms[j + 1]->m_finalTransform.m_position, jointsTransforms[j]->m_finalTransform.m_position);
                const float lambda = bonesLengths[j] / d;

                const glm::vec3 globalPosition = (1 - lambda) * jointsTransforms[j + 1]->m_finalTransform.m_position + lambda * jointsTransforms[j]->m_finalTransform.m_position;
                const glm::vec3 localPosition = glm::inverse(jointsTransforms[j]->m_finalTransform.m_animatedModelMatrix) * glm::vec4(globalPosition, 1.0);

                jointsTransforms[j]->m_ownTransform.m_position = localPosition;*/
            }

            // forward
            jointsTransforms[0]->m_ownTransform.m_position = rootPos;

            {
                const auto& parentTransform = registry->tryGet<Transform>(registry->get<EntityBaseInfo>(chain[0]).getParent());
                TransformUtils::calculateTransform(*jointsTransforms[0], parentTransform ? parentTransform->get() : nullptr);
            }
            // jointsTransforms[0]->m_finalTransform.m_position = rootGlobalPos;

            for(std::int64_t j = 1; j < jointsCount; ++j)
            {
                glm::vec3 dir = glm::normalize(jointsTransforms[j]->m_finalTransform.m_position - jointsTransforms[j - 1]->m_finalTransform.m_position);
                const glm::vec3 globalPos = jointsTransforms[j - 1]->m_finalTransform.m_position + dir * bonesLengths[j - 1];

                glm::vec3 localPos = glm::vec4(globalPos, 0.0);
                if(j - 1 >= 0)
                {
                    localPos = glm::inverse(jointsTransforms[j - 1]->m_finalTransform.m_animatedModelMatrix) * glm::vec4(globalPos, 0.0);
                }

                jointsTransforms[j]->m_ownTransform.m_position = localPos;

                {
                    const auto& parentTransform = registry->tryGet<Transform>(registry->get<EntityBaseInfo>(chain[j]).getParent());
                    TransformUtils::calculateTransform(*jointsTransforms[j], parentTransform ? parentTransform->get() : nullptr);
                }

                // jointsTransforms[j]->m_finalTransform.m_position = globalPos;
            }

            if(glm::distance2(effectorTransform->m_ownTransform.m_position, targetPos) < tolerance * tolerance)
            {
                effectorJoint.m_targetPosition = std::nullopt;
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

        const auto* ikJoint = inRegistry.tryGet<IKJoint>(currentIKEntity);
        if(ikJoint && ikJoint->m_isEndJoint)
        {
            jointsChains.push_back(currentJointsChain);
            return;
        }
    }
    for(auto childEntity : currentIKEntityInfo.getChildren())
    {
        auto newChain = currentJointsChain;

        collectJoints(inRegistry, inRegistry.get<EntityBaseInfo>(childEntity), jointsChains, newChain);
    }
}

// - 1 chain
// -- 2 chain
// ---
// -- 3 chain