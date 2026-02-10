//
// Created by stuka on 10.02.2026.
//

#include "Ragdoll3D.h"

#include "PhysicsWorld3D.h"
#include "SGCore/ECS/Registry.h"
#include "Rigidbody3D.h"

SGCore::Ragdoll3D::Ragdoll3D(const Ref<PhysicsWorld3D>& physicsWorld) noexcept
{
    m_parentPhysicsWorld = physicsWorld;
}

SGCore::ConstraintInfo SGCore::Ragdoll3D::addBone(ECS::entity_t boneEntityA, ECS::entity_t boneEntityB, ECS::registry_t& inRegistry) noexcept
{
    auto lockedWorld = m_parentPhysicsWorld.lock();

    if(!lockedWorld) return {};

    auto rigidbodyA = inRegistry.get<Rigidbody3D>(boneEntityA);
    auto rigidbodyB = inRegistry.get<Rigidbody3D>(boneEntityB);

    auto coneTwist = MakeRef<btConeTwistConstraint>(
        *rigidbodyA->m_body,
        *rigidbodyB->m_body,
        rigidbodyA->getShapeTransform(0),
        rigidbodyB->getShapeTransform(0)
    );

    lockedWorld->getDynamicsWorld()->addConstraint(coneTwist.get());

    auto constraintInfo = ConstraintInfo {
        .m_constraint = coneTwist,
        .m_entityA = boneEntityA,
        .m_entityB = boneEntityB
    };

    m_constraints.push_back(constraintInfo);

    return constraintInfo;
}

const std::vector<SGCore::ConstraintInfo>& SGCore::Ragdoll3D::getConstraints() const noexcept
{
    return m_constraints;
}
