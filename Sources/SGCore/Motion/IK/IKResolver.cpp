//
// Created by stuka on 21.08.2025.
//

#include "IKResolver.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "IKRootJoint.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Transformations/TransformUtils.h"

void SGCore::IKResolver::fixedUpdate(double dt, double fixedDt)
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

        // std::println(std::cout, "chains count: {}", jointsChains.size());

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

            // std::println(std::cout, "chain {} length: {}, joints count: {}", i, totalChainLength, jointsCount);

            // ======= FABRIK

            auto& endJoint = registry->get<IKJoint>(chain[jointsCount - 1]);

            if(!endJoint.m_targetPosition) continue;

            jointsTransforms[jointsCount - 1]->m_finalTransform.m_position = *endJoint.m_targetPosition;

            const auto rootJointOriginalPos = jointsTransforms[0]->m_finalTransform.m_position;

            // === forward
            for(std::int32_t j = jointsCount - 2; j >= 0; --j)
            {
                auto& finalTransform = jointsTransforms[j]->m_finalTransform;
                auto& ownTransform = jointsTransforms[j]->m_ownTransform;
                auto& nextFinalTransform = jointsTransforms[j + 1]->m_finalTransform;

                auto& jointBaseInfo = registry->get<EntityBaseInfo>(jointsTransforms[j]->getThisEntity());

                const auto dir = finalTransform.m_position - nextFinalTransform.m_position;
                finalTransform.m_position = nextFinalTransform.m_position + glm::normalize(dir) * bonesLengths[j];

                Transform* parentTransform {};

                {
                    auto tmpTransform = registry->tryGet<Transform>(jointBaseInfo.getParent());
                    parentTransform = tmpTransform ? tmpTransform->get() : nullptr;
                }

                if(parentTransform)
                {
                    ownTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, finalTransform.m_position);
                }
                else
                {
                    ownTransform.m_position = finalTransform.m_position;
                }
            }

            // ==========

            jointsTransforms[0]->m_finalTransform.m_position = rootJointOriginalPos;

            // === backwards
            for(std::int32_t j = 0; j < jointsCount - 1; ++j)
            {
                auto& finalTransform = jointsTransforms[j]->m_finalTransform;
                auto& nextFinalTransform = jointsTransforms[j + 1]->m_finalTransform;
                auto& nextOwnTransform = jointsTransforms[j + 1]->m_ownTransform;

                auto& jointBaseInfo = registry->get<EntityBaseInfo>(jointsTransforms[j + 1]->getThisEntity());

                const auto dir = nextFinalTransform.m_position - finalTransform.m_position;
                nextFinalTransform.m_position = finalTransform.m_position + glm::normalize(dir) * bonesLengths[j];

                Transform* parentTransform {};

                {
                    auto tmpTransform = registry->tryGet<Transform>(jointBaseInfo.getParent());
                    parentTransform = tmpTransform ? tmpTransform->get() : nullptr;
                }

                if(parentTransform)
                {
                    nextOwnTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, nextFinalTransform.m_position);
                }
                else
                {
                    nextOwnTransform.m_position = nextFinalTransform.m_position;
                }
            }

            // === calculating rotations

            for(size_t j = 0; j < jointsCount - 1; ++j)
            {
                auto& finalTransform = jointsTransforms[j]->m_finalTransform;
                auto& ownTransform = jointsTransforms[j]->m_ownTransform;
                auto& nextFinalTransform = jointsTransforms[j + 1]->m_finalTransform;

                auto& jointBaseInfo = registry->get<EntityBaseInfo>(jointsTransforms[j]->getThisEntity());

                auto dir = nextFinalTransform.m_position - finalTransform.m_position;
                dir = glm::normalize(dir);

                Transform* parentTransform {};

                {
                    auto tmpTransform = registry->tryGet<Transform>(jointBaseInfo.getParent());
                    parentTransform = tmpTransform ? tmpTransform->get() : nullptr;
                }

                // i think
                // glm::vec3 rotationDir(1, 0, 0);
                glm::vec3 rotationDir = finalTransform.m_forward;

                finalTransform.m_rotation = glm::rotation(rotationDir, dir) * finalTransform.m_rotation;
                // ownTransform.m_rotation = glm::rotation(rotationDir, forward);

                if(parentTransform)
                {
                    ownTransform.m_rotation = TransformUtils::calculateLocalRotation(*parentTransform, finalTransform.m_rotation);
                }
                else
                {
                    ownTransform.m_rotation = finalTransform.m_rotation;
                }

                ownTransform.m_rotation = glm::normalize(ownTransform.m_rotation);

                std::println(std::cout, "rotation: {}", glm::to_string(ownTransform.m_rotation));
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