//
// Created by ilya on 18.02.24.
//

#pragma once

#include <stack>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

#include "SGCore/Scene/Scene.h"
#include "SGCore/Main/CoreGlobals.h"
#include "PhysicsDebugDraw.h"
#include "SGCore/Scene/IParallelSystem.h"
#include "SGCore/Transformations/Transform.h"

namespace SGCore
{
    struct Transform;
    
    struct SGCORE_EXPORT PhysicsWorld3D : public IParallelSystem<PhysicsWorld3D>
    {
        sg_implement_type_id(SGCore::PhysicsWorld3D)

        friend struct TransformationsUpdater;

        bool m_simulate = true;
        
        PhysicsWorld3D();

        void parallelUpdate(const double& dt, const double& fixedDt) noexcept final;

        void addBody(const Ref<btRigidBody>& rigidBody) noexcept;
        void removeBody(const Ref<btRigidBody>& rigidBody) noexcept;
        
        void update(double dt, double fixedDt) noexcept override;
        void onAddToScene(const Scene* scene) override;
        void onRemoveFromScene(const Ref<Scene>& scene) override;
        
        auto& getCollisionConfig() noexcept
        {
            return m_collisionConfig;
        }
        
        auto& getCollisionDispatcher() noexcept
        {
            return m_collisionDispatcher;
        }
        
        auto& getOverlappingPairCache() noexcept
        {
            return m_overlappingPairCache;
        }
        
        auto& getSequentialImpulseConstraintSolver() noexcept
        {
            return m_sequentialImpulseConstraintSolver;
        }
        
        auto& getDynamicsWorld() noexcept
        {
            return m_dynamicsWorld;
        }
        
        auto& getDebugDraw() noexcept
        {
            return m_debugDraw;
        }
    
    private:
        struct EntityDesc
        {
            ECS::entity_t m_entity = entt::null;
            const EntityBaseInfo* m_baseInfo {};
            const Ref<Transform>* m_transform {};
            Transform* m_parentTransform {};
        };

        std::stack<EntityDesc> m_postPhysicsEntitiesDesc;

        Scope<btCollisionConfiguration> m_collisionConfig;
        Scope<btCollisionDispatcher> m_collisionDispatcher;
        Scope<btBroadphaseInterface> m_overlappingPairCache;
        Scope<btSequentialImpulseConstraintSolver> m_sequentialImpulseConstraintSolver;
        Scope<btDynamicsWorld> m_dynamicsWorld;
        Scope<PhysicsDebugDraw> m_debugDraw;

        std::mutex m_bodiesCountChangeMutex;

        void updateWorld(double dt, double fixedDt) noexcept;

        void calculatePostPhysicsEntitiesTransforms(const Ref<ECS::registry_t>& inRegistry) noexcept;
    };
}
