//
// Created by stuka on 25.01.2024.
//

#ifndef SUNGEARENGINE_ENTITYBASEINFO_H
#define SUNGEARENGINE_ENTITYBASEINFO_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"

namespace SGCore
{
    struct Layer;

    // sizeof
    struct EntityBaseInfo : public UniqueNameWrapper
    {
        sg_serdespec_as_friend()

        entity_t m_parent = entt::null;

        // sg_member()
        Weak<Layer> m_layer;
    };
}

#endif //SUNGEARENGINE_ENTITYBASEINFO_H
