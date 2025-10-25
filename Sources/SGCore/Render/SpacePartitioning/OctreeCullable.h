//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_CULLABLEMESH_H
#define SUNGEARENGINE_CULLABLEMESH_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct OctreeNode;
    
    struct OctreeCullable : ECS::Component<Ref<OctreeCullable>, Ref<const OctreeCullable>>
    {
        Weak<OctreeNode> m_parentNode;
        
    private:
        bool m_dummy = true;
    };
}

#endif //SUNGEARENGINE_CULLABLEMESH_H
