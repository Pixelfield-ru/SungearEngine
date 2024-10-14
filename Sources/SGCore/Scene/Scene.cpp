//
// Created by stuka on 04.07.2023.
//

#include <GLFW/glfw3.h>
#include <SGCore/Logger/Logger.h>

#include "SGCore/Render/Batching/BatchesRenderer.h"
#include "Scene.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "SGCore/Transformations/Controllables3DUpdater.h"
#include "SGCore/Render/RenderingBasesUpdater.h"
#include "SGCore/Render/Gizmos/BoxGizmosRenderer.h"
#include "SGCore/Render/Gizmos/LineGizmosRenderer.h"
#include "SGCore/Render/Gizmos/SphereGizmosUpdater.h"
#include "SGCore/Render/Atmosphere/AtmosphereUpdater.h"
#include "SGCore/Render/Lighting/DirectionalLightsUpdater.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "SGCore/UI/UIElement.h"
#include "SGCore/Memory/Assets/XMLDocument.h"
#include "SGCore/Render/DebugDraw.h"
#include "SGCore/Render/SpacePartitioning/OctreesSolver.h"
#include "SGCore/Audio/AudioProcessor.h"
#include "SGCore/Serde/Serde.h"
#include "SGCore/Serde/StandardSerdeSpecs.h"

SGCore::Scene::Scene()
{
   createLayer(SG_LAYER_OPAQUE_NAME);

   auto sceneEntity = m_ecsRegistry->create();
   m_ecsRegistry->emplace<Scene*>(sceneEntity, this);
}

void SGCore::Scene::createDefaultSystems()
{
    // physics ===================

    // ALWAYS BEFORE TRANSFORMATIONS UPDATER
    auto physicsWorld = MakeRef<PhysicsWorld3D>();
    addSystem(physicsWorld);

    //std::cout << "dfdfdf" << std::endl;
    
    // ===================
    
    // ===================
    // rendering
    
    auto thisShared = shared_from_this();
    
    auto renderingBasesUpdater = MakeRef<RenderingBasesUpdater>();
    addSystem(renderingBasesUpdater);
    
    auto atmosphereScatteringUpdater = MakeRef<AtmosphereUpdater>();
    addSystem(atmosphereScatteringUpdater);
    
    auto directionalLightsUpdater = MakeRef<DirectionalLightsUpdater>();
    addSystem(directionalLightsUpdater);
    
    // ===================
    
    // transformations ===================

    auto transformationsUpdater = MakeRef<TransformationsUpdater>();
    addSystem(transformationsUpdater);

    auto controllables3DUpdater = MakeRef<Controllables3DUpdater>();
    addSystem(controllables3DUpdater);
    
    // gizmos ===================

    auto boxGizmosUpdater = MakeRef<BoxGizmosRenderer>();
    addSystem(boxGizmosUpdater);

    auto lineGizmosUpdater = MakeRef<LineGizmosRenderer>();
    addSystem(lineGizmosUpdater);

    auto sphereGizmosUpdater = MakeRef<SphereGizmosUpdater>();
    addSystem(sphereGizmosUpdater);
    
    auto debugDraw = MakeRef<DebugDraw>();
    addSystem(debugDraw);

    // ===================
    
    auto batchesRenderer = MakeRef<BatchesRenderer>();
    addSystem(batchesRenderer);
    
    // space partitions solvers ==============

    auto octreesSolver = MakeRef<OctreesSolver>();
    addSystem(octreesSolver);
    
    // audio =================================
    
    auto audioProcessor = MakeRef<AudioProcessor>();
    addSystem(audioProcessor);
}

// ----------------

SGCore::Ref<SGCore::Scene> SGCore::Scene::getCurrentScene() noexcept
{
    return m_currentScene;
}

// ==================================================================

void SGCore::Scene::update(const double& dt, const double& fixedDt)
{
    auto t0 = now();
    
    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        auto before = now();
        system->update(dt, fixedDt);
        auto after = now();
        
        system->m_executionTimes["update"] = timeDiff<double, std::milli>(before, after);
    }
    
    auto t1 = now();
    
    m_update_executionTime = timeDiff<double, std::milli>(t0, t1);

    auto renderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();

    if(renderPipeline)
    {
        renderPipeline->render(shared_from_this());
    }
}

void SGCore::Scene::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto t0 = now();

    for(auto& system : m_systems)
    {
        if(!system->m_active) continue;

        auto before = now();
        system->fixedUpdate(dt, fixedDt);
        auto after = now();

        system->m_executionTimes["fixedUpdate"] = timeDiff<double, std::milli>(before, after);
    }

    auto t1 = now();
    
    m_fixedUpdate_executionTime = timeDiff<double, std::milli>(t0, t1);
}

void SGCore::Scene::addSystem(const Ref<ISystem>& system) noexcept
{
    system->setScene(shared_from_this());
    m_systems.push_back(system);
}

const std::vector<SGCore::Ref<SGCore::ISystem>>& SGCore::Scene::getAllSystems() noexcept
{
    return m_systems;
}

/*size_t SGCore::Scene::createBaseEntity() noexcept
{
    size_t entity = m_ecsRegistry->createEntity();
    auto* baseEntityInfo = m_ecsRegistry->emplace<EntityBaseInfo>(entity);

    return entity;
}*/

SGCore::Ref<SGCore::Layer> SGCore::Scene::createLayer(const std::string& name) noexcept
{
    auto foundIt = std::find_if(m_layers.begin(), m_layers.end(), [&name](const Ref<Layer>& layer) {
        return name == layer->m_name;
    });
    
    if(foundIt != m_layers.end())
    {
        return *foundIt;
    }
    else
    {
        Ref<Layer> newLayer = MakeRef<Layer>();
        newLayer->m_name = name;
        newLayer->m_index = m_maxLayersCount++;
        m_layers.push_back(newLayer);
        
        return newLayer;
    }
}

SGCore::Ref<SGCore::Layer> SGCore::Scene::getLayer(const std::string& name) noexcept
{
    auto foundIt = std::find_if(m_layers.begin(), m_layers.end(), [&name](const Ref<Layer>& layer) {
        return name == layer->m_name;
    });
    
    return foundIt != m_layers.end() ? *foundIt : nullptr;
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
        LOG_E(SGCORE_TAG,
              "Cannot set scene '{}' as current! No such scene (maybe you forgot to add this scene).", sceneName);
    }
}

void SGCore::Scene::setUIXMLDocument(const SGCore::Ref<SGCore::XMLDocument>& xmlDocument) noexcept
{
    m_UIXMLDocument = xmlDocument;
}

SGCore::Weak<SGCore::XMLDocument> SGCore::Scene::getUIXMLDocument() const noexcept
{
    return m_UIXMLDocument;
}

void SGCore::Scene::reloadUI() noexcept
{
    m_ecsRegistry->clear<UIElement>();
    
    if(auto lockedUIDocument = m_UIXMLDocument.lock())
    {
        //Fваkd
    }
}

void SGCore::Scene::saveToFile(const std::filesystem::path& path) noexcept
{
    FileUtils::writeToFile(path, Serde::Serializer::toFormat(*this), false, true);

    LOG_I(SGCORE_TAG, "Scene '{}' has been saved!", m_name)
}
