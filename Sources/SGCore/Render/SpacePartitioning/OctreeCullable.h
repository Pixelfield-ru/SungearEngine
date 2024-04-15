//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_CULLABLEMESH_H
#define SUNGEARENGINE_CULLABLEMESH_H

#include <entt/entity/entity.hpp>
#include <unordered_set>
#include <functional>
#include <set>

#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/EventListener.h"

namespace SGCore
{
    struct OctreeNode;
    
    struct OctreeCullable
    {
        Weak<OctreeNode> m_parentNode;
        
    private:
        bool m_dummy = true;
    };
}

#endif //SUNGEARENGINE_CULLABLEMESH_H
