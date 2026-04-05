//
// Created by stuka on 21.08.2025.
//

#pragma once

#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct Transform;

    struct SGCORE_EXPORT IKResolver : ISystem
    {
        sg_implement_type_id(SGCore::IKResolver)

        void update(double dt, double fixedDt) final;

    private:
        static void collectJoints(const ECS::registry_t& inRegistry,
                                  const EntityBaseInfo::reg_t& currentIKEntityInfo,
                                  std::vector<std::vector<ECS::entity_t>>& jointsChains,
                                  std::vector<ECS::entity_t>& currentJointsChain) noexcept;

        static void jointPass(ECS::registry_t& registry, Transform& currentJointTransform,
                              const Transform& nextJointTransform, float boneLength) noexcept;

        static glm::vec3 calculateJointLocalPosition(ECS::registry_t& registry, ECS::entity_t jointEntity,
                                                     const glm::vec3& jointWorldPosition) noexcept;
    };
}
