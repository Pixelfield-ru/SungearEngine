//
// Created by stuka on 04.07.2023.
//

#include <SGCore/Logger/Logger.h>

#include "Scene.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "SGCore/Transformations/Controllables3DUpdater.h"
#include "SGCore/Render/RenderingBasesUpdater.h"
#include "SGCore/Render/Gizmos/BoxGizmosRenderer.h"
#include "SGCore/Render/Gizmos/LineGizmosRenderer.h"
#include "SGCore/Render/Gizmos/SphereGizmosUpdater.h"
#include "SGCore/Render/Atmosphere/AtmosphereUpdater.h"
#include "SGCore/Render/Lighting/SpotLightsUpdater.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "SGCore/Render/DebugDraw.h"
#include "SGCore/Render/SpacePartitioning/OctreesSolver.h"
#include "SGCore/Audio/AudioProcessor.h"
#include "SGCore/Motion/MotionPlannersResolver.h"
#include "SGCore/Motion/IK/IKResolver.h"
#include "SGCore/Render/Batching/BatchesUpdater.h"
#include "SGCore/Render/Instancing/InstancingUpdater.h"
#include "SGCore/Serde/Serde.h"
#include "SGCore/Serde/StandardSerdeSpecs/SerdeSpecs.h"
#include "../UI/Systems/UILayoutCalculator.h"
#include "SGCore/UI/EventHandling/EventsHandlingSystem.h"

SGCore::Scene::Scene()
{
   createLayer(SG_LAYER_OPAQUE_NAME);
}

