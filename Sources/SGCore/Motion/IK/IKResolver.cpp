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

            const auto jointsCount = std::ssize(chain);

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
                const float boneLength = glm::distance(jointsTransforms[j]->m_worldTransform.m_position, jointsTransforms[j + 1]->m_worldTransform.m_position);
                std::println(std::cout, "boneLength: {}", boneLength);
                bonesLengths.push_back(boneLength);
                totalChainLength += boneLength;
            }

            // std::println(std::cout, "chain {} length: {}, joints count: {}", i, totalChainLength, jointsCount);

            // ======= FABRIK

            auto& endJoint = registry->get<IKJoint>(chain[jointsCount - 1]);

            // if(!endJoint.m_targetPosition) continue;

            // jointsTransforms[jointsCount - 1]->m_worldTransform.m_position = *endJoint.m_targetPosition;

            const auto rootJointOriginalPos = jointsTransforms[0]->m_worldTransform.m_position;

            // std::println(std::cout, "end pos: {}", glm::to_string(jointsTransforms[jointsCount - 1]->m_worldTransform.m_position));

            // todo: вроде forward и backward правильно написаны, но работают некорректно. исправить

            // === forward
            for(std::ptrdiff_t j = jointsCount - 2; j >= 0; --j)
            {
                auto& worldTransform = jointsTransforms[j]->m_worldTransform;
                auto& localTransform = jointsTransforms[j]->m_localTransform;
                auto& nextWorldTransform = jointsTransforms[j + 1]->m_worldTransform;
                auto& nextLocalTransform = jointsTransforms[j + 1]->m_localTransform;

                auto& jointBaseInfo = registry->get<EntityBaseInfo>(jointsTransforms[j]->getThisEntity());

                // const auto dir = glm::normalize(nextWorldTransform.m_position - worldTransform.m_position);
                const auto dir = glm::normalize(worldTransform.m_position - nextWorldTransform.m_position);
                if(glm::any(glm::isnan(dir)))
                {
                    continue;
                }

                const auto boneLength = bonesLengths[j];

                // worldTransform.m_position = worldTransform.m_position + dir * bonesLengths[j];
                worldTransform.m_position = nextWorldTransform.m_position + dir * boneLength;

                // localTransform.m_position += dir * bonesLengths[j];

                // (1, 2) -
                // (3, 3)
                // -2, -1

                Transform* parentTransform {};

                {
                    auto tmpTransform = registry->tryGet<Transform>(jointBaseInfo.getParent());
                    parentTransform = tmpTransform ? tmpTransform->get() : nullptr;
                }

                if(parentTransform)
                {
                    localTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, worldTransform.m_position);
                }
                else
                {
                    localTransform.m_position = worldTransform.m_position;
                }

                std::println(std::cout, "1111 normalized dir: {}, next final pos: {}, next local pos: {}, final pos: {}, local pos: {}, boneLength: {}, j: {}, j + 1: {}",
                             glm::to_string(dir),
                             glm::to_string(nextWorldTransform.m_position),
                             glm::to_string(nextLocalTransform.m_position),
                             glm::to_string(worldTransform.m_position),
                             glm::to_string(localTransform.m_position),
                             boneLength, j, j + 1);
            }

            // ==========

            jointsTransforms[0]->m_worldTransform.m_position = rootJointOriginalPos;

            /*auto& startJointBaseInfo = registry->get<EntityBaseInfo>(jointsTransforms[0]->getThisEntity());
            Transform* startParentTransform {};

            {
                auto tmpTransform = registry->tryGet<Transform>(startJointBaseInfo.getParent());
                startParentTransform = tmpTransform ? tmpTransform->get() : nullptr;
            }

            if(startParentTransform)
            {
                jointsTransforms[0]->m_localTransform.m_position = TransformUtils::calculateLocalPosition(*startParentTransform, jointsTransforms[0]->m_worldTransform.m_position);
            }
            else
            {
                jointsTransforms[0]->m_localTransform.m_position = jointsTransforms[0]->m_worldTransform.m_position;
            }
            */

            // === backwards
            for(std::ptrdiff_t j = 1; j <= jointsCount - 1; ++j)
            {
                auto& worldTransform = jointsTransforms[j]->m_worldTransform;
                auto& localTransform = jointsTransforms[j]->m_localTransform;
                auto& nextWorldTransform = jointsTransforms[j - 1]->m_worldTransform;
                auto& nextLocalTransform = jointsTransforms[j - 1]->m_localTransform;

                auto& jointBaseInfo = registry->get<EntityBaseInfo>(jointsTransforms[j]->getThisEntity());

                // const auto dir = glm::normalize(nextWorldTransform.m_position - worldTransform.m_position);
                const auto dir = glm::normalize(worldTransform.m_position - nextWorldTransform.m_position);
                if(glm::any(glm::isnan(dir)))
                {
                    continue;
                }

                const auto boneLength = bonesLengths[j - 1];

                // nextWorldTransform.m_position = worldTransform.m_position + dir * bonesLengths[j];
                worldTransform.m_position = nextWorldTransform.m_position + dir * boneLength;
                // worldTransform.m_position += dir * bonesLengths[j];

                Transform* parentTransform {};

                {
                    auto tmpTransform = registry->tryGet<Transform>(jointBaseInfo.getParent());
                    parentTransform = tmpTransform ? tmpTransform->get() : nullptr;
                }

                if(parentTransform)
                {
                    localTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, worldTransform.m_position);
                }
                else
                {
                    localTransform.m_position = worldTransform.m_position;
                }

                std::println(std::cout, "2222 normalized dir: {}, next final pos: {}, next local pos: {}, final pos: {}, local pos: {}, boneLength: {}, j: {}, j - 1: {}",
                             glm::to_string(dir),
                             glm::to_string(nextWorldTransform.m_position),
                             glm::to_string(nextLocalTransform.m_position),
                             glm::to_string(worldTransform.m_position),
                             glm::to_string(localTransform.m_position),
                             boneLength,
                             j, j - 1);
            }

            // === calculating rotations
            continue;

            for(size_t j = 0; j < jointsCount - 1; ++j)
            {
                auto& worldTransform = jointsTransforms[j]->m_worldTransform;
                auto& localTransform = jointsTransforms[j]->m_localTransform;
                auto& nextWorldTransform = jointsTransforms[j + 1]->m_worldTransform;

                auto& jointBaseInfo = registry->get<EntityBaseInfo>(jointsTransforms[j]->getThisEntity());

                const auto dir = glm::normalize(nextWorldTransform.m_position - worldTransform.m_position);
                if(glm::any(glm::isnan(dir)))
                {
                    continue;
                }

                Transform* parentTransform {};

                {
                    auto tmpTransform = registry->tryGet<Transform>(jointBaseInfo.getParent());
                    parentTransform = tmpTransform ? tmpTransform->get() : nullptr;
                }

                auto rotationDir = worldTransform.m_forward;

                const auto delta = glm::rotation(rotationDir, dir);

                if(glm::any(glm::isnan(delta)))
                {
                    continue;
                }

                worldTransform.m_rotation = delta * worldTransform.m_rotation;
                // localTransform.m_rotation = delta * localTransform.m_rotation;

                if(parentTransform)
                {
                    localTransform.m_rotation = TransformUtils::calculateLocalRotation(*parentTransform, worldTransform.m_rotation);
                }
                else
                {
                    localTransform.m_rotation = worldTransform.m_rotation;
                }

                localTransform.m_rotation = glm::normalize(localTransform.m_rotation);

                // std::println(std::cout, "rotation: {}, delta: {}", glm::to_string(localTransform.m_rotation), glm::to_string(delta));
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