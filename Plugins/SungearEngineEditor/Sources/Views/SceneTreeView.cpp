//
// Created by ilya on 23.06.24.
//

#include <imgui.h>
#include <imgui_internal.h>
#include <SGCore/Render/LayeredFrameReceiver.h>
#include <SGCore/Graphics/API/IFrameBuffer.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Memory/Assets/SkeletalAnimationAsset.h>
#include "SceneTreeView.h"
#include "EditorScene.h"
#include "ImGuiUtils.h"
#include "SungearEngineEditor.h"

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

    const float compressionCoeff = 4.0f;

    auto currentEditorScene = EditorScene::getCurrentScene();
    if(currentEditorScene && currentEditorScene->m_scene)
    {
        auto* layeredFrameReceiver = currentEditorScene->m_scene->getECSRegistry()->tryGet<SGCore::LayeredFrameReceiver>
                (EditorScene::getCurrentScene()->m_data.m_editorCamera);

        ImGui::Text("Attachments of layered frame receiver.");

        {
            const auto& attachment0 = layeredFrameReceiver->m_layersFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0
            );

            ImGui::Image(attachment0->getTextureNativeHandler(),
                         {attachment0->getWidth() / compressionCoeff, attachment0->getHeight() / compressionCoeff},
                         {0, 1}, {1, 0}
            );

            const auto& attachment1 = layeredFrameReceiver->m_layersFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1
            );

            ImGui::Image(attachment1->getTextureNativeHandler(),
                         {attachment1->getWidth() / compressionCoeff, attachment1->getHeight() / compressionCoeff},
                         {0, 1}, {1, 0}
            );

            const auto& attachment2 = layeredFrameReceiver->m_layersFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2
            );

            ImGui::Image(attachment2->getTextureNativeHandler(),
                         {attachment2->getWidth() / compressionCoeff, attachment2->getHeight() / compressionCoeff},
                         {0, 1}, {1, 0}
            );

            const auto& attachment3 = layeredFrameReceiver->m_layersFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3
            );

            ImGui::Image(attachment3->getTextureNativeHandler(),
                         {attachment3->getWidth() / compressionCoeff, attachment3->getHeight() / compressionCoeff},
                         {0, 1}, {1, 0}
            );

            /*const auto& attachment4 = layeredFrameReceiver->m_layersFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT4
            );

            ImGui::Image(attachment4->getTextureNativeHandler(),
                         {attachment4->getWidth() / compressionCoeff, attachment4->getHeight() / compressionCoeff},
                         {0, 1}, {1, 0}
            );*/

            const auto& attachment31 = layeredFrameReceiver->m_layersFXFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7
            );

            ImGui::Image(attachment31->getTextureNativeHandler(),
                         {attachment31->getWidth() / compressionCoeff, attachment31->getHeight() / compressionCoeff},
                         {0, 1}, {1, 0}
            );
        }

        ImGui::TreePush("Entities");

        auto entitiesView = currentEditorScene->m_scene->getECSRegistry()->view<SGCore::EntityBaseInfo>();
        for(const auto& e : entitiesView)
        {
            drawTreeNode(e, true);
        }

        ImGui::TreePop();

        ImGui::Separator();

        ImGui::TreePush("Skeletal animations");

        auto skeletalAnimationAssets = SGCore::AssetManager::getInstance()->getAssetsWithType<SGCore::SkeletalAnimationAsset>();

        for(const auto& animation : skeletalAnimationAssets)
        {
            if(ImGui::TreeNode("Skeletal animation: %s", animation->m_animationName.c_str()))
            {
                ImGui::TreePop();
            }
            // drawSkeletonNode(skeleton->m_rootBone);
        }

        ImGui::TreePop();
    }
    
    ImGui::End();
    
    ImGui::PopStyleVar(2);
}

void SGE::SceneTreeView::end()
{
    IView::end();
}

void SGE::SceneTreeView::drawTreeNode(const SGCore::ECS::entity_t& parentEntity, bool checkForRoot) noexcept
{
    static auto formEntityName = [](const SGCore::EntityBaseInfo& entityBaseInfo, const SGCore::ECS::entity_t& entity) {
        return entityBaseInfo.getName() + " (entity: " + std::to_string(std::to_underlying(entity)) + ")";
    };

    const auto& currentScene = EditorScene::getCurrentScene()->m_scene;

    auto& entityBaseInfo = currentScene->getECSRegistry()->get<SGCore::EntityBaseInfo>(parentEntity);

    if((entityBaseInfo.getParent() == entt::null || !checkForRoot) &&
       ImGui::TreeNodeEx(formEntityName(entityBaseInfo, parentEntity).c_str(), ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow))
    {
        drawEntity(parentEntity);

        for(const auto& childEntity : entityBaseInfo.getChildren())
        {
            auto& childEntityBaseInfo = currentScene->getECSRegistry()->get<SGCore::EntityBaseInfo>(childEntity);

            if(childEntityBaseInfo.getChildren().empty())
            {
                if(ImGui::TreeNodeEx(formEntityName(childEntityBaseInfo, childEntity).c_str(), ImGuiTreeNodeFlags_Bullet))
                {
                    drawEntity(childEntity);

                    ImGui::TreePop();
                }
            }
            else
            {
                drawTreeNode(childEntity, false);
            }
        }

        ImGui::TreePop();
    }
}

void SGE::SceneTreeView::drawEntity(SGCore::ECS::entity_t entity) noexcept
{
    const auto inspectorView = SungearEngineEditor::getInstance()->getMainView()->getInspectorView();

    if(ImGui::IsItemClicked())
    {
        inspectorView->m_currentChosenEntity = entity;
        inspectorView->m_type = InspectorViewType::INSPECT_ENTITY;
    }

    DragNDropInfo info;
    // info.
}
