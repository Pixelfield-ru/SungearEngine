//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_OCTREESSOLVER_H
#define SUNGEARENGINE_OCTREESSOLVER_H

#include <thread>
#include "SGCore/Scene/ISystem.h"
#include "SGUtils/EventListener.h"
#include "SGUtils/Timer.h"
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
        
        void setScene(const Ref<Scene>& scene) noexcept final;
        
    private:
        void onTransformChanged(const entity_t& entity, const Ref<const Transform>& transform) noexcept;

        EventListener<void(entt::basic_registry<entity_t> & , const entity_t&,
                Ref<const Transform>)> m_transformChangedListener =
        [this](entt::basic_registry<entity_t>& registry, const entity_t& entity, Ref<const Transform> transform) {
            onTransformChanged(entity, transform);
        };
        
        std::vector<std::pair<entity_t, Ref<const Transform>>> m_changedTransforms;
    };
}

#endif //SUNGEARENGINE_OCTREESSOLVER_H
