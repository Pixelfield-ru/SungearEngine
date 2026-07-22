//
// Created by stuka on 10.02.2026.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct PhysicsWorld3D;
    struct Rigidbody3D;

    struct Ragdoll3D : ECS::Component<Ragdoll3D, const Ragdoll3D>
    {

    };
}
