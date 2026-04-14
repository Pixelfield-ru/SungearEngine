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

    std::vector<std::vector<IKChainNode>> jointsChains;
    // in chain
    std::vector<Transform*> jointsTransforms;
    std::vector<IKJoint*> jointsComponents;

    ikRootsView.each([&](const EntityBaseInfo::reg_t& entityBaseInfo, const Transform::reg_t&, const IKRootJoint::reg_t& rootJoint, const IKJoint::reg_t&) {
        jointsChains = rootJoint.getCachedChains();

        // processing joints chains
        for(size_t i = 0; i < jointsChains.size(); ++i)
        {
            const auto& chain = jointsChains[i];

            if(chain.size() < 2) continue;

            jointsTransforms.clear();

            const auto jointsCount = std::ssize(chain);

            // collecting transforms
            for(auto jointNode : chain)
            {
                auto jointTransform = registry->get<Transform>(jointNode.m_jointEntity);
                auto& jointComponent = registry->get<IKJoint>(jointNode.m_jointEntity);

                jointComponent.m_constraintMinRotation = glm::max(jointComponent.m_constraintMinRotation, glm::vec3 { -180.0f });
                jointComponent.m_constraintMaxRotation = glm::min(jointComponent.m_constraintMaxRotation, glm::vec3 { 180.0f });

                jointsTransforms.push_back(jointTransform.get());
                jointsComponents.push_back(&jointComponent);
            }

            auto& r = jointsComponents[0];
            if(!r->m_updateIK) continue;

            // GUARANTEED THAT jointTransform.size() == jointComponent.size()

            // =======================================================
            // ======================================================= FABRIK
            // =======================================================

            auto& endJoint = registry->get<IKJoint>(chain[jointsCount - 1].m_jointEntity);

            if(!endJoint.m_targetPosition) continue;

            // set end joint to target position
            jointsTransforms[jointsCount - 1]->m_worldTransform.m_position = *endJoint.m_targetPosition;

            const auto rootJointOriginalPos = jointsTransforms[0]->m_worldTransform.m_position;

            // ============= FABRIK forward
            for(std::ptrdiff_t j = jointsCount - 1; j >= 1; --j)
            {
                const auto boneLength = chain[j - 1].m_boneLength;
                const auto& joint = *jointsComponents[j - 1];

                auto& worldTransform = jointsTransforms[j]->m_worldTransform;
                auto& prevWorldTransform = jointsTransforms[j - 1]->m_worldTransform;
                auto& prevLocalTransform = jointsTransforms[j - 1]->m_worldTransform;
                auto prevPrevPosition = (j > 1) ? jointsTransforms[j - 2]->m_worldTransform.m_position : rootJointOriginalPos;

                // prevprev to prev
                const auto prevPrevDir = glm::normalize(prevWorldTransform.m_position - prevPrevPosition);
                // const auto prevPrevDir = glm::normalize(prevPrevPosition - prevWorldTransform.m_position);
                const auto rootRotation = glm::rotation(joint.m_rotationDirectionReference, prevPrevDir);

                const auto prevDir = glm::normalize(worldTransform.m_position - prevWorldTransform.m_position);
                // const auto prevDir = glm::normalize(prevWorldTransform.m_position - worldTransform.m_position);
                const auto diffRotationRaw = glm::inverse(rootRotation) * glm::rotation(joint.m_rotationDirectionReference, prevDir);

                const auto euler = glm::degrees(glm::eulerAngles(diffRotationRaw));
                const auto clamped = glm::clamp(euler, joint.m_constraintMinRotation, joint.m_constraintMaxRotation);

                const auto rotation = rootRotation * glm::quat(glm::radians(clamped));

                prevWorldTransform.m_position = worldTransform.m_position + (rotation * joint.m_rotationDirectionReference) * boneLength;

                const auto* parentTransform = getJointParent(*registry, jointsTransforms[j - 1]->getThisEntity());
                if(parentTransform)
                {
                    prevLocalTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, prevWorldTransform.m_position);
                }

                /*IKJoint* parentJoint {};
                if(j - 1 >= 0)
                {
                    parentJoint = jointsComponents[j - 1];
                }*/
                /*jointPass(*registry, *jointsComponents[j], *jointsComponents[j + 1], parentJoint, *jointsTransforms[j],
                          *jointsTransforms[j + 1],
                          chain[j].m_boneLength, false);*/


            }

            // ==========

            // =============== set original position for fixed joints
            const auto* rootJointParent = getJointParent(*registry, chain[0].m_jointEntity);

            jointsTransforms[0]->m_worldTransform.m_position = rootJointOriginalPos;

            jointsTransforms[0]->m_localTransform.m_position = calculateJointLocalPosition(
                *registry,
                jointsTransforms[0]->getThisEntity(),
                jointsTransforms[0]->m_worldTransform.m_position, rootJointParent);

            // =============== FABRIK backwards
            auto rootRotation = glm::identity<glm::quat>();

            for(std::ptrdiff_t j = 0; j < jointsCount - 1; ++j)
            {
                const auto boneLength = chain[j].m_boneLength;

                const auto& joint = *jointsComponents[j];

                auto& worldTransform = jointsTransforms[j]->m_worldTransform;
                auto& nextWorldTransform = jointsTransforms[j + 1]->m_worldTransform;
                auto& nextLocalTransform = jointsTransforms[j + 1]->m_localTransform;

                const auto dir = glm::normalize(nextWorldTransform.m_position - worldTransform.m_position);
                // const auto dir = glm::normalize(worldTransform.m_position - nextWorldTransform.m_position);
                const auto diffRotationRaw = glm::inverse(rootRotation) *  glm::rotation(joint.m_rotationDirectionReference, dir);

                const auto euler = glm::degrees(glm::eulerAngles(diffRotationRaw));
                const auto clamped = glm::clamp(euler, joint.m_constraintMinRotation, joint.m_constraintMaxRotation);

                const auto rotation = rootRotation * glm::quat(glm::radians(clamped));

                nextWorldTransform.m_position = worldTransform.m_position + (rotation * joint.m_rotationDirectionReference) * boneLength;

                rootRotation = rotation;

                const auto* parentTransform = getJointParent(*registry, jointsTransforms[j + 1]->getThisEntity());
                if(parentTransform)
                {
                    nextLocalTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, nextWorldTransform.m_position);
                }

                /*jointPass(*registry, *jointsComponents[j], *jointsComponents[j - 1], jointsComponents[j - 1], *jointsTransforms[j],
                          *jointsTransforms[j - 1],
                          chain[j - 1].m_boneLength, true);*/
            }

            // continue;

            // ============== calculating rotations
            // continue;
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
                    localTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, worldTransform.m_position);
                    worldTransform.m_position = TransformUtils::calculateWorldPosition(*parentTransform, localTransform.m_position);
                }

                const auto dir = glm::normalize(worldTransform.m_position - nextWorldTransform.m_position);
                // const auto dir = glm::normalize(nextWorldTransform.m_position - worldTransform.m_position);
                if(glm::any(glm::isnan(dir)))
                {
                    continue;
                }

                const auto rotationDir = worldTransform.m_rotation * joint.m_rotationDirectionReference;
                /*auto rotationDir = joint.m_rotationDirectionReference;
                if(parentTransform)
                {
                    rotationDir = parentTransform->m_worldTransform.m_rotation * rotationDir;
                }*/

                const auto delta = glm::rotation(rotationDir, dir);

                if(glm::any(glm::isnan(delta)))
                {
                    continue;
                }

                worldTransform.m_rotation = delta * worldTransform.m_rotation;
                // worldTransform.m_rotation = delta;

                if(parentTransform)
                {
                    localTransform.m_rotation = TransformUtils::calculateLocalRotation(*parentTransform, worldTransform.m_rotation);
                }
                else
                {
                    localTransform.m_rotation = worldTransform.m_rotation;
                }

                localTransform.m_rotation = glm::normalize(localTransform.m_rotation);

                std::println(std::cout, "ik joint index: {}, rotation BEFORE constraints: {}", j, glm::to_string(glm::degrees(glm::eulerAngles(localTransform.m_rotation))));

                // applying rotation constration
                /*if(joint.m_useRotationConstraints)
                {
                    const auto constraintDelta = calculateConstraintDelta(
                        joint, worldTransform, localTransform, nextWorldTransform,
                        parentTransform ? &parentTransform->m_worldTransform : nullptr);

                    joint.m_constraintDeltaRotation = constraintDelta;
                    localTransform.m_rotation = constraintDelta * localTransform.m_rotation;
                    // worldTransform.m_rotation = constraintDelta * worldTransform.m_rotation;
                }*/

                std::println(std::cout, "ik joint index: {}, rotation AFTER constraints: {}", j, glm::to_string(glm::degrees(glm::eulerAngles(localTransform.m_rotation))));

                // it may help prevent chain breakage
                /*if(parentTransform)
                {
                    worldTransform.m_rotation = TransformUtils::calculateWorldRotation(*parentTransform, localTransform.m_rotation);
                    worldTransform.m_position = TransformUtils::calculateWorldPosition(*parentTransform, localTransform.m_position);
                    localTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, worldTransform.m_position);
                }
                else
                {
                    worldTransform.m_rotation = localTransform.m_rotation;
                    worldTransform.m_position = localTransform.m_position;
                }*/
            }

            r->m_updateIK = false;
        }

        /*std::println(std::cout, "0 rotation: {}, 1 rotation: {}",
            glm::to_string(glm::degrees(glm::eulerAngles(jointsTransforms[0]->m_localTransform.m_rotation))),
            glm::to_string(glm::degrees(glm::eulerAngles(jointsTransforms[1]->m_localTransform.m_rotation)))
            );*/

        jointsChains.clear();
    });
}

