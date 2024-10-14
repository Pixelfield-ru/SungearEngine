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
#include <SGCore/Render/ShaderComponent.h>
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
    SGCore::Scene::setCurrentScene(scene->m_scene->m_name);
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

SGCore::Ref<SGE::EditorScene> SGE::EditorScene::createSceneForEditor(const std::string& name) noexcept
{
    auto newScene = SGCore::MakeRef<SGCore::Scene>();
    newScene->m_name = name;
    newScene->createDefaultSystems();
    SGCore::Scene::addScene(newScene);

    auto editorScene = SGCore::MakeRef<EditorScene>();
    editorScene->m_scene = newScene;

    // adding standard entities with components
    // adding editor-only camera
    editorScene->m_data.m_editorCamera = newScene->getECSRegistry()->create();
    {
        const auto& camera = editorScene->m_data.m_editorCamera;

        SGCore::EntityBaseInfo& cameraBaseInfo = newScene->getECSRegistry()->emplace<SGCore::EntityBaseInfo>(camera, camera);
        cameraBaseInfo.setRawName("SGMainCamera");

        newScene->getECSRegistry()->emplace<SGCore::Ref<SGCore::Transform>>(camera, SGCore::MakeRef<SGCore::Transform>());
        newScene->getECSRegistry()->emplace<SGCore::NonSavable>(camera);
        newScene->getECSRegistry()->emplace<SGCore::Ref<SGCore::Camera3D>>(camera, SGCore::MakeRef<SGCore::Camera3D>());
        newScene->getECSRegistry()->emplace<SGCore::Ref<SGCore::RenderingBase>>(camera, SGCore::MakeRef<SGCore::RenderingBase>());
        newScene->getECSRegistry()->emplace<SGCore::Controllable3D>(camera);
        auto& layeredFrameReceiver = newScene->getECSRegistry()->emplace<SGCore::LayeredFrameReceiver>(camera);
        layeredFrameReceiver.setRenderOverlayInSeparateFrameBuffer(true);
    }

    // adding editor-only grid
    editorScene->m_data.m_editorGrid = newScene->getECSRegistry()->create();
    {

    }

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

        SGCore::Mesh& skyboxMesh = newScene->getECSRegistry()->get<SGCore::Mesh>(atmosphereEntity);
        SGCore::ShaderComponent& skyboxShaderComponent = newScene->getECSRegistry()->emplace<SGCore::ShaderComponent>(atmosphereEntity);
        SGCore::Atmosphere& atmosphereScattering = newScene->getECSRegistry()->emplace<SGCore::Atmosphere>(atmosphereEntity);
        // atmosphereScattering.m_sunRotation.z = 90.0;
        skyboxMesh.m_base.getMaterial()->addTexture2D(SGTextureType::SGTT_SKYBOX,
                                                      SGCore::AssetManager::getInstance()->loadAsset<SGCore::ICubemapTexture>("standard_skybox0")
        );
        // это топ пж
        SGCore::ShadersUtils::loadShader(skyboxShaderComponent, "SkyboxShader");
        skyboxMesh.m_base.m_meshDataRenderInfo.m_enableFacesCulling = false;

        auto& skyboxTransform = newScene->getECSRegistry()->get<SGCore::Ref<SGCore::Transform>>(atmosphereEntity);

        skyboxTransform->m_ownTransform.m_scale = { 1150, 1150, 1150 };
    }

    return editorScene;
}
