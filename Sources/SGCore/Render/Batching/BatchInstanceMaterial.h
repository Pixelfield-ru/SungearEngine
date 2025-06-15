//
// Created by stuka on 15.06.2025.
//

#ifndef SUNGEARENGINE_BATCHINSTANCEMATERIAL_H
#define SUNGEARENGINE_BATCHINSTANCEMATERIAL_H

#include <entt/entity/entity.hpp>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct BatchInstanceMaterial
    {
        ECS::entity_t m_entity = entt::null;

    };
}

#endif // SUNGEARENGINE_BATCHINSTANCEMATERIAL_H
