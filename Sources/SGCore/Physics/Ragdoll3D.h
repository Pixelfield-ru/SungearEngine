//
// Created by stuka on 10.02.2026.
//

#pragma once

#include <functional>
#include <vector>

#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>
#include <entt/entity/entity.hpp>
#include <glm/vec3.hpp>

#include "Constraint.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore
{
    struct PhysicsWorld3D;
    struct Rigidbody3D;

    struct Ragdoll3D : ECS::Component<Ragdoll3D, const Ragdoll3D>
    {

    };
}
