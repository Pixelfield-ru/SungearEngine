//
// Created by stuka on 03.07.2023.
//

#include "ECSWorld.h"

#include "Transformations/TransformationsUpdater.h"
#include "SGCore/ECS/Transformations/CameraMovement3DSystem.h"
#include "SGCore/ECS/Rendering/Pipelines/PBRFRP/PBRForwardRenderPipeline.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCastersCollector.h"
#include "SGCore/ECS/Rendering/RenderingComponentsUpdater.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsCollector.h"
#include "SGCore/ECS/Rendering/SkyboxesCollector.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/GizmosMeshesRebuilder.h"
#include "SGCore/ECS/Rendering/Gizmos/LinesGizmosCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/ComplexGizmosCollector.h"
#include "SGCore/ECS/Rendering/CamerasCollector.h"

void SGCore::ECSWorld::init() noexcept
{
    auto transformationsSystem = SGSingleton::getSharedPtrInstance<TransformationsUpdater>();

    auto meshedEntitiesCollectorSystem = SGSingleton::getSharedPtrInstance<MeshesCollector>();

    auto renderingComponentsSystem = SGSingleton::getSharedPtrInstance<RenderingComponentsUpdater>();

    auto primitivesUpdaterSystem = SGSingleton::getSharedPtrInstance<GizmosMeshesRebuilder>();

    auto directionalLightsSystem = SGSingleton::getSharedPtrInstance<DirectionalLightsCollector>();

    auto shadowsCasterSystem = SGSingleton::getSharedPtrInstance<ShadowsCastersCollector>();

    auto camera3DMovementSystem = SGSingleton::getSharedPtrInstance<CameraMovement3DSystem>();

    auto skyboxesCollectorSystem = SGSingleton::getSharedPtrInstance<SkyboxesCollector>();

    auto linesCollectorSystem = SGSingleton::getSharedPtrInstance<LinesGizmosCollector>();

    auto complexPrimitivesCollectorSystem = SGSingleton::getSharedPtrInstance<ComplexGizmosCollector>();

    auto camerasCollectorSystem = SGSingleton::getSharedPtrInstance<CamerasCollector>();

    auto pipelineSystem = SGSingleton::getSharedPtrInstance<PBRForwardRenderPipeline>();

    // -------------------------------
    m_systems.emplace(transformationsSystem);
    m_systems.emplace(meshedEntitiesCollectorSystem);
    m_systems.emplace(renderingComponentsSystem);
    m_systems.emplace(primitivesUpdaterSystem);
    // directional light system must be always before shadows caster system
    m_systems.emplace(directionalLightsSystem);
    m_systems.emplace(shadowsCasterSystem);
    m_systems.emplace(camera3DMovementSystem);
    m_systems.emplace(skyboxesCollectorSystem);

    m_systems.emplace(linesCollectorSystem);
    m_systems.emplace(complexPrimitivesCollectorSystem);

    m_systems.emplace(camerasCollectorSystem);

    m_systems.emplace(pipelineSystem);

    //DirectionalLightsSystem f;

    //m_systems.push_back(std::make_shared<TransformationsSystem>());
}

void SGCore::ECSWorld::fixedUpdate(const Ref<Scene>& scene)
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

void SGCore::ECSWorld::update(const Ref<Scene>& scene)
{
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

    // 20,761000 ms average !!!!!!!!!!!!!!

    //std::cout << "ms for SGCore::ECSWorld::FPSRelativeFixedUpdate: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void SGCore::ECSWorld::recacheEntity(const Ref<Entity>& entity)
{
    double t0 = glfwGetTime();

    auto entityLayer = entity->getLayer();
    if(!entityLayer) return;

    for(auto& system : m_systems)
    {
        if (!system->m_active) continue;

        auto& componentsCollector = system->m_componentsCollector;

        // clearing all cached components for this entity
        const auto& foundLayerIter = componentsCollector.m_cachedEntities->find(entityLayer);
        if(foundLayerIter != componentsCollector.m_cachedEntities->cend())
        {
            const auto& foundEntityIter = foundLayerIter->second.find(entity);
            if(foundEntityIter != foundLayerIter->second.cend())
            {
                foundEntityIter->second.clear();
            }
        }

        componentsCollector.cacheEntity(entity);
    }

    double t1 = glfwGetTime();

    // std::cout << "ms for recache: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

std::set<SGCore::Ref<SGCore::ISystem>>& SGCore::ECSWorld::getSystems() noexcept
{
    return m_systems;
}
