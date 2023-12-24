//
// Created by stuka on 04.07.2023.
//

#include "Scene.h"

#include "SGCore/Graphics/Defines.h"
#include "Layer.h"
#include "SGCore/ECS/Transformations/TransformationsUpdater.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "SGCore/ECS/Rendering/RenderingComponentsUpdater.h"
#include "SGCore/ECS/Rendering/Gizmos/GizmosMeshesRebuilder.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsCollector.h"
#include "SGCore/ECS/Transformations/CameraMovement3DSystem.h"
#include "SGCore/ECS/Rendering/SkyboxesCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/LinesGizmosCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/ComplexGizmosCollector.h"
#include "SGCore/ECS/Rendering/CamerasCollector.h"
#include "SGCore/ECS/Rendering/Pipelines/PBRFRP/PBRForwardRenderPipeline.h"
#include "SGCore/Logging/Log.h"

SGCore::Scene::Scene() noexcept
{
    auto transparentLayer = MakeRef<Layer>();
    transparentLayer->m_name = SG_LAYER_TRANSPARENT_NAME;
    transparentLayer->m_index = 0;
    transparentLayer->m_isOpaque  = false;

    m_layers[SG_LAYER_TRANSPARENT_NAME] = std::move(transparentLayer);

    auto opaqueLayer = MakeRef<Layer>();
    opaqueLayer->m_name = SG_LAYER_OPAQUE_NAME;
    opaqueLayer->m_index = 1;

    m_layers[SG_LAYER_OPAQUE_NAME] = std::move(opaqueLayer);
}

void SGCore::Scene::createDefaultSystems()
{
    auto transformationsSystem = std::make_shared<TransformationsUpdater>();

    auto meshedEntitiesCollectorSystem = std::make_shared<MeshesCollector>();

    auto renderingComponentsSystem = std::make_shared<RenderingComponentsUpdater>();

    auto primitivesUpdaterSystem = std::make_shared<GizmosMeshesRebuilder>();

    auto directionalLightsSystem = std::make_shared<DirectionalLightsCollector>();

    auto camera3DMovementSystem = std::make_shared<CameraMovement3DSystem>();

    auto skyboxesCollectorSystem = std::make_shared<SkyboxesCollector>();

    auto linesCollectorSystem = std::make_shared<LinesGizmosCollector>();

    auto complexPrimitivesCollectorSystem = std::make_shared<ComplexGizmosCollector>();

    auto camerasCollectorSystem = std::make_shared<CamerasCollector>();

    auto pipelineSystem = std::make_shared<PBRForwardRenderPipeline>();

    // -------------------------------
    m_systems.emplace(transformationsSystem);
    m_systems.emplace(meshedEntitiesCollectorSystem);
    m_systems.emplace(renderingComponentsSystem);
    m_systems.emplace(primitivesUpdaterSystem);
    // directional light system must be always before shadows caster system
    m_systems.emplace(directionalLightsSystem);
    m_systems.emplace(camera3DMovementSystem);
    m_systems.emplace(skyboxesCollectorSystem);

    m_systems.emplace(linesCollectorSystem);
    m_systems.emplace(complexPrimitivesCollectorSystem);

    m_systems.emplace(camerasCollectorSystem);

    m_systems.emplace(pipelineSystem);

    for(auto& system : m_systems)
    {
        system->useScene(shared_from_this());
    }
}

void SGCore::Scene::addLayer(std::string&& layerName) noexcept
{
    auto newLayer = MakeRef<Layer>();
    newLayer->m_name = std::move(layerName);

    m_layers[newLayer->m_name] = std::move(newLayer);
}

void SGCore::Scene::addEntity(const Ref<Entity>& entity) noexcept
{
    entity->m_layer = m_layers[SG_LAYER_OPAQUE_NAME];
    m_entities.push_back(entity);

    entity->setParentScene(shared_from_this());

    recacheEntity(entity);
}

void SGCore::Scene::addEntity(const Ref<Entity>& entity, const std::string& layerName) noexcept
{
    entity->m_layer = m_layers[layerName];
    m_entities.push_back(entity);

    entity->setParentScene(shared_from_this());

    recacheEntity(entity);
}

void SGCore::Scene::addEntity(const Ref<Entity>& entity, const Ref<Layer>& layer) noexcept
{
    entity->m_layer = layer;
    m_entities.push_back(entity);

    entity->setParentScene(shared_from_this());

    recacheEntity(entity);
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

SGCore::Ref<SGCore::Layer> SGCore::Scene::getLayer(const size_t& layerIndex) noexcept
{
    Ref<Layer> foundLayer;
    for(const auto& layer : m_layers)
    {
        if(layer.second->m_index == layerIndex)
        {
            foundLayer = layer.second;
        }
    }

    return foundLayer;
    //return m_layers.
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
}

void SGCore::Scene::recacheEntity(const SGCore::Ref<SGCore::Entity>& entity)
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
}

std::set<SGCore::Ref<SGCore::ISystem>>& SGCore::Scene::getSystems() noexcept
{
    return m_systems;
}
