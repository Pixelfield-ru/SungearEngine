//
// Created by ilya on 10.03.24.
//

#pragma once

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Utils/Slot.h"
#include "SGCore/Utils/Timer.h"
#include "SGCore/Scene/IParallelSystem.h"
#include "SGCore/Transformations/Transform.h"

namespace SGCore
{
    struct OctreeNode;
    
    class SGCORE_EXPORT OctreesSolver : public IParallelSystem<OctreesSolver>
    {
    public:
        sg_implement_type_id(SGCore::OctreesSolver)

        OctreesSolver();

        void fixedUpdate(double dt, double fixedDt) noexcept final;
        
        void onAddToScene(const Scene* scene) final;
        
    private:
        void onTransformChanged(const ECS::entity_t& entity, const Transform::const_reg_t& transform) noexcept;

        Slot<void(const Ref<ECS::registry_t>& registry, const ECS::entity_t&, const Transform&)> m_transformChangedListener =
        [this](const Ref<ECS::registry_t>& registry, const ECS::entity_t& entity, const Transform& transform) {
            onTransformChanged(entity, transform);
        };

        std::mutex m_changedTransformsMutex;
        std::vector<std::pair<ECS::entity_t, Transform::const_reg_t>> m_changedTransforms;
    };
}
