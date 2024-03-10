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
        void parallelUpdate(const double& dt, const double& fixedDt) noexcept final;
        
        void setScene(const Ref<Scene>& scene) noexcept final;
        
    private:
        std::mutex m_solveMutex;
        
        void onTransformChanged(const entt::entity& entity, const Ref<const Transform>& transform) noexcept;
    
        EventListener<void(entt::registry&, const entt::entity&,
                           Ref<const Transform>)> m_transformChangedListener = MakeEventListener<void(entt::registry&,
                                                                                                      const entt::entity&,
                                                                                                      Ref<const Transform>)>(
                [this](entt::registry& registry, const entt::entity& entity, Ref<const Transform> transform) {
                    onTransformChanged(entity, transform);
                });
    
        std::atomic<size_t> m_lastSize = 0;
        std::vector<std::pair<entt::entity, Ref<const Transform>>> m_changedTransforms;
    };
}

#endif //SUNGEARENGINE_OCTREESSOLVER_H
