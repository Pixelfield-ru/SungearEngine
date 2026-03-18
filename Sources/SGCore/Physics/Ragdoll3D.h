//
// Created by stuka on 10.02.2026.
//

#pragma once

#include <functional>
#include <vector>

#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>
#include <entt/entity/entity.hpp>
#include <glm/vec3.hpp>

#include "SGCore/ECS/Component.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore
{
    struct PhysicsWorld3D;
    struct Rigidbody3D;

    struct ConstraintInfo
    {
        Ref<btTypedConstraint> m_constraint;
        ECS::entity_t m_entityA = entt::null;
        ECS::entity_t m_entityB = entt::null;
    };

    struct Ragdoll3D : ECS::Component<Ragdoll3D, const Ragdoll3D>
    {
        Ragdoll3D(const Ref<PhysicsWorld3D>& physicsWorld) noexcept;
        Ragdoll3D() noexcept = default;
        copy_constructor(Ragdoll3D) = default;
        move_constructor(Ragdoll3D) = default;

        ConstraintInfo addPointToPointConstraint(ECS::entity_t entityA, ECS::entity_t entityB, const glm::vec3& pointA, const glm::vec3& pointB, ECS::registry_t& inRegistry) noexcept;
        ConstraintInfo addConeTwistConstraint(ECS::entity_t entityA, ECS::entity_t entityB, ECS::registry_t& inRegistry) noexcept;

        [[nodiscard]] const std::vector<ConstraintInfo>& getConstraints() const noexcept;

        copy_operator(Ragdoll3D) = default;
        move_operator(Ragdoll3D) = default;

    private:
        ConstraintInfo addBodyToBodyConstraint(ECS::entity_t entityA,
                                               ECS::entity_t entityB,
                                               const std::function<Ref<btTypedConstraint>(Rigidbody3D&, Rigidbody3D&)>& constraintCreationFunc,
                                               ECS::registry_t& inRegistry) noexcept;

        Weak<PhysicsWorld3D> m_parentPhysicsWorld;

        std::vector<ConstraintInfo> m_constraints;
    };
}
