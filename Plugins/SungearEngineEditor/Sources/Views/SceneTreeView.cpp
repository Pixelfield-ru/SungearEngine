//
// Created by ilya on 23.06.24.
//

#include <print>

#include <imgui.h>
#include <imgui_internal.h>
#include <SGCore/Render/LayeredFrameReceiver.h>
#include <SGCore/Graphics/API/IFrameBuffer.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Memory/Assets/SkeletalAnimationAsset.h>
#include <SGCore/Scene/RootEntityTag.h>
#include <SGCore/Transformations/TransformUtils.h>

#include "SceneTreeView.h"
#include "EditorScene.h"
#include "ImGuiUtils.h"
#include "SungearEngineEditor.h"

SGE::SceneTreeView::SceneTreeView() noexcept
{
    m_popup.onElementClicked += [this](const SGCore::Ref<PopupElement>& element) {
        auto currentScene = EditorScene::getCurrentScene()->m_scene;
        auto registry = currentScene->getECSRegistry();

        if(element->m_ID == "NewEntityAction")
        {
            const auto newEntity = registry->create();
            auto& newBaseInfo = registry->get<SGCore::EntityBaseInfo>(newEntity);
            newBaseInfo.setParent(m_rightClickedEntity, *registry);
        }
        else if(element->m_ID == "DeleteEntityAction")
        {
            auto& newBaseInfo = registry->get<SGCore::EntityBaseInfo>(m_rightClickedEntity);
            newBaseInfo.destroy(*registry);
        }
    };
}

bool SGE::SceneTreeView::begin()
{
    return IView::begin();
}

void SGE::SceneTreeView::renderBody()
{
    m_isTreeRightClicked = false;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = (ImGuiDockNodeFlags_) ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_AutoHideTabBar;
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

            ImGui::Image((ImTextureID) attachment0->getTextureNativeHandler(),
                         {attachment0->getWidth() / compressionCoeff, attachment0->getHeight() / compressionCoeff},
                         {0, 1}, {1, 0}
            );

            const auto& attachment1 = layeredFrameReceiver->m_layersFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1
            );

            ImGui::Image((ImTextureID) attachment1->getTextureNativeHandler(),
                         {attachment1->getWidth() / compressionCoeff, attachment1->getHeight() / compressionCoeff},
                         {0, 1}, {1, 0}
            );

            const auto& attachment2 = layeredFrameReceiver->m_layersFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2
            );

            ImGui::Image((ImTextureID) attachment2->getTextureNativeHandler(),
                         {attachment2->getWidth() / compressionCoeff, attachment2->getHeight() / compressionCoeff},
                         {0, 1}, {1, 0}
            );

            const auto& attachment3 = layeredFrameReceiver->m_layersFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3
            );

            ImGui::Image((ImTextureID) attachment3->getTextureNativeHandler(),
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

            ImGui::Image((ImTextureID) attachment31->getTextureNativeHandler(),
                         {attachment31->getWidth() / compressionCoeff, attachment31->getHeight() / compressionCoeff},
                         {0, 1}, {1, 0}
            );
        }

        auto entitiesView = currentEditorScene->m_scene->getECSRegistry()->view<SGCore::EntityBaseInfo, SGCore::RootEntityTag>();

        const bool sceneNodeOpened = ImGui::TreeNodeEx((currentEditorScene->m_scene->m_metaInfo.m_sceneName).c_str(), ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow);
        drawEntity(entt::null);
        acceptDragNDrop(entt::null, DragNDropType::TARGET);

        if(sceneNodeOpened)
        {
            acceptDragNDrop(entt::null, DragNDropType::TARGET);

            for(const auto& e : entitiesView)
            {
                drawTreeNode(e);
            }

            ImGui::TreePop();
        }

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

    if(m_isTreeRightClicked)
    {
        m_popup.setOpened(true);
    }
    m_popup.draw();

    ImGui::End();
    
    ImGui::PopStyleVar(2);
}

void SGE::SceneTreeView::end()
{
    IView::end();
}

