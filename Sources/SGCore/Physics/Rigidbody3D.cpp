//
// Created by ilya on 19.02.24.
//
#include "Rigidbody3D.h"
#include "PhysicsWorld3D.h"
#include "BulletCollision/CollisionShapes/btEmptyShape.h"

SGCore::Rigidbody3D::Rigidbody3D(const SGCore::Ref<PhysicsWorld3D>& parentWorld)
{
    m_parentPhysicsWorld = parentWorld;
    
    btTransform initialTransform;
    initialTransform.setIdentity();
    m_state = MakeScope<btDefaultMotionState>(initialTransform);
    m_shape = MakeScope<btEmptyShape>();
    btRigidBody::btRigidBodyConstructionInfo constructionInfo =
            btRigidBody::btRigidBodyConstructionInfo(250, m_state.get(), m_shape.get(), btVector3(0, 0, 0));
    m_body = MakeScope<btRigidBody>(constructionInfo);
    m_body->setFlags(m_body->getFlags() |  btCollisionObject::CF_STATIC_OBJECT);
    
    m_bodyFlags.m_flags |= m_body->getFlags();
    
    parentWorld->addBody(m_body);
}

SGCore::Rigidbody3D::~Rigidbody3D()
{
    /*if(m_state.use_count() == 1 && m_state)
    {
        delete m_state.get();
        m_state = nullptr;
    }
    
    if(m_shape.use_count() == 1 && m_shape)
    {
        delete m_shape.get();
        m_shape = nullptr;
    }*/
    
    if(auto lockedWorld = m_parentPhysicsWorld.lock())
    {
        if(m_body.use_count() == 1 && m_body)
        {
            lockedWorld->removeBody(m_body);
            
            /*delete m_body.get();
            m_body = nullptr;*/
        }
    }
}

void SGCore::Rigidbody3D::setShape(const SGCore::Ref<btCollisionShape>& shape) noexcept
{
    m_shape = shape;
    
    m_body->setCollisionShape(m_shape.get());
}

SGCore::Ref<btCollisionShape>& SGCore::Rigidbody3D::getShape() noexcept
{
    return m_shape;
}

void SGCore::Rigidbody3D::setParentWorld(const SGCore::Ref<SGCore::PhysicsWorld3D>& world) noexcept
{
    auto lockedWorld = m_parentPhysicsWorld.lock();
    if(lockedWorld)
    {
        lockedWorld->removeBody(m_body);
    }
    
    m_parentPhysicsWorld = world;
    
    if(world)
    {
        world->addBody(m_body);
    }
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

void SGCore::Rigidbody3D::removeFromWorld() const noexcept
{
    auto lockedWorld = m_parentPhysicsWorld.lock();
    if(lockedWorld && m_body)
    {
        lockedWorld->removeBody(m_body);
    }
}
