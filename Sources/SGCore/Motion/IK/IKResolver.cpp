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

// todo: use parallel algorithms. optimize
// todo: scale breaks IK
void SGCore::IKResolver::update(double dt, double fixedDt)
{
    auto scene = getScene();

    if(!scene) return;

    auto registry = scene->getECSRegistry();

    auto ikRootsView = registry->view<EntityBaseInfo, Transform, IKRootJoint, IKJoint>();

    std::vector<std::vector<ECS::entity_t>> jointsChains;
    // in chain
    std::vector<Transform*> jointsTransforms;
    std::vector<IKJoint*> jointsComponents;
    // in chain
    std::vector<float> bonesLengths;

    ikRootsView.each([&](const EntityBaseInfo::reg_t& entityBaseInfo, const Transform::reg_t&, const IKRootJoint::reg_t&, const IKJoint::reg_t&) {
        // jointsChains.emplace_back();
        std::vector<ECS::entity_t> rootChain;

        // collecting all joints chains from root to end effector
        collectJoints(*registry, entityBaseInfo, jointsChains, rootChain);

        // processing joints chains
        for(size_t i = 0; i < jointsChains.size(); ++i)
        {
            const auto& chain = jointsChains[i];

            if(chain.size() < 2) continue;

            jointsTransforms.clear();
            bonesLengths.clear();

            const auto jointsCount = std::ssize(chain);

            // collecting transforms
            for(auto jointEntity : chain)
            {
                auto jointTransform = registry->get<Transform>(jointEntity);
                auto& jointComponent = registry->get<IKJoint>(jointEntity);

                jointComponent.m_constraintMinRotation = glm::max(jointComponent.m_constraintMinRotation, glm::vec3 { -180.0f });
                jointComponent.m_constraintMaxRotation = glm::min(jointComponent.m_constraintMaxRotation, glm::vec3 { 180.0f });

                jointsTransforms.push_back(jointTransform.get());
                jointsComponents.push_back(&jointComponent);
            }

            // GUARANTEED THAT jointsTransforms.size() == jointsComponents.size()

            // calculating bones lengths
            for(size_t j = 0; j < jointsCount - 1; ++j)
            {
                const float boneLength = glm::distance(jointsTransforms[j]->m_worldTransform.m_position, jointsTransforms[j + 1]->m_worldTransform.m_position);
                // const float boneLength = glm::length(jointsTransforms[j]->m_localTransform.m_position);
                bonesLengths.push_back(boneLength);
            }

            // =======================================================
            // ======================================================= FABRIK
            // =======================================================

            auto& endJoint = registry->get<IKJoint>(chain[jointsCount - 1]);

            if(!endJoint.m_targetPosition) continue;

            // set end joint to target position
            jointsTransforms[jointsCount - 1]->m_worldTransform.m_position = *endJoint.m_targetPosition;

            auto& rootJoint = jointsComponents[0];

            const auto rootJointOriginalPos = jointsTransforms[0]->m_worldTransform.m_position;

            // ============= FABRIK forward
            for(std::ptrdiff_t j = jointsCount - 2; j >= 0; --j)
            {
                IKJoint* parentJoint {};
                if(j - 1 >= 0)
                {
                    parentJoint = jointsComponents[j - 1];
                }
                jointPass(*registry, *jointsComponents[j], *jointsComponents[j + 1], parentJoint, *jointsTransforms[j],
                          *jointsTransforms[j + 1],
                          bonesLengths[j], false);
            }

            // ==========

            // ============== calculating rotations

            for(size_t j = 0; j < jointsCount - 1; ++j)
            {
                auto& worldTransform = jointsTransforms[j]->m_worldTransform;
                auto& localTransform = jointsTransforms[j]->m_localTransform;
                auto& nextWorldTransform = jointsTransforms[j + 1]->m_worldTransform;
                auto& joint = *jointsComponents[j];

                auto& jointBaseInfo = registry->get<EntityBaseInfo>(jointsTransforms[j]->getThisEntity());

                Transform* parentTransform {};

                {
                    auto tmpTransform = registry->tryGet<Transform>(jointBaseInfo.getParent());
                    parentTransform = tmpTransform ? tmpTransform->get() : nullptr;
                }

                // worldTransform.m_rotation = glm::identity<glm::quat>();
                // localTransform.m_rotation = glm::identity<glm::quat>();

                if(parentTransform)
                {
                    // ignoring parent rotation
                    // localTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, worldTransform.m_position);
                    // worldTransform.m_position = TransformUtils::calculateWorldPosition(*parentTransform, localTransform.m_position);
                }

                const auto dir = glm::normalize(worldTransform.m_position - nextWorldTransform.m_position);
                // const auto dir = glm::normalize(nextWorldTransform.m_position - worldTransform.m_position);
                if(glm::any(glm::isnan(dir)))
                {
                    continue;
                }

                const auto rotationDir = worldTransform.m_rotation * joint.m_rotationDirectionReference;

                const auto delta = glm::rotation(rotationDir, dir);

                if(glm::any(glm::isnan(delta)))
                {
                    continue;
                }

                worldTransform.m_rotation = delta * worldTransform.m_rotation;

                if(parentTransform)
                {
                    localTransform.m_rotation = TransformUtils::calculateLocalRotation(*parentTransform, worldTransform.m_rotation);
                }
                else
                {
                    localTransform.m_rotation = worldTransform.m_rotation;
                }

                localTransform.m_rotation = glm::normalize(localTransform.m_rotation);

                // applying rotation constration
                if(joint.m_useRotationConstraints)
                {
                    const auto constraintDelta = calculateConstraintDelta(
                        joint, worldTransform, localTransform, nextWorldTransform,
                        parentTransform ? &parentTransform->m_worldTransform : nullptr);

                    joint.m_constraintDeltaRotation = constraintDelta;
                    localTransform.m_rotation = constraintDelta * localTransform.m_rotation;
                    // worldTransform.m_rotation = constraintDelta * worldTransform.m_rotation;
                }

                /*if(joint.m_useRotationConstraints)
                {
                    const auto euler = glm::degrees(glm::eulerAngles(localTransform.m_rotation));
                    const auto clamped = glm::clamp(euler, joint.m_constraintMinRotation, joint.m_constraintMaxRotation);
                    localTransform.m_rotation = glm::quat(glm::radians(clamped));
                }*/

                // it may help prevent chain breakage
                if(parentTransform)
                {
                    worldTransform.m_rotation = TransformUtils::calculateWorldRotation(*parentTransform, localTransform.m_rotation);
                    worldTransform.m_position = TransformUtils::calculateWorldPosition(*parentTransform, localTransform.m_position);
                    localTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, worldTransform.m_position);
                }
                else
                {
                    worldTransform.m_rotation = localTransform.m_rotation;
                    worldTransform.m_position = localTransform.m_position;
                }
            }

            // =============== set original position for root
            auto* rootJointParent = getJointParent(*registry, rootJoint->getThisEntity());

            jointsTransforms[0]->m_worldTransform.m_position = rootJointOriginalPos;

            jointsTransforms[0]->m_localTransform.m_position = calculateJointLocalPosition(
                *registry,
                jointsTransforms[0]->getThisEntity(),
                jointsTransforms[0]->m_worldTransform.m_position, rootJointParent);

                /*if(rootJoint->m_useRotationConstraints)
            {
                const auto* parentJointParentTransform = getJointParent(*registry, rootJoint->getThisEntity());
                const auto constraintDelta =
                    calculateConstraintDelta(*rootJoint,
                        jointsTransforms[0]->m_worldTransform,
                        worldTransform,
                        parentJointParentTransform ? &parentJointParentTransform->m_worldTransform : nullptr);

                dir = constraintDelta * dir;
            }*/

            // =============== FABRIK backwards
            for(std::ptrdiff_t j = 1; j <= jointsCount - 1; ++j)
            {
                jointPass(*registry, *jointsComponents[j], *jointsComponents[j - 1], jointsComponents[j - 1], *jointsTransforms[j],
                          *jointsTransforms[j - 1],
                          bonesLengths[j - 1], true);
            }

            // continue;
        }

        std::println(std::cout, "0 rotation: {}, 1 rotation: {}",
            glm::to_string(glm::degrees(glm::eulerAngles(jointsTransforms[0]->m_localTransform.m_rotation))),
            glm::to_string(glm::degrees(glm::eulerAngles(jointsTransforms[1]->m_localTransform.m_rotation)))
            );

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

void SGCore::IKResolver::jointPass(ECS::registry_t& registry, const IKJoint& joint, const IKJoint& nextJoint, IKJoint* parentJoint, Transform& currentJointTransform,
                                   const Transform& nextJointTransform, float boneLength, bool isBackwardPass) noexcept
{
    auto& worldTransform = currentJointTransform.m_worldTransform;
    auto& localTransform = currentJointTransform.m_localTransform;
    auto& nextWorldTransform = nextJointTransform.m_worldTransform;
    auto& nextLocalTransform = nextJointTransform.m_localTransform;

    glm::vec3 dir {};
    if(!isBackwardPass)
    {
        dir = glm::normalize(worldTransform.m_position - nextWorldTransform.m_position);
    }
    else
    {
        // dir = glm::normalize(worldTransform.m_position - (nextWorldTransform.m_position + boneLength * (nextWorldTransform.m_rotation * nextJoint.m_rotationDirectionReference)));
        // dir = glm::normalize(worldTransform.m_position - (nextWorldTransform.m_position + boneLength * nextWorldTransform.m_up));
        // dir = glm::normalize(nextWorldTransform.m_up);
        dir = -glm::normalize(nextWorldTransform.m_rotation * nextJoint.m_rotationDirectionReference);
        // boneLength = 1.0f;
    }
    if(glm::any(glm::isnan(dir)))
    {
        return;
    }

    auto* parentTransform = getJointParent(registry, currentJointTransform.getThisEntity());

    // constraint for current joint using parent joint.
    /*if(parentJoint && parentJoint->m_useRotationConstraints && isBackwardPass)
    {
        // dir = parentJoint->m_constraintDeltaRotation * dir;
        const auto* parentJointParentTransform = getJointParent(registry, parentJoint->getThisEntity());
        const auto constraintDelta =
            calculateConstraintDelta(*parentJoint,
                nextWorldTransform,
                nextLocalTransform,
                worldTransform,
                parentJointParentTransform ? &parentJointParentTransform->m_worldTransform : nullptr);

        dir = constraintDelta * dir;

        // std::println(std::cout, "constraintDelta: {}", glm::to_string(glm::degrees(glm::eulerAngles(constraintDelta))));
    }*/

    worldTransform.m_position = nextWorldTransform.m_position + dir * boneLength;

    localTransform.m_position = calculateJointLocalPosition(registry, currentJointTransform.getThisEntity(),
                                                            worldTransform.m_position, parentTransform);

    /*std::println(std::cout, "1111 normalized dir: {}, next final pos: {}, next local pos: {}, final pos: {}, local pos: {}, new boneLength: {}, boneLength: {}",
                 glm::to_string(dir),
                 glm::to_string(nextWorldTransform.m_position),
                 glm::to_string(nextLocalTransform.m_position),
                 glm::to_string(worldTransform.m_position),
                 glm::to_string(localTransform.m_position),
                 glm::distance(worldTransform.m_position, nextWorldTransform.m_position),
                 boneLength);*/
}

glm::vec3 SGCore::IKResolver::calculateJointLocalPosition(ECS::registry_t& registry, ECS::entity_t jointEntity,
                                                          const glm::vec3& jointWorldPosition, const Transform* parentTransform) noexcept
{
    if(parentTransform)
    {
        return TransformUtils::calculateLocalPosition(*parentTransform, jointWorldPosition);
    }

    return jointWorldPosition;
}

SGCore::Transform* SGCore::IKResolver::getJointParent(ECS::registry_t& registry, ECS::entity_t jointEntity) noexcept
{
    auto& jointBaseInfo = registry.get<EntityBaseInfo>(jointEntity);

    Transform* parentTransform {};

    {
        auto tmpTransform = registry.tryGet<Transform>(jointBaseInfo.getParent());
        parentTransform = tmpTransform ? tmpTransform->get() : nullptr;
    }

    return parentTransform;
}

glm::quat SGCore::IKResolver::calculateConstraintDelta(IKJoint& joint, const TransformBase& jointWorldTransform,
                                                       const TransformBase& jointLocalTransform,
                                                       const TransformBase& nextWorldTransform,
                                                       const TransformBase* parentWorldTransform) noexcept
{
    const auto localRot = jointLocalTransform.m_rotation;

    const auto euler = glm::degrees(glm::eulerAngles(localRot));
    const auto clamped = glm::clamp(euler, joint.m_constraintMinRotation, joint.m_constraintMaxRotation);
    // calculating clamped rotation
    const auto newRot = glm::normalize(glm::quat(glm::radians(clamped)));

    // getting delta between current joint rotation and new joint rotation (clamped)
    return newRot * glm::inverse(localRot);
    // direction to next joint
    /*const auto dir = glm::normalize(jointWorldTransform.m_position - nextWorldTransform.m_position);

    // rotation reference axis
    auto referenceAxis = joint.m_rotationDirectionReference;
    if(parentWorldTransform)
    {
        // translating reference axis from world space to parent space
        referenceAxis = parentWorldTransform->m_rotation * referenceAxis;
    }

    // calculating current joint rotation using direction to next joint and reference axis
    const auto localRot = glm::rotation(referenceAxis, dir);

    const auto euler = glm::degrees(glm::eulerAngles(localRot));
    const auto clamped = glm::clamp(euler, joint.m_constraintMinRotation, joint.m_constraintMaxRotation);
    // calculating clamped rotation
    const auto newRot = glm::normalize(glm::quat(glm::radians(clamped)));

    // getting delta between current joint rotation and new joint rotation (clamped)
    return newRot * glm::inverse(localRot);*/
    // return localRot * glm::inverse(newRot);
}