SGCore::Scene::~Scene()
{
    for(const auto& system : m_systems)
    {
        if(system->m_scene == this)
        {
            system->m_scene = nullptr;
        }
    }
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
    
    auto renderingBasesUpdater = MakeRef<RenderingBasesUpdater>();
    addSystem(renderingBasesUpdater);
    
    auto atmosphereScatteringUpdater = MakeRef<AtmosphereUpdater>();
    addSystem(atmosphereScatteringUpdater);
    
    auto spotLightsUpdater = MakeRef<SpotLightsUpdater>();
    addSystem(spotLightsUpdater);
    
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
    
    /*auto debugDraw = MakeRef<DebugDraw>();
    addSystem(debugDraw);*/

    // ===================
    
    // space partitions solvers ==============

    auto octreesSolver = MakeRef<OctreesSolver>();
    addSystem(octreesSolver);
    
    // audio =================================
    
    auto audioProcessor = MakeRef<AudioProcessor>();
    addSystem(audioProcessor);

    // motion planners resolver =================================

    auto motionPlannersResolver = MakeRef<MotionPlannersResolver>();
    addSystem(motionPlannersResolver);

    // ik resolver =================================

    auto ikResolver = MakeRef<IKResolver>();
    addSystem(ikResolver);

    // ui =================================

    auto uiLayoutCalculator = MakeRef<UI::UILayoutCalculator>();
    addSystem(uiLayoutCalculator);

    auto uiEventsHandlingSystem = MakeRef<UI::EventsHandlingSystem>();
    addSystem(uiEventsHandlingSystem);

    // batches updater =================================

    auto batchesUpdater = MakeRef<BatchesUpdater>();
    addSystem(batchesUpdater);

    // instancing updater =================================

    auto instancingUpdater = MakeRef<InstancingUpdater>();
    addSystem(instancingUpdater);
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

    auto renderPipeline = RenderPipelinesManager::instance().getCurrentRenderPipeline();

    if(renderPipeline)
    {
        renderPipeline->render(this);
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
    if(isSystemExists(system)) return;

    system->setScene(this);
    m_systems.push_back(system);
}

const std::vector<SGCore::Ref<SGCore::ISystem>>& SGCore::Scene::getAllSystems() const noexcept
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

void SGCore::Scene::swapOrAddScene(const SceneMetaInfo& newScene) noexcept
{
    for(auto& scene : m_scenesMeta)
    {
        if(scene.m_sceneName == newScene.m_sceneName)
        {
            scene = newScene;
            return;
        }
    }

    m_scenesMeta.push_back(newScene);
}

void SGCore::Scene::addScene(const SceneMetaInfo& scene) noexcept
{
    auto foundIt = std::find_if(m_scenesMeta.begin(), m_scenesMeta.end(), [&scene](const SceneMetaInfo& s) {
        return s.m_sceneName == scene.m_sceneName;
    });
    
    if(foundIt != m_scenesMeta.end())
    {
        return;
    }

    m_scenesMeta.push_back(scene);
}

std::optional<SGCore::SceneMetaInfo> SGCore::Scene::getSceneMeta(const std::string& sceneName) noexcept
{
    auto foundIt = std::find_if(m_scenesMeta.begin(), m_scenesMeta.end(), [&sceneName](const SceneMetaInfo& s) {
        return s.m_sceneName == sceneName;
    });
    
    return foundIt == m_scenesMeta.end() ? std::nullopt : std::optional(*foundIt);
}

SGCore::Ref<SGCore::Scene> SGCore::Scene::setCurrentScene(const std::string& sceneName) noexcept
{
    auto foundIt = std::find_if(m_scenesMeta.begin(), m_scenesMeta.end(), [&sceneName](const SceneMetaInfo& s) {
        return s.m_sceneName == sceneName;
    });
    
    if(foundIt != m_scenesMeta.end())
    {
        if(!std::filesystem::exists(foundIt->m_sceneLocalPath))
        {
            LOG_E(SGCORE_TAG,
                  "Cannot set scene '{}' as current! Local path '{}' of this scene is not valid.", sceneName, Utils::toUTF8(foundIt->m_sceneLocalPath.u16string()));
            return nullptr;
        }

        const std::string sceneText = FileUtils::readFile(foundIt->m_sceneLocalPath);

        std::string sceneLoadOutputLog;
        Ref<Scene> loadedScene;
        Serde::Serializer::fromFormat(sceneText, loadedScene, sceneLoadOutputLog);

        m_currentScene = loadedScene;

        return loadedScene;
    }
    else
    {
        LOG_E(SGCORE_TAG,
              "Cannot set scene '{}' as current! No such scene (maybe you forgot to add this scene).", sceneName);
    }

    return nullptr;
}

SGCore::Ref<SGCore::Scene> SGCore::Scene::loadSceneAndSetAsCurrent(const std::filesystem::path& scenePath) noexcept
{
    if(!std::filesystem::exists(scenePath))
    {
        LOG_E(SGCORE_TAG,
              "Cannot set scene as current! Local path '{}' of this scene is not valid.", Utils::toUTF8(scenePath.u16string()));
        return nullptr;
    }

    const std::string sceneText = FileUtils::readFile(scenePath);

    std::string sceneLoadOutputLog;
    Ref<Scene> loadedScene;
    Serde::Serializer::fromFormat(sceneText, loadedScene, sceneLoadOutputLog);

    m_currentScene = loadedScene;

    if(!sceneLoadOutputLog.empty())
    {
        LOG_E(SGCORE_TAG, "Error while loading scene by path '{}': {}",
              Utils::toUTF8(scenePath.u16string()), sceneLoadOutputLog);
    }

    return loadedScene;
}

void SGCore::Scene::resolveAllEntitiesRefs() const noexcept
{
    auto entityBaseInfoView = m_ecsRegistry->template view<EntityBaseInfo::reg_t>();

    entityBaseInfoView.each([&entityBaseInfoView, this](EntityBaseInfo::reg_t& entityBaseInfo) {
        entityBaseInfo.resolveAllEntitiesRefs(m_ecsRegistry);
    });
}

void SGCore::Scene::saveToFile(const std::filesystem::path& path) noexcept
{
    m_metaInfo.m_sceneLocalPath = path;

    FileUtils::writeToFile(path, Serde::Serializer::toFormat(*this), false, true);

    LOG_I(SGCORE_TAG, "Scene '{}' has been saved!", m_metaInfo.m_sceneName)
}

bool SGCore::Scene::isSystemExists(const SGCore::Ref<SGCore::ISystem>& system) const noexcept
{
    for(const auto& sys : m_systems)
    {
        LOG_W(SGCORE_TAG, "Current system '{}' (type id: '{}'), finding system '{}' (type id: '{}').", typeid(*sys).name(), sys->getTypeID(), typeid(*system).name(), system->getTypeID());

        if(sys->getTypeID() == system->getTypeID()) return true;
    }

    return false;
}

void SGCore::Scene::setCurrentScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    m_currentScene = scene;
}
