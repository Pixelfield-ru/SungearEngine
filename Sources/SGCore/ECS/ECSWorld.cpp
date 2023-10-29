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
    auto transformationsSystem = Patterns::Singleton::getInstance<TransformationsSystem>();
    transformationsSystem->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);

    auto meshedEntitiesCollectorSystem = Patterns::Singleton::getInstance<MeshedEntitiesCollectorSystem>();

    auto renderingComponentsSystem = Patterns::Singleton::getInstance<RenderingComponentsSystem>();
    renderingComponentsSystem->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);

    auto primitivesUpdaterSystem = Patterns::Singleton::getInstance<PrimitivesUpdaterSystem>();
    primitivesUpdaterSystem->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);

    auto directionalLightsSystem = Patterns::Singleton::getInstance<DirectionalLightsSystem>();
    directionalLightsSystem->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);

    auto shadowsCasterSystem = Patterns::Singleton::getInstance<ShadowsCasterSystem>();
    shadowsCasterSystem->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);

    auto camera3DMovementSystem = Patterns::Singleton::getInstance<Camera3DMovementSystem>();
    camera3DMovementSystem->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);

    auto cameraRenderingSystem = Patterns::Singleton::getInstance<CameraRenderingSystem>();
    cameraRenderingSystem->addFlag(SystemsFlags::SGSF_NOT_PER_ENTITY);

    // -------------------------------
    m_systems.emplace(transformationsSystem);
    m_systems.emplace(meshedEntitiesCollectorSystem);
    m_systems.emplace(renderingComponentsSystem);
    m_systems.emplace(primitivesUpdaterSystem);
    // directional light system must be always before shadows caster system
    m_systems.emplace(directionalLightsSystem);
    m_systems.emplace(shadowsCasterSystem);
    m_systems.emplace(camera3DMovementSystem);
    m_systems.emplace(cameraRenderingSystem);

    //DirectionalLightsSystem f;

    //m_systems.push_back(std::make_shared<TransformationsSystem>());
}

void Core::ECS::ECSWorld::fixedUpdate(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    // const auto& f = m_cachedComponentsCollections;

    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        auto updateFuncsQueryIter = system->m_fixedUpdateFunctionsQuery.begin();
        while(updateFuncsQueryIter != system->m_fixedUpdateFunctionsQuery.end())
        {
            bool funcDone = updateFuncsQueryIter->second();

            if(funcDone)
            {
                system->m_fixedUpdateFunctionsQuery.erase(updateFuncsQueryIter++);
            }
            else
            {
                updateFuncsQueryIter++;
            }
        }

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

        // todo:: fix infinite adding to query
        auto updateFuncsQueryIter = system->m_updateFunctionsQuery.begin();
        while(updateFuncsQueryIter != system->m_updateFunctionsQuery.end())
        {
            bool funcDone = updateFuncsQueryIter->second();

            if(funcDone)
            {
                system->m_updateFunctionsQuery.erase(updateFuncsQueryIter++);
            }
            else
            {
                updateFuncsQueryIter++;
            }
        }

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

    for(auto& system : m_systems)
    {
        if (!system->m_active) continue;

        // clearing all cached components for this entity
        auto& foundComponentsCollection = system->m_cachedEntities[entity];
        if(foundComponentsCollection)
        {
            foundComponentsCollection->clear();
        }

        system->cacheEntity(entity);
    }

    double t1 = glfwGetTime();

    // std::cout << "ms for recache: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}
