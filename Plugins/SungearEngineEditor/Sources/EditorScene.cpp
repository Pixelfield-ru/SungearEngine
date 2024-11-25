//
// Created by stuka on 06.09.2024.
//

#include "EditorScene.h"
#include "SungearEngineEditor.h"

#include <SGCore/Serde/Serde.h>
#include <SGCore/Serde/StandardSerdeSpecs.h>
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

template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<SGE::EditorSceneData, TFormatType> : BaseTypes<>, DerivedTypes<>
{
    static inline const std::string type_name = "SGE::EditorSceneData";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SerializableValueView<SGE::EditorSceneData, TFormatType>& valueView)
    {
        valueView.getValueContainer().addMember("m_editorCamera", valueView.m_data->m_editorCamera);
        valueView.getValueContainer().addMember("m_editorGrid", valueView.m_data->m_editorGrid);
    }

    static void deserialize(DeserializableValueView<SGE::EditorSceneData, TFormatType>& valueView)
    {
        const auto editorCamera = valueView.getValueContainer()
                .template getMember<SGCore::entity_t>("m_editorCamera");
        if(editorCamera)
        {
            valueView.m_data->m_editorCamera = *editorCamera;
        }

        const auto editorGrid = valueView.getValueContainer()
                .template getMember<SGCore::entity_t>("m_editorGrid");
        if(editorGrid)
        {
            valueView.m_data->m_editorGrid = *editorGrid;
        }
    }
};

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

    SGCore::FileUtils::writeToFile(editorScenePath, SGCore::Serde::Serializer::toFormat(m_data), false, true);
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
        });

        atmosphereEntity = skyboxEntities[2];

        auto& skyboxMesh = newScene->getECSRegistry()->get<SGCore::Mesh>(atmosphereEntity);
        auto& atmosphereScattering = newScene->getECSRegistry()->emplace<SGCore::Atmosphere>(atmosphereEntity);
        atmosphereScattering.m_sunRotation.z = 90.0;
        skyboxMesh.m_base.setMaterial(SGCore::AssetManager::getInstance()->getAsset<SGCore::IMaterial>("standard_skybox_material0"));
        skyboxMesh.m_base.m_meshDataRenderInfo.m_enableFacesCulling = false;

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
        layeredFrameReceiver.setRenderOverlayInSeparateFrameBuffer(true);
    }

    // adding editor-only grid
    {
        auto scene = m_scene;

        std::vector<SGCore::entity_t> gridEntities;
        auto cubeModel =  SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("plane_model");
        cubeModel->m_nodes[0]->addOnScene(m_scene, SG_LAYER_OPAQUE_NAME, [&gridEntities, scene](const auto& entity) {
            gridEntities.push_back(entity);
            scene->getECSRegistry()->emplace<SGCore::IgnoreOctrees>(entity);
        });

        m_data.m_editorGrid = gridEntities[2];
        registry->emplace<SGCore::NonSavable>(gridEntities[0]);
        auto& gridMesh = scene->getECSRegistry()->get<SGCore::Mesh>(m_data.m_editorGrid);
        gridMesh.m_base.setMaterial(SGCore::AssetManager::getInstance()->getAsset<SGCore::IMaterial>("standard_grid_material"));
        gridMesh.m_base.m_meshDataRenderInfo.m_enableFacesCulling = false;

        auto& gridTransform = *scene->getECSRegistry()->get<SGCore::Ref<SGCore::Transform>>(m_data.m_editorGrid);
        gridTransform.m_ownTransform.m_scale = { 100.0f, 100.0f, 1.0f, };

        auto& editorTransform = *scene->getECSRegistry()->get<SGCore::Ref<SGCore::Transform>>(m_data.m_editorGrid);
        editorTransform.m_ownTransform.m_rotation = glm::rotate(glm::identity<glm::quat>(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));;

        auto& cameraLayeredFrameReceiver = scene->getECSRegistry()->get<SGCore::LayeredFrameReceiver>(m_data.m_editorCamera);
        auto chunksPPLayer = cameraLayeredFrameReceiver.addLayer("chunks_layer");
        auto testLayerShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>(
                SGCore::CoreMain::getSungearEngineRootPath() / "Plugins/SungearEngineEditor/Resources/shaders/glsl4/test_layer.glsl");
        chunksPPLayer->setFXSubPassShader(testLayerShader->getSubPassShader("SGLPPLayerFXPass"));

        chunksPPLayer->m_frameBuffer->bind();
        chunksPPLayer->m_frameBuffer->addAttachment(
                SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1,
                SGGColorFormat::SGG_RGBA,
                SGGColorInternalFormat::SGG_RGBA8,
                0,
                0
        );
        chunksPPLayer->m_frameBuffer->unbind();

        chunksPPLayer->m_attachmentsToRenderIn.push_back(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1);

        chunksPPLayer->m_attachmentsForCombining[SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1] = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1;

        SGCore::PostProcessFXSubPass subPass;
        subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1;
        chunksPPLayer->m_subPasses.push_back(subPass);

        chunksPPLayer->getFXSubPassShader()->addTextureBinding("currentLayer", chunksPPLayer->m_frameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));

        gridMesh.m_base.m_layeredFrameReceiversMarkup[&cameraLayeredFrameReceiver] = chunksPPLayer;
    }
}