void SGCore::IKResolver::jointPass(ECS::registry_t& registry, const IKJoint& joint, const IKJoint& nextJoint, IKJoint* parentJoint, Transform& currentJointTransform,
                                   const Transform& nextJointTransform, float boneLength, bool isBackwardPass) noexcept
{
    auto& worldTransform = currentJointTransform.m_worldTransform;
    auto& localTransform = currentJointTransform.m_localTransform;
    auto& nextWorldTransform = nextJointTransform.m_worldTransform;
    auto& nextLocalTransform = nextJointTransform.m_localTransform;

    glm::vec3 dir {};
    dir = glm::normalize(worldTransform.m_position - nextWorldTransform.m_position);
    if(glm::any(glm::isnan(dir)))
    {
        return;
    }

    // applying rotation constraints on forward pass
    /*if(!isBackwardPass && joint.m_useRotationConstraints)
    {
        const auto rotation = glm::rotation(glm::normalize(worldTransform.m_position), glm::normalize(nextWorldTransform.m_position)) * localTransform.m_rotation;

        const auto euler = glm::degrees(glm::eulerAngles(rotation));
        const auto clamped = glm::clamp(euler, parentJoint->m_constraintMinRotation, parentJoint->m_constraintMaxRotation);
        const auto clampedRotation = glm::quat(glm::radians(clamped));

        auto delta = glm::inverse(rotation) * clampedRotation;

        dir = glm::inverse(delta) * dir;
    }*/

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


    auto* parentTransform = getJointParent(registry, currentJointTransform.getThisEntity());

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
}
