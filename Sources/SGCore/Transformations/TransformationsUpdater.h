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

/*
 * в этой системе (TransformationsUpdater) пробегаемся по всем сущностям с трансформом и смотрим, изменился ли трансформ. если нет, то добавляем
 * сущность в вектор m_notTransformUpdatedEntities. далее в системе физике пробегаемся по всем сущностям в этом векторе и смотрим
 * изменился ли трансформ у rigidbody. если изменился, то сразу на лету изменяем трансформ сущности.
 * ВАЖНО: ОЧИЩАТЬ m_notTransformUpdatedEntitiesSet И m_notTransformUpdatedEntities НАДО ИМЕННО В СИСТЕМЕ ФИЗИКИ, ТАК КАК НАДО СНАЧАЛА УБЕДИТЬСЯ,
 * ЧТО ТРАНСФОРМ ВСЕХ СУЩНОСТЕЙ БЫЛ ОБНОВЛЁН НА ОСНОВЕ ФИЗИКИ.
 * m_notTransformUpdatedEntitiesSet нужен для того, чтобы в TransformationsUpdater не добавлять одну и ту же сущность много раз
 */
namespace SGCore
{
    class IMeshData;

    struct TransformBase;

    struct TransformationsUpdater : public IParallelSystem<TransformationsUpdater>
    {
        sg_implement_type_id(TransformationsUpdater, 20)

        friend class PhysicsWorld3D;

        TransformationsUpdater();

        void parallelUpdate(const double& dt, const double& fixedDt) noexcept final;

        // main thread
        void fixedUpdate(const double& dt, const double& fixedDt) noexcept final;
        
        Signal<void(const Ref<ECS::registry_t>& registry, const ECS::entity_t&, const Transform::const_reg_t)> onTransformChanged;
        
    private:
        std::vector<ECS::entity_t> m_notTransformUpdatedEntities;

        std::mutex m_notTransformUpdatedEntitiesMutex;

        // todo: replace unordered_set by sparse set
        std::unordered_set<ECS::entity_t> m_notTransformUpdatedEntitiesSet;
    };
}
