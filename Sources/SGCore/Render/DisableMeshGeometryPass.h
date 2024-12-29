//
// Created by ilya on 28.02.24.
//

#ifndef SUNGEARENGINE_DISABLEMESHGEOMETRYPASS_H
#define SUNGEARENGINE_DISABLEMESHGEOMETRYPASS_H

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct DisableMeshGeometryPass : ECS::Component<DisableMeshGeometryPass, const DisableMeshGeometryPass>
    {
    private:
        bool m_dummy = true;
    };
}

#endif //SUNGEARENGINE_DISABLEMESHGEOMETRYPASS_H
