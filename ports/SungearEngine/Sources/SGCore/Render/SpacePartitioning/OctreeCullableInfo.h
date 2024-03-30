//
// Created by ilya on 11.03.24.
//

#ifndef SUNGEARENGINE_OCTREECULLABLEINFO_H
#define SUNGEARENGINE_OCTREECULLABLEINFO_H

#include <functional>
#include <unordered_set>
#include <entt/entity/entity.hpp>

#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/Event.h"
#include "SGUtils/Utils.h"

namespace SGCore
{
    struct OctreeNode;
    
    struct OctreeCullableInfo
    {
        Weak<OctreeNode> m_parentNode;
        
    private:
        bool m_dummy = true;
    };
}

#endif //SUNGEARENGINE_OCTREECULLABLEINFO_H
