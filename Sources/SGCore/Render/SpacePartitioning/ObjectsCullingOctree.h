//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_OBJECTSCULLINGOCTREE_H
#define SUNGEARENGINE_OBJECTSCULLINGOCTREE_H

#include "SGCore/ECS/Component.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct ObjectsCullingOctree : ECS::Component<Ref<ObjectsCullingOctree>, Ref<const ObjectsCullingOctree>>
    {
    private:
        bool m_dummy = true;
    };
}

#endif //SUNGEARENGINE_OBJECTSCULLINGOCTREE_H
