//
// Created by ilya on 19.02.24.
//

#pragma once

#include <BulletCollision/btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <functional>
#include <sgcore_export.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/ECS/Component.h"
#include "Common.h"
#include "SGCore/Serde/Defines.h"
#include "Constraint.h"

sg_predeclare_serde()

namespace SGCore
{
    struct PhysicsWorld3D;

    struct SGCORE_EXPORT Rigidbody3D : ECS::Component<Rigidbody3D, const Rigidbody3D>
    {
        sg_serde_as_friend()

        friend struct PhysicsWorld3D;
        friend struct TransformationsUpdater;

        Ref<btRigidBody> m_body;

        Rigidbody3D(const Ref<PhysicsWorld3D>& physicsWorld);
        Rigidbody3D();
        Rigidbody3D(const Rigidbody3D& other) noexcept = default;
        Rigidbody3D(Rigidbody3D&& other) noexcept = default;
        
        ~Rigidbody3D();
        
        Ref<btMotionState> m_state;

        void stop() const noexcept;

        void reAddToWorld() const noexcept;
        void removeFromWorld() const noexcept;

        ConstraintInfo addPointToPointConstraint(Rigidbody3D& otherRigidbody, const glm::vec3& pointA, const glm::vec3& pointB, ECS::registry_t& inRegistry, bool disableCollisionBetweenBodies) noexcept;
        ConstraintInfo addConeTwistConstraint(Rigidbody3D& otherRigidbody, ECS::registry_t& inRegistry, bool disableCollisionBetweenBodies) noexcept;
        
        [[nodiscard]] Ref<const btCompoundShape> getFinalShape() const noexcept;

        void addShape(const btTransform& shapeTransform, const Ref<btCollisionShape>& shape) noexcept;
        void removeShape(const Ref<btCollisionShape>& shape) noexcept;
        void removeAllShapes() noexcept;
        size_t getShapesCount() const noexcept;
        [[nodiscard]] const std::vector<Ref<btCollisionShape>>& getShapes() const noexcept;

        [[nodiscard]] btTransform& getShapeTransform(size_t index) noexcept;
        [[nodiscard]] const btTransform& getShapeTransform(size_t index) const noexcept;
        void updateShapeTransform(size_t index, const btTransform& newTransform, bool recalculateLocalAABB = true) noexcept;
        void updateShapeTransform(size_t index, bool recalculateLocalAABB = true) noexcept;

        void setParentWorld(const Ref<PhysicsWorld3D>& world) noexcept;
        [[nodiscard]] Weak<PhysicsWorld3D> getParentPhysicsWorld() const noexcept;

        void setType(PhysicalObjectType type) noexcept;
        PhysicalObjectType getType() const noexcept;
        
        Rigidbody3D& operator=(const Rigidbody3D& other) noexcept = default;
        Rigidbody3D& operator=(Rigidbody3D&& other) noexcept = default;
        
    private:
        // contains ALL shapes in ALL COMPOUND SHAPES
        std::vector<Ref<btCollisionShape>> m_shapes;
        Ref<btCompoundShape> m_finalShape;
        Weak<PhysicsWorld3D> m_parentPhysicsWorld;

        std::vector<ConstraintInfo> m_constraints;

        // last rigidbody position
        glm::vec3 m_lastPosition { };
        // last rigidbody rotation
        glm::quat m_lastRotation = glm::identity<glm::quat>();

        PhysicalObjectType m_type = PhysicalObjectType::OT_STATIC;

        ConstraintInfo addBodyToBodyConstraintImpl(Rigidbody3D& otherRigidbody,
                                                   const std::function<Ref<btTypedConstraint>()>&
                                                   constraintCreationFunc,
                                                   ECS::registry_t& inRegistry,
                                                   bool disableCollisionBetweenBodies) noexcept;
    };
}
