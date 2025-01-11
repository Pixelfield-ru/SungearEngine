//
// Created by stuka on 06.09.2024.
//

#include "SceneView.h"
#include "EditorScene.h"
#include "ImGuiUtils.h"
#include <imgui_internal.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Graphics/API/IFrameBuffer.h>
#include <SGCore/Input/InputManager.h>
#include <SGCore/Input/InputListener.h>
#include <SGCore/Memory/Assets/ModelAsset.h>
#include <SGCore/Utils/StringInterpolation/InterpolationResolver.h>
#include <SGCore/Render/Mesh.h>
#include <SGCore/Memory/Assets/Materials/IMaterial.h>
#include <SGCore/Render/RenderingBase.h>
#include <SGCore/Scene/EntityBaseInfo.h>
#include <SGCore/Render/Camera3D.h>
#include <SGCore/Scene/SceneUtils.h>
#include <SGCore/Memory/Assets/AnimationsFile.h>
#include <SGCore/Animation/MotionPlanner.h>

#include "SungearEngineEditor.h"
#include "Views/MainView.h"
#include "Views/Explorer/DirectoryExplorer.h"

bool SGE::SceneView::begin()
{
    return IView::begin();
}

void SGE::SceneView::renderBody()
{
    auto mainInputListener = SGCore::InputManager::getMainInputListener();

    SGCore::Ref<SGCore::Camera3D> editorCamera3D;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);

    ImGui::Begin("SceneView");


    const auto windowPos = ImGui::GetWindowPos();
    const auto windowSize = ImGui::GetWindowSize();

    // setting up imguizmo and drawing gizmo to manipulate entities
    m_entitiesManipulator.m_rectPos = windowPos;
    m_entitiesManipulator.m_rectSize = windowSize;

    auto currentEditorScene = EditorScene::getCurrentScene();
    if(currentEditorScene && currentEditorScene->m_scene)
    {

        /*auto tex = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>(
                "../SGResources/textures/skyboxes/skybox0/standard_skybox0_zfront.png"
        );*/
        auto* layeredFrameReceiver = currentEditorScene->m_scene->getECSRegistry()->tryGet<SGCore::LayeredFrameReceiver>
                (EditorScene::getCurrentScene()->m_data.m_editorCamera);

        auto* renderingBase = currentEditorScene->m_scene->getECSRegistry()->tryGet<SGCore::RenderingBase>
                (EditorScene::getCurrentScene()->m_data.m_editorCamera);

        auto* camera3D = currentEditorScene->m_scene->getECSRegistry()->tryGet<SGCore::Camera3D>
                (EditorScene::getCurrentScene()->m_data.m_editorCamera);

        if(layeredFrameReceiver && camera3D)
        {
            editorCamera3D = *camera3D;

            ImGui::Image(layeredFrameReceiver->m_layersFXFrameBuffer->getAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7
            )->getTextureNativeHandler(), ImGui::GetContentRegionAvail(), { 0, 1 }, { 1, 0 });

            if(editorCamera3D)
            {
                m_entitiesManipulator.manipulateEntities(*currentEditorScene->m_scene,
                                                         currentEditorScene->m_data.m_editorCamera);
            }

            const bool isMouseDoubleClicked = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);

            // picking entity
            if(mainInputListener->mouseButtonPressed(SGCore::MouseButton::MOUSE_BUTTON_LEFT) &&
               ImGui::IsWindowHovered() && !ImGuizmo::IsOver())
            {
                const glm::vec2 mousePos = { mainInputListener->getCursorPositionX(),
                                             mainInputListener->getCursorPositionY() };

                auto attachment2 = layeredFrameReceiver->m_layersFrameBuffer
                        ->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2);

                const glm::vec2 mouseRelativePos = {
                        (mousePos.x - windowPos.x) * (attachment2->getWidth() / windowSize.x),
                        attachment2->getHeight() - (mousePos.y - windowPos.y) * (attachment2->getHeight() / windowSize.y)
                };

                SGCore::ECS::entity_t pickedEntity =
                        SGCore::SceneUtils::pickEntity(mouseRelativePos,
                                                       *currentEditorScene->m_scene->getECSRegistry(),
                                                       layeredFrameReceiver->m_layersFrameBuffer.get(),
                                                       SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2
                                                       );

                auto& cameraPickedEntities = (*camera3D)->m_pickedEntities;

                auto leftControlDown = mainInputListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_LEFT_CONTROL);

                // picking
                if(pickedEntity == entt::null && !leftControlDown)
                {
                    cameraPickedEntities.clear();
                    m_entitiesManipulator.m_manipulatingEntities.clear();
                }
                else if(camera3D)
                {
                    auto& entityBaseInfo =
                            currentEditorScene->m_scene->getECSRegistry()->get<SGCore::EntityBaseInfo::reg_t>(pickedEntity);

                    std::vector<SGCore::ECS::entity_t> pickedEntities;
                    const auto rootEntity =
                            entityBaseInfo.getRootParent(*currentEditorScene->m_scene->getECSRegistry());

                    auto& rootBaseInfo =
                            currentEditorScene->m_scene->getECSRegistry()->get<SGCore::EntityBaseInfo::reg_t>(rootEntity);

                    rootBaseInfo.getAllChildren(*currentEditorScene->m_scene->getECSRegistry(),
                                                 pickedEntities
                    );

                    const bool containsRootEntity = m_entitiesManipulator.m_manipulatingEntities.contains(rootEntity);
                    const bool containsPickedEntity = m_entitiesManipulator.m_manipulatingEntities.contains(pickedEntity);

                    if(!leftControlDown)
                    {
                        cameraPickedEntities.clear();
                        m_entitiesManipulator.m_manipulatingEntities.clear();

                        if(containsRootEntity)
                        {
                            cameraPickedEntities.insert(pickedEntity);
                            m_entitiesManipulator.m_manipulatingEntities.insert(pickedEntity);
                            m_entitiesManipulator.m_isWholeModelPicking = false;

                            entityBaseInfo.m_outlineColor = SGCore::EntityBaseInfo::s_outlineColor0;
                        }
                        else
                        {
                            for(const auto& e : pickedEntities)
                            {
                                auto& pickedEntityBaseInfo =
                                        currentEditorScene->m_scene->getECSRegistry()->get<SGCore::EntityBaseInfo::reg_t>(e);

                                cameraPickedEntities.insert(e);

                                pickedEntityBaseInfo.m_outlineColor = SGCore::EntityBaseInfo::s_outlineColor1;
                            }
                            m_entitiesManipulator.m_manipulatingEntities.insert(rootEntity);
                            m_entitiesManipulator.m_isWholeModelPicking = true;
                        }
                    }
                    else
                    {
                        if(m_entitiesManipulator.m_isWholeModelPicking)
                        {
                            if(containsRootEntity)
                            {
                                for(const auto& e : pickedEntities)
                                {
                                    cameraPickedEntities.erase(e);
                                }

                                m_entitiesManipulator.m_manipulatingEntities.erase(rootEntity);
                            }
                            else
                            {
                                for(const auto& e : pickedEntities)
                                {
                                    cameraPickedEntities.insert(e);

                                    auto& pickedEntityBaseInfo =
                                            currentEditorScene->m_scene->getECSRegistry()->get<SGCore::EntityBaseInfo::reg_t>(e);

                                    pickedEntityBaseInfo.m_outlineColor = SGCore::EntityBaseInfo::s_outlineColor1;
                                }
                                m_entitiesManipulator.m_manipulatingEntities.insert(rootEntity);
                            }
                        }
                        else
                        {
                            if(containsPickedEntity)
                            {
                                cameraPickedEntities.erase(pickedEntity);
                                m_entitiesManipulator.m_manipulatingEntities.erase(pickedEntity);
                            }
                            else
                            {
                                auto& pickedEntityBaseInfo =
                                        currentEditorScene->m_scene->getECSRegistry()->get<SGCore::EntityBaseInfo::reg_t>(pickedEntity);

                                pickedEntityBaseInfo.m_outlineColor = SGCore::EntityBaseInfo::s_outlineColor0;

                                cameraPickedEntities.insert(pickedEntity);
                                m_entitiesManipulator.m_manipulatingEntities.insert(pickedEntity);
                            }
                        }
                    }
                }

                if(m_entitiesManipulator.m_manipulatingEntities.empty())
                {
                    m_entitiesManipulator.m_isWholeModelPicking = true;
                }

                LOG_I(SGEDITOR_TAG, "ENTITY PICKING: Mouse pos x = '{}', y = '{}'. Picked entity: '{}'",
                      mouseRelativePos.x,
                      mouseRelativePos.y,
                      std::to_underlying(pickedEntity));
            }

            acceptFilesFromDirectoryExplorer();
        }

        if(renderingBase)
        {
            if(windowSize.x != 0 && windowSize.y != 0)
            {
                (*renderingBase)->m_aspect = windowSize.x / windowSize.y;
            }
            if(layeredFrameReceiver)
            {
                /*layeredFrameReceiver->m_layersFrameBuffer->m_viewportWidth = windowSize.x;
                layeredFrameReceiver->m_layersFrameBuffer->m_viewportHeight = windowSize.y;
                layeredFrameReceiver->m_layersFrameBuffer->m_viewportPosX = windowPos.x;
                layeredFrameReceiver->m_layersFrameBuffer->m_viewportPosY = windowPos.y;*/
            }
            // glViewport(0, 0, windowSize.x, windowSize.y);
        }

        if(mainInputListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_LEFT_CONTROL) &&
           mainInputListener->keyboardKeyPressed(SGCore::KeyboardKey::KEY_B) &&
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

            if(fileExt == ".obj" || fileExt == ".gltf" || fileExt == ".fbx")
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

    if(!modelAsset->m_rootNode)
    {
        LOG_E(SGEDITOR_TAG, "Can not drop model asset on scene: model by path '{}' does not have nodes.",
              SGCore::Utils::toUTF8(modelPath.u16string()));
        return;
    }

    std::vector<SGCore::ECS::entity_t> entities;
    modelAsset->m_rootNode->addOnScene(SGCore::Scene::getCurrentScene(), SG_LAYER_OPAQUE_NAME, [&entities](const auto& entity) {
        entities.push_back(entity);
        auto* pickableComponent =
                SGCore::Scene::getCurrentScene()->getECSRegistry()->tryGet<SGCore::Pickable>(entity);
        if(pickableComponent)
        {
            pickableComponent->m_pickableForCameras.emplace_back(EditorScene::getCurrentScene()->m_data.m_editorCamera);
        }
    });

    // TEST OF ANIMATIONS !!!! ==========================================

    // animated_girl0
    // animated_cat1
    // ak47_reload

    if(modelAsset->getPath() != "${projectPath}\\Resources\\models\\ak47_reload\\scene.gltf") return;

    auto rootJointEntity = SGCore::Scene::getCurrentScene()
            ->getECSRegistry()
            ->get<SGCore::EntityBaseInfo>(entities[0])
            .findEntity(*SGCore::Scene::getCurrentScene()->getECSRegistry(), "_rootJoint");

    std::cout << "found entity root join: " << std::to_underlying(rootJointEntity) << std::endl;

    if(rootJointEntity != entt::null)
    {
        // TEST: getting skeleton of cat
        auto skeleton = SGCore::AssetManager::getInstance()->getAsset<SGCore::Skeleton>(
                SGCore::Utils::toUTF8(
                        SGCore::InterpolatedPath(
                                "${projectPath}\\Resources\\models\\ak47_reload\\scene.gltf\\skeletons\\_rootJoint"
                        )
                        .resolved()
                        .u16string()
                )
        );

        if(!skeleton)
        {
            std::cout << "can not get skeleton of cat!!" << std::endl;
            return;
        }

        // TEST: getting animations of cat
        auto animations = SGCore::AssetManager::getInstance()->getAsset<SGCore::AnimationsFile>(
                SGCore::Utils::toUTF8(
                        SGCore::InterpolatedPath(
                                "${projectPath}\\Resources\\models\\ak47_reload\\scene.gltf\\animations"
                        )
                        .resolved()
                        .u16string()
                )
        );

        if(!animations)
        {
            std::cout << "can not get animations of cat!!" << std::endl;
            return;
        }

        auto mainAnimation = animations->m_skeletalAnimations[0];

        auto& motionPlanner = SGCore::Scene::getCurrentScene()->getECSRegistry()->emplace<SGCore::MotionPlanner>(entities[0]);
        motionPlanner.m_skeleton = skeleton;

        auto mainNode = SGCore::MakeRef<SGCore::MotionPlannerNode>();
        mainNode->m_isRepeated = true;
        mainNode->m_animationSpeed = 0.1f;
        mainNode->m_skeletalAnimation = mainAnimation;

        motionPlanner.m_rootNodes.push_back(mainNode);
    }
}
