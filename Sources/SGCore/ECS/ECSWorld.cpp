//
// Created by stuka on 03.07.2023.
//

#include "ECSWorld.h"

#include "Transformations/TransformationsUpdater.h"
#include "SGCore/ECS/Transformations/CameraMovement3DSystem.h"
#include "SGCore/ECS/Rendering/Pipelines/PBRForwardRenderPipeline.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCastersCollector.h"
#include "SGCore/ECS/Rendering/RenderingComponentsUpdater.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsCollector.h"
#include "SGCore/ECS/Rendering/SkyboxesCollector.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/GizmosMeshesRebuilder.h"
#include "SGCore/ECS/Rendering/Gizmos/LinesGizmosCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/ComplexGizmosCollector.h"

void Core::ECS::ECSWorld::init() noexcept
{
    auto transformationsSystem = Patterns::Singleton::getInstance<TransformationsUpdater>();

    auto meshedEntitiesCollectorSystem = Patterns::Singleton::getInstance<MeshesCollector>();

    auto renderingComponentsSystem = Patterns::Singleton::getInstance<RenderingComponentsUpdater>();

    auto primitivesUpdaterSystem = Patterns::Singleton::getInstance<GizmosMeshesRebuilder>();

    auto directionalLightsSystem = Patterns::Singleton::getInstance<DirectionalLightsCollector>();

    auto shadowsCasterSystem = Patterns::Singleton::getInstance<ShadowsCastersCollector>();

    auto camera3DMovementSystem = Patterns::Singleton::getInstance<CameraMovement3DSystem>();

    auto pipelineSystem = Patterns::Singleton::getInstance<PBRForwardRenderPipeline>();

    auto skyboxesCollectorSystem = Patterns::Singleton::getInstance<SkyboxesCollector>();

    auto linesCollectorSystem = Patterns::Singleton::getInstance<LinesGizmosCollector>();

    auto complexPrimitivesCollectorSystem = Patterns::Singleton::getInstance<ComplexGizmosCollector>();

    // -------------------------------
    m_systems.emplace(transformationsSystem);
    m_systems.emplace(meshedEntitiesCollectorSystem);
    m_systems.emplace(renderingComponentsSystem);
    m_systems.emplace(primitivesUpdaterSystem);
    // directional light system must be always before shadows caster system
    m_systems.emplace(directionalLightsSystem);
    m_systems.emplace(shadowsCasterSystem);
    m_systems.emplace(camera3DMovementSystem);
    m_systems.emplace(pipelineSystem);
    m_systems.emplace(skyboxesCollectorSystem);

    m_systems.emplace(linesCollectorSystem);
    m_systems.emplace(complexPrimitivesCollectorSystem);

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

        double before = glfwGetTime();
        system->fixedUpdate(scene);
        double after = glfwGetTime();

        system->m_fixedUpdate_executionTime = (after - before) * 1000.0;
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

        double before = glfwGetTime();
        system->update(scene);
        double after = glfwGetTime();

        system->m_update_executionTime = (after - before) * 1000.0;
    }

    double t1 = glfwGetTime();

    // 20,761000 ms average !!!!!!!!!!!!!!

    //std::cout << "ms for Core::ECS::ECSWorld::FPSRelativeFixedUpdate: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::ECSWorld::recacheEntity(const std::shared_ptr<Entity>& entity)
{
    double t0 = glfwGetTime();

    auto entityLayer = entity->getLayer();
    if(!entityLayer) return;

    for(auto& system : m_systems)
    {
        if (!system->m_active) continue;

        // clearing all cached components for this entity
        const auto& foundLayerIter = system->m_cachedEntities.find(entityLayer);
        if(foundLayerIter != system->m_cachedEntities.cend())
        {
            const auto& foundEntityIter = foundLayerIter->second.find(entity);
            if(foundEntityIter != foundLayerIter->second.cend())
            {
                if(foundEntityIter->second)
                {
                    foundEntityIter->second->clear();
                }
            }
        }

        system->cacheEntity(entity);
    }

    double t1 = glfwGetTime();

    // std::cout << "ms for recache: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

std::set<std::shared_ptr<Core::ECS::ISystem>>& Core::ECS::ECSWorld::getSystems() noexcept
{
    return m_systems;
}
