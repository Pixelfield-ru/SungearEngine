//
// Created by stuka on 02.05.2023.
//

#pragma once

#include <SGCore/pch.h>

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Utils/Timer.h"
#include "SGCore/Threading/SafeObject.h"
#include "SGCore/Scene/EntityComponentMember.h"
#include "SGCore/Utils/Event.h"
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
        
        void onAddToScene(const Ref<Scene>& scene) final;
        
        Event<void(const Ref<registry_t>& registry, const entity_t&, Ref<const Transform>)> onTransformChanged;
        
    private:
        Ref<Scene> m_sharedScene;

        SafeObject<std::vector<EntityComponentMember<glm::mat4>>> m_changedModelMatrices;
        SafeObject<std::vector<entity_t>> m_entitiesForPhysicsUpdateToCheck;
        
        // TODO: FIX. MAY PRODUCE SIGSEGV WHEN ITERATING THROUGH IN ONE THREAD AND push_back IN OTHER
        SafeObject<std::vector<EntityComponentMember<Ref<const Transform>>>> m_calculatedNotPhysicalEntities;
        SafeObject<std::vector<EntityComponentMember<Ref<const Transform>>>> m_calculatedPhysicalEntities;
        
        std::atomic<bool> m_canCopyEntities = true;
        std::vector<EntityComponentMember<Ref<const Transform>>> m_calculatedNotPhysicalEntitiesCopy;
        
        std::atomic<bool> m_canCopyNotPhysicalEntities = true;
        std::vector<EntityComponentMember<Ref<const Transform>>> m_calculatedPhysicalEntitiesCopy;
    };
}
