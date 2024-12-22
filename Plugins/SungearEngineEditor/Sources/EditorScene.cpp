//
// Created by stuka on 06.09.2024.
//

#include "EditorScene.h"
#include "SungearEngineEditor.h"

#include <SGCore/Utils/FileUtils.h>
#include <SGCore/Transformations/Transform.h>
#include <SGCore/Render/Camera3D.h>
#include <SGCore/Transformations/Controllable3D.h>
#include <SGCore/Render/RenderingBase.h>
#include <SGCore/Render/LayeredFrameReceiver.h>
#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Memory/Assets/ModelAsset.h>
#include <SGCore/Render/SpacePartitioning/IgnoreOctrees.h>
#include <SGCore/Render/Mesh.h>
#include <SGCore/Render/Atmosphere/Atmosphere.h>
#include <SGCore/Memory/Assets/Materials/IMaterial.h>
#include <SGCore/Graphics/API/ICubemapTexture.h>
#include <SGCore/Serde/Components/NonSavable.h>

void SGE::EditorScene::setCurrentScene(const SGCore::Ref<SGE::EditorScene>& scene) noexcept
{
    s_currentScene = scene;
    SGCore::Scene::setCurrentScene(scene->m_scene);
}

SGCore::Ref<SGE::EditorScene> SGE::EditorScene::getCurrentScene() noexcept
{
    return s_currentScene;
}

void SGE::EditorScene::saveByPath(const std::filesystem::path& toPath, const std::filesystem::path& fileName) const noexcept
{
    if(!m_scene) return;

    std::filesystem::path scenePath = toPath / fileName;
    scenePath += ".sgscene";

    std::filesystem::path editorScenePath = toPath / fileName;
    editorScenePath += ".es";

    m_scene->saveToFile(scenePath);

    SGCore::FileUtils::writeToFile(editorScenePath,
                                   SGCore::Serde::Serializer::toFormat(SGCore::Serde::Serializer::getDefaultFormatType(),
                                                                       m_data, *m_scene), false, true);

    // SGCore::FileUtils::writeToFile(editorScenePath, SGCore::Serde::Serializer::toFormat(m_data), false, true);
}

SGCore::Ref<SGE::EditorScene> SGE::EditorScene::loadByPath(const std::filesystem::path& fromPath,
                                                           const std::filesystem::path& fileName) noexcept
{
    SGCore::Ref<EditorScene> loadedScene = SGCore::MakeRef<EditorScene>();

    std::filesystem::path scenePath = fromPath / fileName;
    scenePath += ".sgscene";

    std::filesystem::path editorScenePath = fromPath / fileName;
    editorScenePath += ".es";

    {
        std::string outputLog;
        SGCore::Serde::Serializer::fromFormat(SGCore::FileUtils::readFile(scenePath), loadedScene->m_scene, outputLog);

        if(!outputLog.empty())
        {
            LOG_E(SGEDITOR_TAG, "Error while deserializing scene: '{}'", outputLog);
        }
    }

    {
        std::string outputLog;
        SGCore::Serde::Serializer::fromFormat(SGCore::FileUtils::readFile(editorScenePath), loadedScene->m_data,
                                              SGCore::Serde::Serializer::getDefaultFormatType(), outputLog,
                                              *loadedScene->m_scene
        );

        if(!outputLog.empty())
        {
            LOG_E(SGEDITOR_TAG, "Error while deserializing editor scene: '{}'", outputLog);
        }
    }

    // adding editor camera to Pickable component of all entities
    auto pickableEntitiesView = loadedScene->m_scene->getECSRegistry()->view<SGCore::Pickable>();
    pickableEntitiesView.each([&loadedScene](SGCore::Pickable& pickable){
        pickable.m_pickableForCameras.push_back(loadedScene->m_data.m_editorCamera);
    });

    loadedScene->m_scene->resolveAllEntitiesRefs();

    // resolving camera entities refs
    /*{
        auto* cameraBaseInfo = loadedScene->m_scene->getECSRegistry()->try_get<SGCore::EntityBaseInfo>(loadedScene->m_data.m_editorCamera);
        if(cameraBaseInfo)
        {
            cameraBaseInfo->resolveAllEntitiesRefs(loadedScene->m_scene->getECSRegistry());
        }
        else
        {
            LOG_E(SGEDITOR_TAG, "Can not resolve EntityRef`s for camera '{}': this camera does not have EntityBaseInfo component!",
                  std::to_underlying(loadedScene->m_data.m_editorCamera));
        }
    }*/

    return loadedScene;
}

