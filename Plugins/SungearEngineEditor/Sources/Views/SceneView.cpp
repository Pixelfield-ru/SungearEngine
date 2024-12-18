//
// Created by stuka on 06.09.2024.
//

#include "SceneView.h"
#include "EditorScene.h"
#include "ImGuiUtils.h"
#include <imgui_internal.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Render/LayeredFrameReceiver.h>
#include <SGCore/Graphics/API/IFrameBuffer.h>
#include <SGCore/Input/InputManager.h>
#include <SGCore/Input/InputListener.h>
#include <SGCore/Memory/Assets/ModelAsset.h>
#include <SGCore/Utils/StringInterpolation/InterpolationResolver.h>
#include <SGCore/Render/Mesh.h>
#include <SGCore/Memory/Assets/Materials/IMaterial.h>
#include <SGCore/Render/RenderingBase.h>
#include <SGCore/Render/Camera3D.h>

#include "SungearEngineEditor.h"
#include "Views/MainView.h"
#include "Views/Explorer/DirectoryExplorer.h"

bool SGE::SceneView::begin()
{
    return IView::begin();
}

void SGE::SceneView::renderBody()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);

    ImGui::Begin("SceneView");

    const auto windowPos = ImGui::GetCursorScreenPos();
    const auto windowSize = ImGui::GetWindowSize();

    auto currentEditorScene = EditorScene::getCurrentScene();
    if(currentEditorScene && currentEditorScene->m_scene)
    {
        /*auto tex = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>(
                "../SGResources/textures/skyboxes/skybox0/standard_skybox0_zfront.png"
        );*/
        auto* layeredFrameReceiver = currentEditorScene->m_scene->getECSRegistry()->try_get<SGCore::LayeredFrameReceiver>
                (EditorScene::getCurrentScene()->m_data.m_editorCamera);

        auto renderingBase = currentEditorScene->m_scene->getECSRegistry()->try_get<SGCore::Ref<SGCore::RenderingBase>>
                (EditorScene::getCurrentScene()->m_data.m_editorCamera);

        if(layeredFrameReceiver)
        {
            ImGui::Image(layeredFrameReceiver->m_layersFXFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7
            )->getTextureNativeHandler(), ImGui::GetContentRegionAvail(), { 0, 1 }, { 1, 0 });

            acceptFilesFromDirectoryExplorer();
        }

        if(renderingBase)
        {
            (*renderingBase)->m_aspect = windowSize.x / windowSize.y;
            if(layeredFrameReceiver)
            {
                /*layeredFrameReceiver->m_layersFrameBuffer->m_viewportWidth = windowSize.x;
                layeredFrameReceiver->m_layersFrameBuffer->m_viewportHeight = windowSize.y;
                layeredFrameReceiver->m_layersFrameBuffer->m_viewportPosX = windowPos.x;
                layeredFrameReceiver->m_layersFrameBuffer->m_viewportPosY = windowPos.y;*/
            }
            // glViewport(0, 0, windowSize.x, windowSize.y);
        }

        if(SGCore::InputManager::getMainInputListener()->keyboardKeyDown(SGCore::KeyboardKey::KEY_LEFT_CONTROL) &&
           SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_S) &&
           !ImGui::GetIO().WantTextInput)
        {
            const auto& scenePath = currentEditorScene->m_scene->m_metaInfo.m_sceneLocalPath;
            currentEditorScene->saveByPath(scenePath.parent_path(), scenePath.stem());
            std::cout << "scene saved" << std::endl;
        }

        if(SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_K))
        {
            /*auto noMat = SGCore::AssetManager::getInstance()->getOrAddAssetByPath<SGCore::IMaterial>("${enginePath}/Resources/materials/no_material.sgmat");
            auto& mesh = EditorScene::getCurrentScene()->m_scene->getECSRegistry()->get<SGCore::Mesh>(EditorScene::getCurrentScene()->m_data.m_editorGrid);
            mesh.m_base.setMaterial(noMat);*/
        }
    }

    ImGui::End();

    ImGui::PopStyleVar(2);
}

void SGE::SceneView::end()
{
    IView::end();
}

void SGE::SceneView::acceptFilesFromDirectoryExplorer() noexcept
{
    auto directoryExplorer = SungearEngineEditor::getInstance()->getMainView()->getDirectoryExplorer();

    DragNDropInfo dragNDropInfo;
    dragNDropInfo.m_type = DragNDropType::TARGET;
    dragNDropInfo.m_name = directoryExplorer->getDragNDropPayloadName();
    dragNDropInfo.m_payloadProcessFunction = [directoryExplorer, this](const ImGuiPayload* payload) {
        for(const auto* file : directoryExplorer->getSelectedFiles())
        {
            const auto fileExt = file->getPath().extension();

            if(fileExt == ".obj" || fileExt == ".gltf")
            {
                loadModelByPath(file->getPath());
            }
        }
    };

    ImGuiUtils::UseDragNDrop(&dragNDropInfo);
}

void SGE::SceneView::loadModelByPath(const std::filesystem::path& modelPath) const noexcept
{
    const SGCore::InterpolatedPath relativeModelPath = std::filesystem::relative(modelPath, SGCore::PathInterpolationMarkup::getGlobalMarkup()["projectPath"]);
    auto modelAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("${projectPath}" / relativeModelPath);

    if(!modelAsset)
    {
        LOG_E(SGEDITOR_TAG, "Can not drop model asset on scene: can not load model asset by path '{}'",
              SGCore::Utils::toUTF8(modelPath.u16string()));
        return;
    }

    if(modelAsset->m_nodes.empty())
    {
        LOG_E(SGEDITOR_TAG, "Can not drop model asset on scene: model by path '{}' does not have nodes.",
              SGCore::Utils::toUTF8(modelPath.u16string()));
        return;
    }

    std::vector<SGCore::entity_t> entities;
    modelAsset->m_nodes[0]->addOnScene(SGCore::Scene::getCurrentScene(), SG_LAYER_OPAQUE_NAME, [&entities](const auto& entity) {
        entities.push_back(entity);

        auto curScene = EditorScene::getCurrentScene();
        auto* editorCamera = curScene->m_scene->getECSRegistry()->try_get<SGCore::Ref<SGCore::Camera3D>>(curScene->m_data.m_editorCamera);
        (*editorCamera)->m_pickableEntities.emplace(entity);
        /*auto noMat = SGCore::AssetManager::getInstance()->getOrAddAssetByPath<SGCore::IMaterial>("${enginePath}/Resources/materials/no_material.sgmat");

        auto* mesh = SGCore::Scene::getCurrentScene()->getECSRegistry()->try_get<SGCore::Mesh>(entity);
        if(mesh)
        {
            mesh->m_base.setMaterial(noMat);
        }*/
    });
    /*auto& rotation = SGCore::Scene::getCurrentScene()->getECSRegistry()->get<SGCore::Ref<SGCore::Transform>>(entities[0])->m_ownTransform.m_rotation;
    rotation = glm::rotate(glm::identity<glm::quat>(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));*/

    // SG_ASSERT(false, "");
    std::cout << "" << std::endl;
}
