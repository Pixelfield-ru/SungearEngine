//
// Created by stuka on 04.07.2023.
//

#include "Scene.h"

#include "SGCore/ECS/Transformations/TransformationsUpdater.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "SGCore/ECS/Rendering/RenderingComponentsUpdater.h"
#include "SGCore/ECS/Rendering/Gizmos/GizmosMeshesRebuilder.h"
#include "SGCore/ECS/Transformations/CameraMovement3DSystem.h"
#include "SGCore/ECS/Rendering/Pipelines/PBRFRP/PBRForwardRenderPipeline.h"
#include "SGCore/ECS/Rendering/CamerasCollector.h"
#include "SGCore/ECS/Rendering/Pipelines/PBRFRP/PBRFRPDirectionalLightsCollector.h"
#include "GLFW/glfw3.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "BaseEntityInfo.h"
#include "EntityBaseInfo.h"

SGCore::Scene::Scene()
{
   createLayer(SG_LAYER_OPAQUE_NAME);
}

void SGCore::Scene::createDefaultSystems()
{
    auto transformationsSystem = std::make_shared<TransformationsUpdater>();

    auto meshedEntitiesCollectorSystem = std::make_shared<MeshesCollector>();

    auto renderingComponentsSystem = std::make_shared<RenderingComponentsUpdater>();

    auto primitivesUpdaterSystem = std::make_shared<GizmosMeshesRebuilder>();

    auto camera3DMovementSystem = std::make_shared<CameraMovement3DSystem>();

    auto pbrfrpCamerasCollector = std::make_shared<CamerasCollector>();
    auto pbrfrpDirectionalLightsCollector = std::make_shared<PBRFRPDirectionalLightsCollector>();

    setRenderPipeline(MakeRef<PBRForwardRenderPipeline>());

    // -------------------------------
    m_systems.emplace(transformationsSystem);
    m_systems.emplace(meshedEntitiesCollectorSystem);
    m_systems.emplace(renderingComponentsSystem);
    m_systems.emplace(primitivesUpdaterSystem);
    // directional light system must be always before shadows caster system
    m_systems.emplace(camera3DMovementSystem);

    m_systems.emplace(pbrfrpCamerasCollector);
    m_systems.emplace(pbrfrpDirectionalLightsCollector);


    // m_systems.emplace(pipelineSystem);

    for(auto& system : m_systems)
    {
        system->useScene(shared_from_this());
    }
}

// ----------------

SGCore::Ref<SGCore::Scene> SGCore::Scene::getCurrentScene() noexcept
{
    return m_currentScene;
}

void SGCore::Scene::setCurrentScene(const Ref<Scene>& newCurrentScene) noexcept
{
    m_currentScene = newCurrentScene;
}

// ==================================================================

void SGCore::Scene::fixedUpdate()
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
        system->fixedUpdate(shared_from_this());
        double after = glfwGetTime();

        system->m_fixedUpdate_executionTime = (after - before) * 1000.0;
    }

    double t1 = glfwGetTime();
}

void SGCore::Scene::update()
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
        system->update(shared_from_this());
        double after = glfwGetTime();

        system->m_update_executionTime = (after - before) * 1000.0;
    }

    if(RenderPipelinesManager::getRenderPipeline())
    {
        RenderPipelinesManager::getRenderPipeline()->render(shared_from_this());
    }
}

std::set<SGCore::Ref<SGCore::ISystem>>& SGCore::Scene::getSystems() noexcept
{
    return m_systems;
}

size_t SGCore::Scene::createBaseEntity() noexcept
{
    size_t entity = m_ecsRegistry->createEntity();
    auto* baseEntityInfo = m_ecsRegistry->emplace<EntityBaseInfo>(entity);

    return entity;
}

SGCore::Layer SGCore::Scene::createLayer(const std::string& name) noexcept
{
    Layer layer = m_layers[name];
    layer.m_index = m_maxLayersCount++;
    return layer;
}