void SGE::SceneTreeView::drawTreeNode(SGCore::ECS::entity_t entity) noexcept
{
    static auto formEntityName = [](const SGCore::EntityBaseInfo& entityBaseInfo, const SGCore::ECS::entity_t& entity) {
        return entityBaseInfo.getName() + " (entity: " + std::to_string(std::to_underlying(entity)) + ")";
    };

    const auto& currentScene = EditorScene::getCurrentScene()->m_scene;

    auto& entityBaseInfo = currentScene->getECSRegistry()->get<SGCore::EntityBaseInfo>(entity);

    bool treeNodeOpened = false;
    if(!entityBaseInfo.getChildren().empty())
    {
        treeNodeOpened = ImGui::TreeNodeEx(formEntityName(entityBaseInfo, entity).c_str(), ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow);
    }
    else
    {
        treeNodeOpened = ImGui::TreeNodeEx(formEntityName(entityBaseInfo, entity).c_str(), ImGuiTreeNodeFlags_Bullet);
    }

    drawEntity(entity);
    acceptDragNDrop(entity, DragNDropType::BOTH);

    if(!entityBaseInfo.getChildren().empty())
    {
        if(treeNodeOpened)
        {
            for(const auto& childEntity : entityBaseInfo.getChildren())
            {
                drawTreeNode(childEntity);
            }

            ImGui::TreePop();
        }
    }
    else if(treeNodeOpened)
    {
        ImGui::TreePop();
    }
}

void SGE::SceneTreeView::drawEntity(SGCore::ECS::entity_t entity) noexcept
{
    const auto inspectorView = SungearEngineEditor::getInstance()->getMainView()->getInspectorView();

    if(ImGui::IsItemClicked())
    {
        inspectorView->setChosenEntity(entity);
        inspectorView->m_type = InspectorViewType::INSPECT_ENTITY;
    }

    if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        LOG_I(SGEDITOR_TAG, "Righted clicked entity {}", std::to_underlying(entity));

        m_rightClickedEntity = entity;
        m_isTreeRightClicked = true;
    }
}

void SGE::SceneTreeView::acceptDragNDrop(SGCore::ECS::entity_t entity, DragNDropType dndType) noexcept
{
    DragNDropInfo info;
    info.m_type = dndType;
    info.m_payloadProcessFunction = [entity](const ImGuiPayload* payload) {
        const auto droppedEntity = *reinterpret_cast<SGCore::ECS::entity_t*>(payload->Data);

        const auto scene = EditorScene::getCurrentScene()->m_scene;

        auto& entityBaseInfo = scene->getECSRegistry()->get<SGCore::EntityBaseInfo>(droppedEntity);

        auto* parentTransform = scene->getECSRegistry()->tryGet<SGCore::Transform>(entity);

        if(parentTransform)
        {
            auto& entityTransform = scene->getECSRegistry()->get<SGCore::Transform>(droppedEntity);

            entityTransform.m_localTransform.m_position = SGCore::TransformUtils::calculateLocalPosition(*parentTransform, entityTransform.m_worldTransform.m_position);
            entityTransform.m_localTransform.m_rotation = SGCore::TransformUtils::calculateLocalRotation(*parentTransform, entityTransform.m_worldTransform.m_rotation);
        }
        // entityTransform.m_localTransform.m_scale = SGCore::TransformUtils::calculateLocalScale(parentTransform, entityTransform.m_worldTransform.m_scale);

        entityBaseInfo.setParent(entity, *scene->getECSRegistry());

        LOG_I(SGEDITOR_TAG, "Dropped entity {} into entity {}", std::to_underlying(droppedEntity), std::to_underlying(entity));
    };
    info.m_name = "EntityDragNDrop";
    info.m_flags = ImGuiDragDropFlags_None;
    info.m_drawSourceFunction = [entity]() {
        ImGui::Text(("Drop Entity " + std::to_string(std::to_underlying(entity))).c_str());
    };
    info.m_data = &entity;
    info.m_dataSize = sizeof(entity);

    ImGuiUtils::UseDragNDrop(&info);
}
