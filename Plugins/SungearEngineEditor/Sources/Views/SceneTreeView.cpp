//
// Created by ilya on 23.06.24.
//

#include <imgui.h>
#include <imgui_internal.h>
#include <SGCore/Render/LayeredFrameReceiver.h>
#include <SGCore/Graphics/API/IFrameBuffer.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include "SceneTreeView.h"
#include "EditorScene.h"

bool SGE::SceneTreeView::begin()
{
    return IView::begin();
}

void SGE::SceneTreeView::renderBody()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);
    
    ImGui::Begin("SceneTreeView");

    auto currentEditorScene = EditorScene::getCurrentScene();
    if(currentEditorScene && currentEditorScene->m_scene)
    {
        auto* layeredFrameReceiver = currentEditorScene->m_scene->getECSRegistry()->try_get<SGCore::LayeredFrameReceiver>
                (EditorScene::getCurrentScene()->m_data.m_editorCamera);

        for(const auto& layer : layeredFrameReceiver->getLayers())
        {
            ImGui::Text(("Attachments of layer '" + layer->m_name + "'").c_str());

            for(const auto& attachment : layer->m_frameBuffer->getAttachments())
            {
                ImGui::Text(("Attachment '" + sgFrameBufferAttachmentTypeToString(attachment.first) + "'").c_str());

                const float compressionCoeff = 4.0f;

                ImGui::Image(attachment.second->getTextureNativeHandler(),
                             { attachment.second->getWidth() / compressionCoeff, attachment.second->getHeight() / compressionCoeff }, { 0, 1 }, { 1, 0 });
            }
        }
    }
    
    ImGui::End();
    
    ImGui::PopStyleVar(2);
}

void SGE::SceneTreeView::end()
{
    IView::end();
}
