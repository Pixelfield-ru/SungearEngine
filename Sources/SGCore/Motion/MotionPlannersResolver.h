//
// Created by stuka on 07.01.2025.
//

#pragma once

#include "SGCore/Scene/ISystem.h"
#include "MotionPlanner.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Scene/EntityBaseInfo.h"

namespace SGCore
{
    struct DebugDraw;
    struct SkeletalAnimationNode;

    /**
     * MotionPlannersResolver is required system to resolve skeletal animations nodes, 3D ragdolls and IK.
     */
    struct SGCORE_EXPORT MotionPlannersResolver : ISystem
    {
        sg_implement_type_id(SGCore::MotionPlannersResolver)

        void fixedUpdate(double dt, double fixedDt) final;

    private:
        void processMotionNodes(const double& dt,
                                SGCore::Ref<SGCore::ECS::registry_t>& inRegistry,
                                MotionPlanner& motionPlanner,
                                const std::vector<Ref<SkeletalAnimationNode>>& nodesToInterpolate,
                                const AssetRef<Skeleton>& skeleton,
                                const ECS::entity_t& currentEntity,
                                const EntityBaseInfo::reg_t& currentEntityBaseInfo,
                                const Transform::reg_t& currentEntityTransform,
                                const Transform::reg_t& parentEntityTransform,
                                std::int32_t& bonesCount) noexcept;

        Weak<DebugDraw> m_debugDrawPassRef;
    };
}
