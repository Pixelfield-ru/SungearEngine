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
#include "SGCore/ECS/Rendering/MeshedEntitiesCollectorSystem.h"
#include "GLFW/glfw3.h"

void Core::ECS::ECSWorld::init() noexcept
{
    createSystem<TransformationsSystem>()
            ->removeFlag(SystemsFlags::SGSF_PER_ENTITY)
            ->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);
    createSystem<MeshedEntitiesCollectorSystem>()
            ->removeFlag(SystemsFlags::SGSF_PER_ENTITY);
    createSystem<RenderingComponentsSystem>()
            ->removeFlag(SystemsFlags::SGSF_PER_ENTITY)
            ->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);
    //createSystem<SkyboxRenderingSystem>();
    createSystem<PrimitivesUpdaterSystem>();
    // directional light system must be always before shadows caster system
    createSystem<DirectionalLightsSystem>()
            ->removeFlag(SystemsFlags::SGSF_PER_ENTITY)
            ->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);
    createSystem<ShadowsCasterSystem>()
            ->removeFlag(SystemsFlags::SGSF_PER_ENTITY)
            ->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);

    createSystem<Camera3DMovementSystem>();
    createSystem<CameraRenderingSystem>()
            ->removeFlag(SystemsFlags::SGSF_PER_ENTITY)
            ->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);

    //DirectionalLightsSystem f;

    //m_systems.push_back(std::make_shared<TransformationsSystem>());
}

void Core::ECS::ECSWorld::fixedUpdate(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        if(system->isFlagSet(SystemsFlags::SGSF_NOT_PER_ENTITY))
        {
            system->fixedUpdate(scene);
        }

        if(system->isFlagSet(SystemsFlags::SGSF_PER_ENTITY))
        {
            for(const auto& layer : scene->getLayers())
            {
                for(auto& entity : layer.second->m_entities)
                {
                    system->fixedUpdate(scene, entity);
                }
            }
        }
    }

    double t1 = glfwGetTime();

    // 0.025100 ms average

    // std::cout << "ms: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::ECSWorld::update(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        if(system->isFlagSet(SystemsFlags::SGSF_NOT_PER_ENTITY))
        {
            system->update(scene);
        }

        if(system->isFlagSet(SystemsFlags::SGSF_PER_ENTITY))
        {
            for(const auto& layer : scene->getLayers())
            {
                for(auto& entity : layer.second->m_entities)
                {
                    system->update(scene, entity);
                }
            }
        }
    }

    double t1 = glfwGetTime();

    // 20,761000 ms average !!!!!!!!!!!!!!

    //std::cout << "ms for Core::ECS::ECSWorld::FPSRelativeFixedUpdate: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::ECSWorld::recacheEntity(const std::shared_ptr<Entity>& entity)
{
    double t0 = glfwGetTime();

    // clearing all cached components for this entity
    for(const auto& systemCachedEntities : m_cachedComponentsCollections)
    {
        if(systemCachedEntities.second != nullptr)
        {
            auto& foundComponentsCollection = systemCachedEntities.second->m_cachedEntities[entity];
            if(foundComponentsCollection != nullptr)
            {
                foundComponentsCollection->clear();
            }
        }
    }

    for(auto& system : m_systems)
    {
        if (!system->m_active) continue;

        system->cacheEntity(entity);
    }

    double t1 = glfwGetTime();

    // std::cout << "ms for recache: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}
