//
// Created by ilya on 19.02.24.
//
#include "Rigidbody3D.h"
#include "PhysicsWorld3D.h"
#include "BulletCollision/btBulletCollisionCommon.h"

SGCore::Rigidbody3D::Rigidbody3D(const SGCore::Ref<SGCore::PhysicsWorld3D>& physicsWorld)
{
    m_parentPhysicsWorld = physicsWorld;
    
    btTransform initialTransform;
    initialTransform.setIdentity();
    m_state = MakeRef<btDefaultMotionState>(initialTransform);
    m_shape = MakeRef<btEmptyShape>();
    btRigidBody::btRigidBodyConstructionInfo constructionInfo =
            btRigidBody::btRigidBodyConstructionInfo(250, m_state.get(), m_shape.get(), btVector3(0, 0, 0));
    m_body = MakeRef<btRigidBody>(constructionInfo);
    m_body->setFlags(m_body->getFlags() |  btCollisionObject::CF_STATIC_OBJECT);
    
    physicsWorld->addBody(m_body);
    
    m_bodyFlags.m_flags |= m_body->getFlags();
}

SGCore::Rigidbody3D::~Rigidbody3D()
{
    if(auto lockedWorld = m_parentPhysicsWorld.lock())
    {
        if(m_body.use_count() == 1)
        {
            lockedWorld->removeBody(m_body);
        }
    }
}

void SGCore::Rigidbody3D::setShape(const SGCore::Ref<btCollisionShape>& shape) noexcept
{
    m_shape = shape;
    
    m_body->setCollisionShape(m_shape.get());
}

SGCore::Ref<btCollisionShape> SGCore::Rigidbody3D::getShape() const noexcept
{
    return m_shape;
}

SGCore::Weak<SGCore::PhysicsWorld3D> SGCore::Rigidbody3D::getParentPhysicsWorld() const noexcept
{
    return m_parentPhysicsWorld;
}

void SGCore::Rigidbody3D::updateFlags() noexcept
{
    m_body->setFlags(m_bodyFlags.m_flags);
}

void SGCore::Rigidbody3D::reAddToWorld() const noexcept
{
    if(auto lockedWorld = m_parentPhysicsWorld.lock())
    {
        lockedWorld->removeBody(m_body);
        lockedWorld->addBody(m_body);
    }
}
