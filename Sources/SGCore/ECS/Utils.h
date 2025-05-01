//
// Created by stuka on 01.05.2025.
//

#ifndef SUNGEARENGINE_ECSUTILS_H
#define SUNGEARENGINE_ECSUTILS_H

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::ECS
{
    struct Utils final
    {
        static entity_t createDecal(registry_t& inRegistry, entity_t parentEntity = entt::null) noexcept;
    };
}

#endif // SUNGEARENGINE_ECSUTILS_H
