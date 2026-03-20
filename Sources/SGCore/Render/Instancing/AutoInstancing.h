//
// Created by stuka on 28.07.2025.
//

#pragma once

#include <sgcore_export.h>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct SGCORE_EXPORT AutoInstancing
    {
        /**
         * Automatically distributes an entity to an instancing object.
         * @param entity The entity that needs to be instantiated.
         * @param inRegistry Scene registry.
         * @return Instancing entity or entt::null if entity does not have mesh or it is nullptr.
         */
        static ECS::entity_t addEntity(const ECS::entity_t& entity, ECS::registry_t& inRegistry) noexcept;
    };
}
