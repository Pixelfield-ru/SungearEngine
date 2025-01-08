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
        static void processMotionNode(const double& dt,
                                      Ref<ECS::registry_t>& inRegistry,
                                      MotionPlanner::reg_t& motionPlanner,
                                      const Ref<MotionPlannerNode>& currentNode,
                                      const AssetRef<Skeleton>& skeleton,
                                      const EntityBaseInfo::reg_t& currentEntity);
    };
}

#endif //SUNGEARENGINE_ANIMATORSSYSTEM_H
