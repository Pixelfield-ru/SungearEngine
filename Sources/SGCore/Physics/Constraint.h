//
// Created by stuka on 19.03.2026.
//

#pragma once

#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct ConstraintInfo
    {
        Ref<btTypedConstraint> m_constraint;
        ECS::entity_t m_entityA = entt::null;
        ECS::entity_t m_entityB = entt::null;
    };
}