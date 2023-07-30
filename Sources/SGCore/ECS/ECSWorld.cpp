//
// Created by stuka on 03.07.2023.
//

#include "ECSWorld.h"

#include "Transformations/TransformationsSystem.h"
#include "SGCore/ECS/Rendering/Camera3DMovementSystem.h"
#include "SGCore/ECS/Rendering/CameraRenderingSystem.h"
#include "SGCore/ECS/Rendering/ShadowsCasterSystem.h"

void Core::ECS::ECSWorld::init() noexcept
{
    m_systems.push_back(std::make_shared<TransformationsSystem>());
    m_systems.push_back(std::make_shared<Camera3DMovementSystem>());
    m_systems.push_back(std::make_shared<CameraRenderingSystem>());
    m_systems.push_back(std::make_shared<ShadowsCasterSystem>());
}

void Core::ECS::ECSWorld::update(const std::shared_ptr<Scene>& scene)
{
    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        for(auto& entity: scene->m_entities)
        {
            system->update(scene, entity);
        }
    }
}

void Core::ECS::ECSWorld::deltaUpdate(const std::shared_ptr<Scene>& scene, const double& deltaTime)
{
    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        for(auto& entity: scene->m_entities)
        {
            system->deltaUpdate(scene, entity, deltaTime);
        }
    }
}
