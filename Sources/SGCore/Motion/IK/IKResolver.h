//
// Created by stuka on 21.08.2025.
//

#pragma once

#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct SGCORE_EXPORT IKResolver : ISystem
    {
        sg_implement_type_id(IKResolver, 51)

        void fixedUpdate(const double& dt, const double& fixedDt) final;

    private:
        static void collectJoints(const ECS::registry_t& inRegistry,
                                  const EntityBaseInfo::reg_t& currentIKEntityInfo,
                                  std::vector<std::vector<ECS::entity_t>>& jointsChains,
                                  std::vector<ECS::entity_t>& currentJointsChain) noexcept;
    };
}
