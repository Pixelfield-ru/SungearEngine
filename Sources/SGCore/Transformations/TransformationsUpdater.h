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
        Threading::WrappedObject<std::vector<EntityComponentMember<glm::mat4>>> m_changedModelMatrices;
        Threading::WrappedObject<std::vector<ECS::entity_t>> m_entitiesForPhysicsUpdateToCheck;
        
        // TODO: FIX. MAY PRODUCE SIGSEGV WHEN ITERATING THROUGH IN ONE THREAD AND push_back IN OTHER
        // Reviewed MisterElect 04.10.2024: no sigsegv found
        Threading::WrappedObject<std::vector<EntityComponentMember<const Transform::reg_t>>> m_calculatedNotPhysicalEntities;
        Threading::WrappedObject<std::vector<EntityComponentMember<const Transform::reg_t>>> m_calculatedPhysicalEntities;

        Threading::WrappedObject<std::vector<EntityComponentMember<const Transform::reg_t>>> m_calculatedNotPhysicalEntitiesCopy;
        Threading::WrappedObject<std::vector<EntityComponentMember<const Transform::reg_t>>> m_calculatedPhysicalEntitiesCopy;
    };
}
