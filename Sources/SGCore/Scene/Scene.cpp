//
// Created by stuka on 04.07.2023.
//

#include "Scene.h"

#include "GLFW/glfw3.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "EntityBaseInfo.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "SGCore/Transformations/Controllables3DUpdater.h"
#include "SGCore/Render/RenderingBasesUpdater.h"
#include "SGCore/Render/Gizmos/BoxGizmosUpdater.h"
#include "SGCore/Render/Gizmos/LineGizmosUpdater.h"
#include "SGCore/Render/Gizmos/SphereGizmosUpdater.h"
#include "SGCore/Render/PBRRP/PBRRenderPipeline.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Atmosphere/AtmosphereUpdater.h"
#include "SGCore/Render/Lighting/DirectionalLightsUpdater.h"

SGCore::Scene::Scene()
{
   createLayer(SG_LAYER_OPAQUE_NAME);
}

void SGCore::Scene::createDefaultSystems()
{
    // -------------
    // rendering
    
    auto thisShared = shared_from_this();
    
    auto renderingBasesUpdater = MakeRef<RenderingBasesUpdater>();
    m_systems.emplace(renderingBasesUpdater);
    
    auto atmosphereScatteringUpdater = MakeRef<AtmosphereUpdater>();
    m_systems.emplace(atmosphereScatteringUpdater);
    
    auto directionalLightsUpdater = MakeRef<DirectionalLightsUpdater>();
    m_systems.emplace(directionalLightsUpdater);
    
    // -------------
    
    // transformations

    auto transformationsUpdater = MakeRef<TransformationsUpdater>();
    m_systems.emplace(transformationsUpdater);

    auto controllables3DUpdater = MakeRef<Controllables3DUpdater>();
    m_systems.emplace(controllables3DUpdater);
    
    // gizmos

    auto boxGizmosUpdater = MakeRef<BoxGizmosUpdater>();
    m_systems.emplace(boxGizmosUpdater);

    auto lineGizmosUpdater = MakeRef<LineGizmosUpdater>();
    m_systems.emplace(lineGizmosUpdater);

    auto sphereGizmosUpdater = MakeRef<SphereGizmosUpdater>();
    m_systems.emplace(sphereGizmosUpdater);
    

    // -------------
    
    for(auto& system : m_systems)
    {
        system->setScene(thisShared);
    }

    // m_systems.emplace(pipelineSystem);
}

// ----------------

SGCore::Ref<SGCore::Scene> SGCore::Scene::getCurrentScene() noexcept
{
    return m_currentScene;
}

// ==================================================================

void SGCore::Scene::fixedUpdate()
{
    double t0 = glfwGetTime();

    // const auto& f = m_cachedComponentsCollections;

    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        double before = glfwGetTime();
        system->fixedUpdate();
        double after = glfwGetTime();

        system->m_fixedUpdate_executionTime = (after - before) * 1000.0;
    }

    double t1 = glfwGetTime();
    
    m_fixedUpdate_executionTime = (t1 - t0) * 1000.0f;
}

void SGCore::Scene::update()
{
    double t0 = glfwGetTime();
    
    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        double before = glfwGetTime();
        system->update();
        double after = glfwGetTime();

        system->m_update_executionTime = (after - before) * 1000.0;
    }
    
    double t1 = glfwGetTime();
    
    m_update_executionTime = (t1 - t0) * 1000.0f;
    
    if(RenderPipelinesManager::getCurrentRenderPipeline())
    {
        RenderPipelinesManager::getCurrentRenderPipeline()->render(shared_from_this());
    }
}

std::set<SGCore::Ref<SGCore::ISystem>>& SGCore::Scene::getAllSystems() noexcept
{
    return m_systems;
}

/*size_t SGCore::Scene::createBaseEntity() noexcept
{
    size_t entity = m_ecsRegistry->createEntity();
    auto* baseEntityInfo = m_ecsRegistry->emplace<EntityBaseInfo>(entity);

    return entity;
}*/

SGCore::Layer SGCore::Scene::createLayer(const std::string& name) noexcept
{
    Layer layer = m_layers[name];
    layer.m_index = m_maxLayersCount++;
    return layer;
}

double SGCore::Scene::getUpdateFunctionExecutionTime() const noexcept
{
    return m_update_executionTime;
}

double SGCore::Scene::getFixedUpdateFunctionExecutionTime() const noexcept
{
    return m_fixedUpdate_executionTime;
}

void SGCore::Scene::addScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    auto foundIt = std::find_if(m_scenes.begin(), m_scenes.end(), [&scene](const Ref<Scene>& s) {
        return s->m_name == scene->m_name;
    });
    
    if(foundIt != m_scenes.end())
    {
        return;
    }
    
    m_scenes.push_back(scene);
}

SGCore::Ref<SGCore::Scene> SGCore::Scene::getScene(const std::string& sceneName) noexcept
{
    auto foundIt = std::find_if(m_scenes.begin(), m_scenes.end(), [&sceneName](const Ref<Scene>& s) {
        return s->m_name == sceneName;
    });
    
    return foundIt == m_scenes.end() ? nullptr : *foundIt;
}

void SGCore::Scene::setCurrentScene(const std::string& sceneName) noexcept
{
    auto foundIt = std::find_if(m_scenes.begin(), m_scenes.end(), [&sceneName](const Ref<Scene>& s) {
        return s->m_name == sceneName;
    });
    
    if(foundIt != m_scenes.end())
    {
        m_currentScene = *foundIt;
    }
    else
    {
        spdlog::error("Cannot set scene '{0}' as current! No such scene (maybe you forgot to add this scene).", sceneName);
    }
}