SGCore::Ref<SGE::EditorScene> SGE::EditorScene::createBasicScene(const std::string& name) noexcept
{
    auto newScene = SGCore::MakeRef<SGCore::Scene>();
    newScene->m_metaInfo.m_sceneName = name;
    newScene->createDefaultSystems();

    auto editorScene = SGCore::MakeRef<EditorScene>();
    editorScene->m_scene = newScene;

    editorScene->addEditorEntities();

    // adding scene atmosphere
    SGCore::entity_t atmosphereEntity = entt::null;
    {
        std::vector<SGCore::entity_t> skyboxEntities;
        auto cubeModel =  SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("cube_model");
        cubeModel->m_nodes[0]->addOnScene(newScene, SG_LAYER_OPAQUE_NAME, [&skyboxEntities, newScene](const auto& entity) {
            skyboxEntities.push_back(entity);
            newScene->getECSRegistry()->emplace<SGCore::IgnoreOctrees>(entity);
            newScene->getECSRegistry()->remove<SGCore::Pickable>(entity);
        });

        atmosphereEntity = skyboxEntities[2];

        auto& skyboxMesh = newScene->getECSRegistry()->get<SGCore::Mesh>(atmosphereEntity);
        auto& atmosphereScattering = newScene->getECSRegistry()->emplace<SGCore::Atmosphere>(atmosphereEntity);
        atmosphereScattering.m_sunRotation.z = 90.0;
        skyboxMesh.m_base.setMaterial(SGCore::AssetManager::getInstance()->getAsset<SGCore::IMaterial>("standard_skybox_material0"));

        auto& skyboxTransform = newScene->getECSRegistry()->get<SGCore::Ref<SGCore::Transform>>(atmosphereEntity);

        skyboxTransform->m_ownTransform.m_scale = { 1150, 1150, 1150 };
    }

    return editorScene;
}

void SGE::EditorScene::addEditorEntities() noexcept
{
    auto registry = m_scene->getECSRegistry();

    // adding standard entities with components
    // adding editor-only camera
    m_data.m_editorCamera = registry->create();
    {
        const auto& camera = m_data.m_editorCamera;

        SGCore::EntityBaseInfo& cameraBaseInfo = registry->emplace<SGCore::EntityBaseInfo>(camera, camera);
        cameraBaseInfo.setRawName("SGMainCamera");

        registry->emplace<SGCore::Ref<SGCore::Transform>>(camera, SGCore::MakeRef<SGCore::Transform>());
        registry->emplace<SGCore::NonSavable>(camera);
        registry->emplace<SGCore::Ref<SGCore::Camera3D>>(camera, SGCore::MakeRef<SGCore::Camera3D>());
        registry->emplace<SGCore::Ref<SGCore::RenderingBase>>(camera, SGCore::MakeRef<SGCore::RenderingBase>());
        registry->emplace<SGCore::Controllable3D>(camera);
        auto& layeredFrameReceiver = registry->emplace<SGCore::LayeredFrameReceiver>(camera);
    }

    // adding editor-only grid
    {
        auto scene = m_scene;

        std::vector<SGCore::entity_t> gridEntities;
        auto cubeModel =  SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("plane_model");
        cubeModel->m_nodes[0]->addOnScene(m_scene, SG_LAYER_OPAQUE_NAME, [&gridEntities, scene](const auto& entity) {
            gridEntities.push_back(entity);
            scene->getECSRegistry()->emplace<SGCore::IgnoreOctrees>(entity);
            scene->getECSRegistry()->remove<SGCore::Pickable>(entity);
        });

        m_data.m_editorGrid = gridEntities[2];
        registry->emplace<SGCore::NonSavable>(gridEntities[0]);
        auto& gridMesh = scene->getECSRegistry()->get<SGCore::Mesh>(m_data.m_editorGrid);
        gridMesh.m_base.setMaterial(SGCore::AssetManager::getInstance()->getAsset<SGCore::IMaterial>("standard_grid_material"));

        auto& gridTransform = *scene->getECSRegistry()->get<SGCore::Ref<SGCore::Transform>>(m_data.m_editorGrid);
        // gridTransform.m_ownTransform.m_scale = { 100.0f, 100.0f, 1.0f, };
        gridTransform.m_ownTransform.m_scale = { 3.0f, 3.0f, 1.0f, };
        gridTransform.m_ownTransform.m_rotation = glm::rotate(glm::identity<glm::quat>(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));;

        /*auto& cameraLayeredFrameReceiver = scene->getECSRegistry()->get<SGCore::LayeredFrameReceiver>(m_data.m_editorCamera);
        auto chunksPPLayer = cameraLayeredFrameReceiver.addLayer("chunks_layer");
        auto testLayerShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>(
                SGCore::CoreMain::getSungearEngineRootPath() / "Plugins/SungearEngineEditor/Resources/shaders/glsl4/test_layer.glsl");
        chunksPPLayer->setFXSubPassShader(testLayerShader->getSubPassShader("SGPPLayerFXPass"));

        SGCore::PostProcessFXSubPass subPass;
        // subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1;
        chunksPPLayer->m_subPasses.push_back(subPass);

        // chunksPPLayer->getFXSubPassShader()->addTextureBinding("currentLayer", chunksPPLayer->m_frameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));

        gridMesh.m_base.m_layeredFrameReceiversMarkup[&cameraLayeredFrameReceiver] = chunksPPLayer;*/
    }
}
