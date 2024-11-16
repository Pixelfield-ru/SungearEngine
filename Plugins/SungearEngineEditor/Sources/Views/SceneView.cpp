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

    auto currentEditorScene = EditorScene::getCurrentScene();
    if(currentEditorScene && currentEditorScene->m_scene)
    {
        /*auto tex = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>(
                "../SGResources/textures/skyboxes/skybox0/standard_skybox0_zfront.png"
        );*/
        auto* layeredFrameReceiver = currentEditorScene->m_scene->getECSRegistry()->try_get<SGCore::LayeredFrameReceiver>
                (EditorScene::getCurrentScene()->m_data.m_editorCamera);

        if(layeredFrameReceiver)
        {
            ImGui::Image(layeredFrameReceiver->getOverlayFrameBuffer()->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0
            )->getTextureNativeHandler(), ImGui::GetContentRegionAvail(), { 0, 1 }, { 1, 0 });

            acceptFilesFromDirectoryExplorer();
        }

        if(SGCore::InputManager::getMainInputListener()->keyboardKeyDown(SGCore::KeyboardKey::KEY_LEFT_CONTROL) &&
           SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_S) &&
           !ImGui::GetIO().WantTextInput)
        {
            const auto& scenePath = currentEditorScene->m_scene->m_metaInfo.m_sceneLocalPath;
            currentEditorScene->saveByPath(scenePath.parent_path(), scenePath.stem());
            std::cout << "scene saved" << std::endl;
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

            if(fileExt == ".obj")
            {
                loadModelByPath(file->getPath());
            }
        }
    };

    ImGuiUtils::UseDragNDrop(&dragNDropInfo);
}

void SGE::SceneView::loadModelByPath(const std::filesystem::path& modelPath) const noexcept
{
    auto modelAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelPath);

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

    modelAsset->m_nodes[0]->addOnScene(SGCore::Scene::getCurrentScene(), SG_LAYER_OPAQUE_NAME);

    SG_ASSERT(false, "");
    std::cout << "" << std::endl;
}
