//
// Created by ilya on 10.03.24.
//

#pragma once

#include "SGCore/ECS/Component.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct SGCORE_EXPORT ObjectsCullingOctree : ECS::Component<ObjectsCullingOctree, const ObjectsCullingOctree>
    {
    private:
        bool m_dummy = true;
    };
}
