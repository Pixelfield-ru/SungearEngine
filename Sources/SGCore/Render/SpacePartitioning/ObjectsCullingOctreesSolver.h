//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_OBJECTSCULLINGOCTREESSOLVER_H
#define SUNGEARENGINE_OBJECTSCULLINGOCTREESSOLVER_H

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Scene/IParallelSystemSubprocess.h"
#include "OctreesSolver.h"
#include "SGUtils/Math/Frustum.h"

namespace SGCore
{
    struct ObjectsCullingOctreesSolver : public IParallelSystemSubprocess<OctreesSolver>
    {
        void fixedUpdate(const double& dt, const double& fixedDt, Ref<OctreesSolver> parent) noexcept final;
        
    private:
        void testNode(entt::registry& registry, const entt::entity& cameraEntity, const Frustum& cameraFrustum, Ref<OctreeNode> node) noexcept;
    };
}

#endif //SUNGEARENGINE_OBJECTSCULLINGOCTREESSOLVER_H
