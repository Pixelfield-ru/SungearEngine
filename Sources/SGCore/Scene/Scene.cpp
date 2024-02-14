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
#include "SGCore/ECSObservers/Flags/ModelMatrixChangedFlag.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/ECSObservers/Observers/ModelMatrixChangedObserver.h"
#include "SGCore/Render/Atmosphere/AtmosphereScatteringUpdater.h"
#include "SGCore/Render/Lighting/DirectionalLightsUpdater.h"

SGCore::Scene::Scene()
{
   createLayer(SG_LAYER_OPAQUE_NAME);
}

void SGCore::Scene::createDefaultSystems()
{
    // -------------
    // rendering
    
    RenderPipelinesManager::setRenderPipeline(MakeRef<PBRRenderPipeline>());
    
    auto renderingBasesUpdater = MakeRef<RenderingBasesUpdater>();
    m_systems.emplace(renderingBasesUpdater);
    
    auto atmosphereScatteringUpdater = MakeRef<AtmosphereScatteringUpdater>();
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

    auto modelMatrixChangedObserver = MakeRef<ModelMatrixChangedObserver>();
    addFlagObserverSystem<ModelMatrixChangedFlag>(modelMatrixChangedObserver);

    // -------------

    // m_systems.emplace(pipelineSystem);
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
