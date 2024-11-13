//
// Created by stuka on 06.09.2024.
//

#include "SceneView.h"
#include "EditorScene.h"
#include <imgui_internal.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Render/LayeredFrameReceiver.h>
#include <SGCore/Graphics/API/IFrameBuffer.h>

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
        }
    }

    ImGui::End();

    ImGui::PopStyleVar(2);
}

void SGE::SceneView::end()
{
    IView::end();
}
