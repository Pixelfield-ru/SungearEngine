//
// Created by ilya on 18.02.24.
//

#ifndef SUNGEARENGINE_PHYSICSWORLD3D_H
#define SUNGEARENGINE_PHYSICSWORLD3D_H

#include <SGCore/pch.h>

#include "SGCore/Scene/Scene.h"
#include "SGCore/Main/CoreGlobals.h"
#include "PhysicsDebugDraw.h"
#include "SGCore/Utils/Timer.h"
#include "SGCore/Scene/EntityComponentMember.h"
#include "SGCore/Threading/WrappedObject.h"
#include "SGCore/Scene/IParallelSystem.h"

namespace SGCore
{
    struct Transform;
    
    struct PhysicsWorld3D : public IParallelSystem<PhysicsWorld3D>
    {
        sg_implement_type_id(PhysicsWorld3D, 17)

        friend struct TransformationsUpdater;

        bool m_simulate = true;
        
        PhysicsWorld3D();

        void parallelUpdate(const double& dt, const double& fixedDt) noexcept final;

        void addBody(const Ref<btRigidBody>& rigidBody) noexcept;
        void removeBody(const Ref<btRigidBody>& rigidBody) noexcept;
        
        void update(const double& dt, const double& fixedDt) noexcept override;
        void onAddToScene(const Ref<Scene>& scene) override;
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
        void updateWorld(double dt, double fixedDt) noexcept;

        Scope<btCollisionConfiguration> m_collisionConfig;
        Scope<btCollisionDispatcher> m_collisionDispatcher;
        Scope<btBroadphaseInterface> m_overlappingPairCache;
        Scope<btSequentialImpulseConstraintSolver> m_sequentialImpulseConstraintSolver;
        Scope<btDynamicsWorld> m_dynamicsWorld;
        Scope<PhysicsDebugDraw> m_debugDraw;
        
        std::mutex m_bodiesCountChangeMutex;
    };
}

#endif // SUNGEARENGINE_PHYSICSWORLD3D_H
