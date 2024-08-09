//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_OCTREESSOLVER_H
#define SUNGEARENGINE_OCTREESSOLVER_H

#include <SGCore/pch.h>

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Utils/EventListener.h"
#include "SGCore/Utils/Timer.h"
#include "SGCore/Scene/IParallelSystem.h"
#include "SGCore/Scene/EntityComponentMember.h"

namespace SGCore
{
    struct Transform;
    struct OctreeNode;
    
    class OctreesSolver : public IParallelSystem<OctreesSolver>
    {
    public:
        OctreesSolver();

        void fixedUpdate(const double& dt, const double& fixedDt) noexcept final;
        
        void onAddToScene(const Ref<Scene>& scene) final;
        
    private:
        void onTransformChanged(const entity_t& entity, const Ref<const Transform>& transform) noexcept;

        EventListener<void(const Ref<registry_t>& registry, const entity_t&,
                Ref<const Transform>)> m_transformChangedListener =
        [this](const Ref<registry_t>& registry, const entity_t& entity, Ref<const Transform> transform) {
            onTransformChanged(entity, transform);
        };
        
        std::vector<std::pair<entity_t, Ref<const Transform>>> m_changedTransforms;
    };
}

#endif //SUNGEARENGINE_OCTREESSOLVER_H
