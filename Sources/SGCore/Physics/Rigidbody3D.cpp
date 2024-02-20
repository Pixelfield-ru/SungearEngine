//
// Created by ilya on 19.02.24.
//
#include "Rigidbody3D.h"
#include "PhysicsWorld3D.h"
#include "BulletCollision/btBulletCollisionCommon.h"

SGCore::Rigidbody3D::Rigidbody3D(const SGCore::Ref<SGCore::PhysicsWorld3D>& physicsWorld)
{
    m_parentPhysicsWorld = physicsWorld;
    
    m_state = MakeRef<btDefaultMotionState>(btTransform());
    m_shape = MakeRef<btSphereShape>(0.5f);
    m_shape->setUnscaledRadius(5);
    btRigidBody::btRigidBodyConstructionInfo constructionInfo =
            btRigidBody::btRigidBodyConstructionInfo(25, m_state.get(), m_shape.get(), btVector3(0, 0, 0));
    m_body = MakeRef<btRigidBody>(constructionInfo);
    m_body->setFlags(m_body->getFlags() | btCollisionObject::CF_DYNAMIC_OBJECT);
    
    physicsWorld->getDynamicsWorld()->addRigidBody(m_body.get());
    // m_body->activate(true);
    
    std::cout << "created" << std::endl;
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

SGCore::Weak<SGCore::PhysicsWorld3D> SGCore::Rigidbody3D::getParentPhysicsWorld() const noexcept
{
    return m_parentPhysicsWorld;
}
