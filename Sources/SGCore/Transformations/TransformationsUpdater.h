//
// Created by stuka on 02.05.2023.
//

#pragma once

#include <SGCore/pch.h>

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Utils/Timer.h"
#include "SGCore/Scene/EntityComponentMember.h"
#include "SGCore/Utils/Signal.h"
#include "Transform.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Scene/IParallelSystem.h"
#include "SGCore/Threading/WrappedObject.h"

namespace SGCore
{
    class IMeshData;

    struct TransformBase;

    struct TransformationsUpdater : public IParallelSystem<TransformationsUpdater>
    {
        sg_implement_type_id(TransformationsUpdater, 20)

        friend class PhysicsWorld3D;

        TransformationsUpdater();

        void update(const double& dt, const double& fixedDt) noexcept final;

        // main thread
        void fixedUpdate(const double& dt, const double& fixedDt) noexcept final;
        
        Signal<void(const Ref<ECS::registry_t>& registry, const ECS::entity_t&, const Transform::const_reg_t)> onTransformChanged;
        
    private:
        void updateTransform(const EntityBaseInfo::reg_t& currentEntityBaseInfo,
                             const ECS::entity_t& currentEntity,
                             const Transform::reg_t& currentEntityTransform,
                             const Transform::reg_t& parentTransform,
                             const Ref<ECS::registry_t>& inRegistry);
    };
}
