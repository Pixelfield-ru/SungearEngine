//
// Created by stuka on 03.07.2023.
//

#include "ECSWorld.h"

#include "Transformations/TransformationsSystem.h"
#include "SGCore/ECS/Transformations/Camera3DMovementSystem.h"
#include "SGCore/ECS/Rendering/CameraRenderingSystem.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterSystem.h"
#include "SGCore/ECS/Rendering/RenderingComponentsSystem.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsSystem.h"
#include "SGCore/ECS/Rendering/SkyboxRenderingSystem.h"
#include "SGCore/ECS/Rendering/Primitives/PrimitivesUpdaterSystem.h"

void Core::ECS::ECSWorld::init() noexcept
{
    createSystem<TransformationsSystem>();
    createSystem<RenderingComponentsSystem>();
    createSystem<SkyboxRenderingSystem>();
    createSystem<PrimitivesUpdaterSystem>();
    // directional light system must be always before shadows caster system
    createSystem<DirectionalLightsSystem>()->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);
    createSystem<ShadowsCasterSystem>()->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);
    createSystem<Camera3DMovementSystem>();
    createSystem<CameraRenderingSystem>();
}

void Core::ECS::ECSWorld::FPSNotRelativeFixedUpdate(const std::shared_ptr<Scene>& scene)
{
    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        if(system->isFlagSet(SystemsFlags::SGSF_NOT_PER_ENTITY))
        {
            system->FPSNotRelativeFixedUpdate(scene);
        }

        if(system->isFlagSet(SystemsFlags::SGSF_PER_ENTITY))
        {
            for(auto& entity: scene->m_entities)
            {
                system->FPSNotRelativeFixedUpdate(scene, entity);
            }
        }
    }
}

void Core::ECS::ECSWorld::FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene)
{
    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        if(system->isFlagSet(SystemsFlags::SGSF_NOT_PER_ENTITY))
        {
            system->FPSRelativeFixedUpdate(scene);
        }

        if(system->isFlagSet(SystemsFlags::SGSF_PER_ENTITY))
        {
            for(auto& entity: scene->m_entities)
            {
                system->FPSRelativeFixedUpdate(scene, entity);
            }
        }
    }
}
