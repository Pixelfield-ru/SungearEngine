//
// Created by stuka on 02.05.2023.
//

#pragma once

#include <entt/entity/observer.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/entity.hpp>
#include <thread>

#include "SGCore/Scene/ISystem.h"
#include "SGUtils/Utils.h"
#include "SGUtils/Timer.h"
#include "SGCore/Threading/SafeObject.h"
#include "SGCore/Scene/EntityComponentMember.h"
#include "SGUtils/Event.h"
#include "Transform.h"
#include "SGCore/Threading/FixedVector.h"
#include "SGCore/Scene/IParallelSystem.h"

namespace SGCore
{
    class IMeshData;

    struct TransformBase;
    
    struct TransformationsUpdater : public IParallelSystem<TransformationsUpdater>
    {
        friend class PhysicsWorld3D;

        TransformationsUpdater();

        void parallelUpdate(const double& dt, const double& fixedDt) noexcept final;

        // main thread
        void fixedUpdate(const double& dt, const double& fixedDt) noexcept final;
        
        void setScene(const Ref<Scene>& scene) noexcept final;
        
        Event<void(entt::registry&, const entt::entity&, Ref<const Transform>)> m_transformChangedEvent = MakeEvent<void(entt::registry&, const entt::entity&, Ref<const Transform>)>();
        
    private:
        Ref<Scene> m_sharedScene;

        SafeObject<std::vector<EntityComponentMember<glm::mat4>>> m_changedModelMatrices;
        SafeObject<std::vector<entt::entity>> m_entitiesForPhysicsUpdateToCheck;
        
        // TODO: FIX. MAY PRODUCE SIGSEGV WHEN ITERATING THROUGH IN ONE THREAD AND push_back IN OTHER
        SafeObject<FixedVector<EntityComponentMember<Ref<const Transform>>>> m_calculatedNotPhysicalEntities;
        SafeObject<FixedVector<EntityComponentMember<Ref<const Transform>>>> m_calculatedPhysicalEntities;
    };
}
