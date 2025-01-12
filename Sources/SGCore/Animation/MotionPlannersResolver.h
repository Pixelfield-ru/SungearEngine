//
// Created by stuka on 07.01.2025.
//

#ifndef SUNGEARENGINE_ANIMATORSSYSTEM_H
#define SUNGEARENGINE_ANIMATORSSYSTEM_H

#include "SGCore/Scene/ISystem.h"
#include "MotionPlanner.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Scene/EntityBaseInfo.h"

namespace SGCore
{
    struct MotionPlannersResolver : ISystem
    {
        void fixedUpdate(const double& dt, const double& fixedDt) final;

    private:
        static void processMotionNodes(const double& dt,
                                       SGCore::Ref<SGCore::ECS::registry_t>& inRegistry,
                                       MotionPlanner& motionPlanner,
                                       const std::vector<Ref<MotionPlannerNode>>& nodesToInterpolate,
                                       const AssetRef<Skeleton>& skeleton,
                                       const ECS::entity_t& currentEntity,
                                       const EntityBaseInfo::reg_t& currentEntityBaseInfo,
                                       const Transform::reg_t& currentEntityTransform,
                                       const Transform::reg_t& parentEntityTransform,
                                       std::int32_t& bonesCount) noexcept;

        static void collectAndUpdateNodesToInterpolate(const double& dt,
                                                       const Ref<MotionPlannerNode>& currentNode,
                                                       std::vector<Ref<MotionPlannerNode>>& nodesToInterpolate) noexcept;
    };
}

#endif //SUNGEARENGINE_ANIMATORSSYSTEM_H
