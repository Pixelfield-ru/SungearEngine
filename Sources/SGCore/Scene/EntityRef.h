//
// Created by stuka on 17.12.2024.
//

#ifndef SUNGEARENGINE_ENTITYREF_H
#define SUNGEARENGINE_ENTITYREF_H

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct EntityRef
    {
        Ref<entity_t> m_referencedEntity;
    };
}

#endif //SUNGEARENGINE_ENTITYREF_H
