//
// Created by stuka on 10.02.2026.
//

#include "Ragdoll3D.h"

#include "SGCore/ECS/Registry.h"
#include "SGCore/Physics/Rigidbody3D.h"
#include "PhysicsWorld3D.h"

#include <BulletDynamics/ConstraintSolver/btConeTwistConstraint.h>
#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>

SGCore::Ragdoll3D::Ragdoll3D(const Ref<PhysicsWorld3D>& physicsWorld) noexcept
{
    m_parentPhysicsWorld = physicsWorld;
}

SGCore::ConstraintInfo SGCore::Ragdoll3D::addPointToPointConstraint(ECS::entity_t entityA, ECS::entity_t entityB,
                                                                    const glm::vec3& pointA,
                                                                    const glm::vec3& pointB,
                                                                    ECS::registry_t& inRegistry) noexcept
{
    const auto construction = [&pointA, &pointB](Rigidbody3D& rigidbodyA, Rigidbody3D& rigidbodyB) {
        return MakeRef<btPoint2PointConstraint>(
            *rigidbodyA.m_body,
            *rigidbodyB.m_body,
            btVector3(pointA.x, pointA.y, pointA.z),
            btVector3(pointB.x, pointB.y, pointB.z)
        );
    };

    return addBodyToBodyConstraint(entityA, entityB, construction, inRegistry);
}

SGCore::ConstraintInfo SGCore::Ragdoll3D::addConeTwistConstraint(ECS::entity_t entityA,
                                                                 ECS::entity_t entityB,
                                                                 ECS::registry_t& inRegistry) noexcept
{
    const auto construction = [](Rigidbody3D& rigidbodyA, Rigidbody3D& rigidbodyB) {
        return MakeRef<btConeTwistConstraint>(
            *rigidbodyA.m_body,
            *rigidbodyB.m_body,
            rigidbodyA.getShapeTransform(0),
            rigidbodyB.getShapeTransform(0)
        );
    };

    return addBodyToBodyConstraint(entityA, entityB, construction, inRegistry);
}

const std::vector<SGCore::ConstraintInfo>& SGCore::Ragdoll3D::getConstraints() const noexcept
{
    return m_constraints;
}

SGCore::ConstraintInfo SGCore::Ragdoll3D::addBodyToBodyConstraint(ECS::entity_t entityA,
                                                                  ECS::entity_t entityB,
                                                                  const std::function<Ref<btTypedConstraint>(Rigidbody3D&, Rigidbody3D&)>& constraintCreationFunc,
                                                                  ECS::registry_t& inRegistry) noexcept
{
    auto lockedWorld = m_parentPhysicsWorld.lock();

    if(!lockedWorld) return {};

    auto rigidbodyA = inRegistry.get<Rigidbody3D>(entityA);
    auto rigidbodyB = inRegistry.get<Rigidbody3D>(entityB);

    auto constraint = constraintCreationFunc(*rigidbodyA, *rigidbodyB);

    lockedWorld->getDynamicsWorld()->addConstraint(constraint.get());

    auto constraintInfo = ConstraintInfo {
        .m_constraint = constraint,
        .m_entityA = entityA,
        .m_entityB = entityB
    };

    m_constraints.push_back(constraintInfo);

    return constraintInfo;
}
