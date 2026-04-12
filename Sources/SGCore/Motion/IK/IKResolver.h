//
// Created by stuka on 21.08.2025.
//

#pragma once

#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>

#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct Transform;
    struct TransformBase;
    struct IKJoint;

    struct SGCORE_EXPORT IKResolver : ISystem
    {
        sg_implement_type_id(SGCore::IKResolver)

        void update(double dt, double fixedDt) final;

    private:
        static void collectJoints(const ECS::registry_t& inRegistry,
                                  const EntityBaseInfo::reg_t& currentIKEntityInfo,
                                  std::vector<std::vector<ECS::entity_t>>& jointsChains,
                                  std::vector<ECS::entity_t>& currentJointsChain) noexcept;

        static void jointPass(ECS::registry_t& registry, const IKJoint& joint, const IKJoint& nextJoint, IKJoint* parentJoint, Transform& currentJointTransform,
                              const Transform& nextJointTransform, float boneLength, bool isBackwardPass) noexcept;

        static glm::vec3 calculateJointLocalPosition(ECS::registry_t& registry, ECS::entity_t jointEntity,
                                                     const glm::vec3& jointWorldPosition, const Transform* parentTransform) noexcept;

        static Transform* getJointParent(ECS::registry_t& registry, ECS::entity_t jointEntity) noexcept;

        static glm::quat calculateConstraintDelta(IKJoint& joint, const TransformBase& jointWorldTransform,
                                                  const TransformBase& jointLocalTransform,
                                                  const TransformBase& nextWorldTransform,
                                                  const TransformBase* parentWorldTransform) noexcept;
    };
}
