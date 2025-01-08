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

        ImGui::TreePush("Skeletons");

        auto skeletonsAssets = SGCore::AssetManager::getInstance()->getAssetsWithType<SGCore::Skeleton>();

        for(const auto& skeleton : skeletonsAssets)
        {
            drawSkeletonNode(skeleton->m_rootBone);
        }

        ImGui::TreePop();

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
    auto& entityTransform = currentScene->getECSRegistry()->get<SGCore::Transform>(parentEntity);
    auto* entityMesh = currentScene->getECSRegistry()->tryGet<SGCore::Mesh>(parentEntity);

    if((entityBaseInfo.getParent() == entt::null || !checkForRoot) &&
       ImGui::TreeNode(formEntityName(entityBaseInfo, parentEntity).c_str()))
    {
        {
            auto euler = glm::eulerAngles(entityTransform->m_ownTransform.m_rotation);
            auto scale = entityTransform->m_ownTransform.m_scale;
            auto position = entityTransform->m_ownTransform.m_position;
            ImGui::Text(fmt::format("Position: {}, {}, {}", position.x, position.y, position.z).c_str());
            ImGui::Text(fmt::format("Rotation: {}, {}, {}", glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z)).c_str());
            ImGui::Text(fmt::format("Scale: {}, {}, {}", scale.x, scale.y, scale.z).c_str());
        }

        for(const auto& childEntity : entityBaseInfo.getChildren())
        {
            auto& childEntityBaseInfo = currentScene->getECSRegistry()->get<SGCore::EntityBaseInfo>(childEntity);
            auto& childEntityTransform = currentScene->getECSRegistry()->get<SGCore::Transform>(childEntity);
            auto* childEntityMesh = currentScene->getECSRegistry()->tryGet<SGCore::Mesh>(childEntity);

            if(childEntityBaseInfo.getChildren().empty())
            {
                if(ImGui::TreeNodeEx(formEntityName(childEntityBaseInfo, childEntity).c_str(), ImGuiTreeNodeFlags_Bullet))
                {
                    auto euler = glm::eulerAngles(childEntityTransform->m_ownTransform.m_rotation);
                    auto scale = childEntityTransform->m_ownTransform.m_scale;
                    auto position = childEntityTransform->m_ownTransform.m_position;
                    ImGui::Text(fmt::format("Position: {}, {}, {}", position.x, position.y, position.z).c_str());
                    ImGui::Text(fmt::format("Rotation: {}, {}, {}", glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z)).c_str());
                    ImGui::Text(fmt::format("Scale: {}, {}, {}", scale.x, scale.y, scale.z).c_str());

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

void SGE::SceneTreeView::drawSkeletonNode(const SGCore::AssetRef<SGCore::Bone>& parentBone) noexcept
{
    ImGui::PushID(std::hash<decltype(parentBone.get())>()(parentBone.get()));

    if(ImGui::TreeNode(parentBone->m_boneName.c_str()))
    {
        const auto& offsetMatrix = parentBone->m_offsetMatrix;
        ImGui::Text("Offset matrix: %s", glm::to_string(offsetMatrix).c_str());

        ImGui::PushID(std::hash<decltype(&parentBone->m_affectedMeshesBoneData)>()(&parentBone->m_affectedMeshesBoneData));

        if(!parentBone->m_affectedMeshesBoneData.empty() && ImGui::TreeNode("Data"))
        {
            for(const auto& affectedMeshBoneData: parentBone->m_affectedMeshesBoneData)
            {
                const auto& affectedMesh = affectedMeshBoneData.m_affectedMesh;
                const auto& weights = affectedMeshBoneData.m_weights;

                ImGui::PushID(std::hash<decltype(&affectedMesh)>()(&affectedMesh));

                if(ImGui::TreeNode("For mesh %s", affectedMesh->m_name.c_str()))
                {
                    ImGui::Text("Affected mesh name: %s", affectedMesh->m_name.c_str());

                    ImGui::PushID(std::hash<decltype(&weights)>()(&weights));

                    if(ImGui::TreeNode("Weights"))
                    {
                        for(const auto& weight : weights)
                        {
                            ImGui::Text("Weight = %f, Vertex ID = %i", weight.m_weight, weight.m_vertexIdx);
                        }

                        ImGui::TreePop();
                    }

                    ImGui::PopID();

                    ImGui::TreePop();
                }

                ImGui::PopID();
            }

            ImGui::TreePop();
        }

        ImGui::PopID();

        ImGui::PushID(std::hash<decltype(&parentBone->m_children)>()(&parentBone->m_children));

        if(!parentBone->m_children.empty() && ImGui::TreeNode("Children"))
        {
            for(const auto& child: parentBone->m_children)
            {
                drawSkeletonNode(child);
            }

            ImGui::TreePop();
        }

        ImGui::PopID();

        ImGui::TreePop();
    }

    ImGui::PopID();
}
