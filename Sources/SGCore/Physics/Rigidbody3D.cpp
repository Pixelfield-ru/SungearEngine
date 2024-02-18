//
// Created by ilya on 19.02.24.
//
#include "Rigidbody3D.h"
#include "PhysicsWorld.h"
#include "BulletCollision/btBulletCollisionCommon.h"

SGCore::Rigidbody3D::Rigidbody3D(const SGCore::Ref<SGCore::PhysicsWorld>& physicsWorld)
{
    m_parentPhysicsWorld = physicsWorld;
    
    m_motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo m_constructionInfo =
            btRigidBody::btRigidBodyConstructionInfo(5, m_motionState, nullptr, btVector3(0, 0, 0));
    m_rigidBody = new btRigidBody(m_constructionInfo);
    // m_rigidBody->setMassProps(btScalar(0.),btVector3(btScalar(0.),btScalar(0.),btScalar(0.)));
    
    physicsWorld->getDiscreteDynamicsWorld()->addRigidBody(m_rigidBody);
}

SGCore::Rigidbody3D::~Rigidbody3D()
{
    auto lockedPhysicsWorld = m_parentPhysicsWorld.lock();
    
    if(m_rigidBody)
    {
        if(lockedPhysicsWorld)
        {
            /*int num = m_rigidBody->getNumConstraintRefs();
            for(int j = 0;j < num; ++j)
            {
                lockedPhysicsWorld->getDiscreteDynamicsWorld()->removeConstraint(m_rigidBody->getConstraintRef(0));
            }*/
            lockedPhysicsWorld->getDiscreteDynamicsWorld()->removeRigidBody(m_rigidBody);
        }
        
        
        // delete m_rigidBody->getMotionState();
        // delete m_rigidBody->getCollisionShape();
        //
        // delete m_rigidBody;
    }
}

