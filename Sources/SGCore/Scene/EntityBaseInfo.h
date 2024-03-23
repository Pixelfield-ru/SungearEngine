//
// Created by stuka on 25.01.2024.
//

#ifndef SUNGEARENGINE_ENTITYBASEINFO_H
#define SUNGEARENGINE_ENTITYBASEINFO_H

#include <set>
#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/UniqueName.h"

namespace SGCore
{
    struct Layer;

    // sizeof
    struct EntityBaseInfo : public UniqueNameWrapper
    {
        entity_t m_parent = entt::null;

        Weak<Layer> m_layer;
    };
}

#endif //SUNGEARENGINE_ENTITYBASEINFO_H
