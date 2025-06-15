//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_OBJECTSCULLINGOCTREESSOLVER_H
#define SUNGEARENGINE_OBJECTSCULLINGOCTREESSOLVER_H

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Scene/IParallelSystemSubprocess.h"
#include "OctreesSolver.h"
#include "SGCore/Math/Frustum.h"

namespace SGCore
{
    struct ObjectsCullingOctreesSolver : public IParallelSystemSubprocess<OctreesSolver>
    {
        void fixedUpdate(const double& dt, const double& fixedDt, OctreesSolver* parent) noexcept final;
        
    private:
        void testNode(const ECS::entity_t& cameraEntity, const Frustum& cameraFrustum, const Ref<OctreeNode>& node) noexcept;
    };
}

#endif //SUNGEARENGINE_OBJECTSCULLINGOCTREESSOLVER_H
