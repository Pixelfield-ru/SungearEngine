//
// Created by stuka on 02.05.2023.
//

#pragma once

#include <stack>

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Utils/Signal.h"
#include "Transform.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Scene/IParallelSystem.h"

namespace SGCore
{
    class IMeshData;

    struct TransformBase;

    struct SGCORE_EXPORT TransformationsUpdater : public IParallelSystem<TransformationsUpdater>
    {
        sg_implement_type_id(SGCore::TransformationsUpdater)

        friend struct PhysicsWorld3D;

        TransformationsUpdater();

        void update(double dt, double fixedDt) noexcept final;

        // main thread
        void fixedUpdate(double dt, double fixedDt) noexcept final;
        
        Signal<void(const Ref<ECS::registry_t>& registry, const ECS::entity_t&, const Transform::const_reg_t)> onTransformChanged;
        
    private:
        struct EntityDesc
        {
            ECS::entity_t m_entity = entt::null;
            const EntityBaseInfo* m_baseInfo {};
            const Ref<Transform>* m_transform {};
            Transform* m_parentTransform {};
        };

        std::stack<EntityDesc> m_entitiesDesc;
    };
}
