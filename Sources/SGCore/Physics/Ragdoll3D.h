//
// Created by stuka on 10.02.2026.
//

#pragma once

#include <vector>

#include <BulletDynamics/ConstraintSolver/btConeTwistConstraint.h>
#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>
#include <entt/entity/entity.hpp>

#include "SGCore/ECS/Component.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Serde/Defines.h"

namespace SGCore
{
    class PhysicsWorld3D;
    struct Rigidbody3D;

    struct ConstraintInfo
    {
        Ref<btConeTwistConstraint> m_constraint;
        ECS::entity_t m_entityA = entt::null;
        ECS::entity_t m_entityB = entt::null;
    };

    struct Ragdoll3D : ECS::Component<Ragdoll3D, const Ragdoll3D>
    {
        Ragdoll3D(const Ref<PhysicsWorld3D>& physicsWorld) noexcept;
        Ragdoll3D() noexcept = default;
        copy_constructor(Ragdoll3D) = default;
        move_constructor(Ragdoll3D) = default;

        ConstraintInfo addBone(ECS::entity_t boneEntityA, ECS::entity_t boneEntityB, ECS::registry_t& inRegistry) noexcept;

        [[nodiscard]] const std::vector<ConstraintInfo>& getConstraints() const noexcept;

        copy_operator(Ragdoll3D) = default;
        move_operator(Ragdoll3D) = default;

    private:
        Weak<PhysicsWorld3D> m_parentPhysicsWorld;

        std::vector<ConstraintInfo> m_constraints;
    };
}
