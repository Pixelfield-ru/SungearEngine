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

    struct EntityBaseInfo : public UniqueNameWrapper
    {
        entt::entity m_parent = entt::null;

        Weak<Layer> m_layer;
    };
}

#endif //SUNGEARENGINE_ENTITYBASEINFO_H