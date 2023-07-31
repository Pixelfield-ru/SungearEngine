//
// Created by stuka on 03.07.2023.
//

#include "ECSWorld.h"

#include "Transformations/TransformationsSystem.h"
#include "SGCore/ECS/Rendering/Camera3DMovementSystem.h"
#include "SGCore/ECS/Rendering/CameraRenderingSystem.h"
#include "SGCore/ECS/Rendering/ShadowsCasterSystem.h"
#include "SGCore/ECS/Rendering/RenderingComponentsSystem.h"

void Core::ECS::ECSWorld::init() noexcept
{
    createSystem<TransformationsSystem>();
    createSystem<RenderingComponentsSystem>();
    createSystem<ShadowsCasterSystem>()->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);
    createSystem<Camera3DMovementSystem>();
    createSystem<CameraRenderingSystem>();
}

void Core::ECS::ECSWorld::update(const std::shared_ptr<Scene>& scene)
{
    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        if(system->isFlagSet(SystemsFlags::SGSF_NOT_PER_ENTITY))
        {
            system->update(scene);
        }

        if(system->isFlagSet(SystemsFlags::SGSF_PER_ENTITY))
        {
            for(auto& entity: scene->m_entities)
            {
                system->update(scene, entity);
            }
        }
    }
}

void Core::ECS::ECSWorld::deltaUpdate(const std::shared_ptr<Scene>& scene, const double& deltaTime)
{
    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        if(system->isFlagSet(SystemsFlags::SGSF_NOT_PER_ENTITY))
        {
            system->deltaUpdate(scene, deltaTime);
        }

        if(system->isFlagSet(SystemsFlags::SGSF_PER_ENTITY))
        {
            for(auto& entity: scene->m_entities)
            {
                system->deltaUpdate(scene, entity, deltaTime);
            }
        }
    }
}
