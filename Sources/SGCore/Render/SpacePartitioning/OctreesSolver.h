//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_OCTREESSOLVER_H
#define SUNGEARENGINE_OCTREESSOLVER_H

#include <SGCore/pch.h>

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Utils/Slot.h"
#include "SGCore/Utils/Timer.h"
#include "SGCore/Scene/IParallelSystem.h"
#include "SGCore/Transformations/Transform.h"

namespace SGCore
{
    struct OctreeNode;
    
    class OctreesSolver : public IParallelSystem<OctreesSolver>
    {
    public:
        sg_implement_type_id(OctreesSolver, 26)

        OctreesSolver();

        void fixedUpdate(const double& dt, const double& fixedDt) noexcept final;
        
        void onAddToScene(const Scene* scene) final;
        
    private:
        void onTransformChanged(const ECS::entity_t& entity, const Transform::const_reg_t& transform) noexcept;

        Slot<void(const Ref<ECS::registry_t>& registry, const ECS::entity_t&, Ref<const Transform>)> m_transformChangedListener =
        [this](const Ref<ECS::registry_t>& registry, const ECS::entity_t& entity, Ref<const Transform> transform) {
            onTransformChanged(entity, transform);
        };

        std::mutex m_changedTransformsMutex;
        std::vector<std::pair<ECS::entity_t, Transform::const_reg_t>> m_changedTransforms;
    };
}

#endif //SUNGEARENGINE_OCTREESSOLVER_H
