//
// Created by ilya on 23.06.24.
//

#include <imgui.h>
#include <imgui_internal.h>
#include "InspectorView.h"

#include <SGCore/Render/DebugDraw.h>
#include <SGCore/Render/RenderPipelinesManager.h>
#include <SGCore/Scene/Scene.h>
#include <SGCore/Physics/Rigidbody3D.h>
#include <SGCore/Physics/PhysicsWorld3D.h>
#include <SGCore/Render/Mesh.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Motion/MotionPlanner.h>
#include <SGCore/Motion/IK/IKJoint.h>
#include <SGCore/Motion/IK/IKRootJoint.h>
#include <SGCore/Render/Terrain/Terrain.h>

#include "SungearEngineEditor.h"

SGE::InspectorView::InspectorView() noexcept
{
    m_assetChooseWindow = SGCore::MakeRef<Window>();
    m_assetChooseWindow->m_name = "Choose Asset";
    m_assetChooseWindow->addButton(Button {
        .m_text = "Choose"
    });
    m_assetChooseWindow->addButton(Button {
        .m_text = "Cancel",
        .onClicked = [](Button& self, SGCore::ImGuiWrap::IView* parentView) {
            parentView->setActive(false);
        }
    });
    m_assetChooseWindow->setActive(false);

    addChild(m_assetChooseWindow);
}

bool SGE::InspectorView::begin()
{
    return IView::begin();
}

void SGE::InspectorView::renderBody()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);
    
    ImGui::Begin("InspectorView");

    if(ImGui::CollapsingHeader("Profiler (TO REMOVE)"))
    {
        ImGui::Text("Profiler (TO REMOVE)");

        auto currentRenderPipeline = SGCore::RenderPipelinesManager::instance().getCurrentRenderPipeline();

        ImGui::Text("Render (render pipeline '%s'):", typeid(*currentRenderPipeline).name());

        for(const auto& renderPass : currentRenderPipeline->m_renderPasses)
        {
            ImGui::Text("Render pass '%s' execution time: %f", typeid(*renderPass).name(), renderPass->getExecutionTime());
        }

        if(SGCore::Scene::getCurrentScene())
        {
            ImGui::Separator();

            ImGui::Text("Scene systems");

            for(const auto& system : SGCore::Scene::getCurrentScene()->getAllSystems())
            {
                ImGui::Text("System '%s'. 'update' execution time: %f, 'parallelUpdate' execution time: %f, 'fixedUpdate' execution time: %f",
                    typeid(*system).name(),
                    system->m_executionTimes["update"],
                    system->m_executionTimes["parallelUpdate"],
                    system->m_executionTimes["fixedUpdate"]);
            }
        }
    }

    ImGui::Separator();

    if(ImGui::CollapsingHeader("Engine Settings"))
    {
        auto debugRenderPass = SGCore::RenderPipelinesManager::instance().getCurrentRenderPipeline()->getRenderPass<SGCore::DebugDraw>();

        static const char* debugModes[] {
            "No debug", "Wireframe"
        };

        ImGui::Combo("Debug Draw Mode", (int*) &debugRenderPass->m_mode, debugModes, 2);

        auto currentScene = SGCore::Scene::getCurrentScene();

        if(currentScene)
        {
            const auto physicsWorldSystem = currentScene->getSystem<SGCore::PhysicsWorld3D>();

            if(ImGui::RadioButton("Enable physics simulation", physicsWorldSystem->m_simulate))
            {
                physicsWorldSystem->m_simulate = !physicsWorldSystem->m_simulate;

                const auto rigidbodies = currentScene->getECSRegistry()->view<SGCore::Rigidbody3D>();
                rigidbodies.each([&](auto& rigidbody) {
                    rigidbody->stop();
                });
            }
        }
    }

    ImGui::Separator();

    switch(m_type)
    {
        case InspectorViewType::INSPECT_ENTITY:
        {
            inspectEntity();
            break;
        }
        case InspectorViewType::INSPECT_MATERIAL:
        {
            inspectMaterial();
            break;
        }
    }

    ImGui::End();

    ImGui::PopStyleVar(2);
}

void SGE::InspectorView::end()
{
    IView::end();
}

void SGE::InspectorView::inspectEntity() const noexcept
{
    if(m_currentChosenEntity != entt::null && SGCore::Scene::getCurrentScene())
    {
        auto ecsRegistry = SGCore::Scene::getCurrentScene()->getECSRegistry();
        auto currentScene = SGCore::Scene::getCurrentScene();
        const auto inspectorView = SungearEngineEditor::getInstance()->getMainView()->getInspectorView();
        const auto mainAssetManager = SGCore::AssetManager::getInstance();

        ImGui::Text("Entity: %d", m_currentChosenEntity);

        if(ImGui::CollapsingHeader("Components"))
        {
            const auto entityUnderlying = std::to_underlying(m_currentChosenEntity);

            auto& entityBaseInfo = ecsRegistry->get<SGCore::EntityBaseInfo>(m_currentChosenEntity);
            if(ImGui::CollapsingHeader("Entity Base Info"))
            {
                ImGui::Text("Name: %s", entityBaseInfo.getName().c_str());
                if(entityBaseInfo.getParent() != entt::null)
                {
                    auto& parentBaseInfo = ecsRegistry->get<SGCore::EntityBaseInfo>(entityBaseInfo.getParent());
                    ImGui::Text("Parent: %llu (name: %s)", entityBaseInfo.getParent(), parentBaseInfo.getName().c_str());
                }
                else
                {
                    ImGui::Text("Parent: null");
                }
            }

            auto* tmpTransform = ecsRegistry->tryGet<SGCore::Transform>(m_currentChosenEntity);
            if(tmpTransform && ImGui::CollapsingHeader("Transform"))
            {
                auto& transform = *tmpTransform;

                ImGui::DragFloat3("Position", &transform->m_ownTransform.m_position.x);
                ImGui::DragFloat3("Scale", &transform->m_ownTransform.m_scale.x);

                glm::vec3 euler = glm::degrees(glm::eulerAngles(transform->m_ownTransform.m_rotation));
                if(ImGui::DragFloat3("Rotation", &euler.x))
                {
                    transform->m_ownTransform.m_rotation = glm::quat(glm::radians(euler));
                }
            }

            auto* mesh = ecsRegistry->tryGet<SGCore::Mesh>(m_currentChosenEntity);
            if(mesh && ImGui::CollapsingHeader("Mesh"))
            {
                if(ImGui::Button("View Material"))
                {
                    inspectorView->m_currentMaterial = mesh->m_base.getMaterial();
                    inspectorView->m_type = InspectorViewType::INSPECT_MATERIAL;
                }
            }

            auto* tmpRigidbody3D = ecsRegistry->tryGet<SGCore::Rigidbody3D>(m_currentChosenEntity);
            if(tmpRigidbody3D && ImGui::CollapsingHeader("Rigidbody3D"))
            {
                auto rigidbody = *tmpRigidbody3D;

                ImGui::BeginGroup();

                ImGui::Text("Rigidbody type");

                if(ImGui::RadioButton(fmt::format("Static {}", entityUnderlying).c_str(), rigidbody->getType() == SGCore::PhysicalObjectType::OT_STATIC))
                {
                    rigidbody->removeFromWorld();
                    rigidbody->setType(SGCore::PhysicalObjectType::OT_STATIC);
                    rigidbody->stop();
                    rigidbody->reAddToWorld();
                }
                if(ImGui::RadioButton(fmt::format("Dynamic {}", entityUnderlying).c_str(), rigidbody->getType() == SGCore::PhysicalObjectType::OT_DYNAMIC))
                {
                    rigidbody->setType(SGCore::PhysicalObjectType::OT_DYNAMIC);
                    rigidbody->m_body->setActivationState(DISABLE_DEACTIVATION);
                    rigidbody->reAddToWorld();
                    // rigidbody->reAddToWorld();
                }
                if(ImGui::RadioButton(fmt::format("Kinematic {}", entityUnderlying).c_str(), rigidbody->getType() == SGCore::PhysicalObjectType::OT_KINEMATIC))
                {
                    rigidbody->removeFromWorld();
                    rigidbody->setType(SGCore::PhysicalObjectType::OT_KINEMATIC);
                    rigidbody->stop();
                    rigidbody->reAddToWorld();
                }

                ImGui::EndGroup();

                ImGui::Text("Body mass");
                ImGui::SameLine();
                btScalar bodyMass = rigidbody->m_body->getMass();
                if(ImGui::DragFloat(fmt::format("##BodyMass_{}", entityUnderlying).c_str(), &bodyMass))
                {
                    rigidbody->removeFromWorld();

                    btVector3 localInertia(0, 0, 0);
                    rigidbody->m_body->getCollisionShape()->calculateLocalInertia(bodyMass, localInertia);
                    rigidbody->m_body->setMassProps(bodyMass, localInertia);
                    rigidbody->m_body->updateInertiaTensor();
                    // rigidbody->m_body->activate();

                    if(bodyMass == 0.0f)
                    {
                        rigidbody->stop();
                    }

                    rigidbody->reAddToWorld();
                    // rigidbody->reAddToWorld();
                    /*for(const auto& shape : rigidbody->getShapes())
                    {
                        btVector3 shapeInertia;
                        shape->calculateLocalInertia(bodyMass, shapeInertia);
                    }*/
                }

                for(size_t i = 0; i < rigidbody->getShapesCount(); ++i)
                {
                    if(ImGui::CollapsingHeader(fmt::format("Shape {}", i).c_str()))
                    {
                        auto& shape = rigidbody->getShapes()[i];
                        auto& shapeTransform = rigidbody->getShapeTransform(i);
                        ImGui::Text("Shape pos");
                        if(ImGui::DragFloat3(fmt::format("##ShapePos_{}_{}", entityUnderlying, i).c_str(), shapeTransform.getOrigin().m_floats, 0.1f))
                        {
                            rigidbody->updateShapeTransform(0);
                        }

                        if(shape->getShapeType() == BOX_SHAPE_PROXYTYPE)
                        {
                            auto* boxShape = static_cast<btBoxShape*>(shape.get());

                            auto boxLocalScaling = boxShape->getLocalScaling();
                            if(ImGui::DragFloat3(fmt::format("##BoxShapeSize_{}_{}", entityUnderlying, i).c_str(), boxLocalScaling.m_floats, 0.1f, 0.001f))
                            {
                                if(boxLocalScaling.x() <= 0.0f)
                                {
                                    boxLocalScaling.setX(0.0001f);
                                }

                                if(boxLocalScaling.y() <= 0.0f)
                                {
                                    boxLocalScaling.setY(0.0001f);
                                }

                                if(boxLocalScaling.z() <= 0.0f)
                                {
                                    boxLocalScaling.setZ(0.0001f);
                                }

                                boxShape->setLocalScaling(boxLocalScaling);
                                rigidbody->updateShapeTransform(0);
                            }
                        }
                        // ImGui::DragFloat3("Shape rotation", shapeTransform.getRotation().m_floats);
                    }
                }

                if(ImGui::Button("Add Box Shape"))
                {
                    auto boxShape = SGCore::MakeRef<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f));
                    btTransform boxTransform;
                    boxTransform.setIdentity();
                    rigidbody->addShape(boxTransform, boxShape);
                }
            }

            auto* ikJoint = ecsRegistry->tryGet<SGCore::IKJoint>(m_currentChosenEntity);
            if(ikJoint && ImGui::CollapsingHeader("IKJoint"))
            {
                glm::vec3 dest = ikJoint->m_targetPosition ? *ikJoint->m_targetPosition : glm::vec3();
                if(ImGui::DragFloat3("Destination", &dest.x, 0.01))
                {
                    ikJoint->m_targetPosition = dest;
                }

                ImGui::Checkbox("End Joint", &ikJoint->m_isEndJoint);
            }

            auto* ikRootJoint = ecsRegistry->tryGet<SGCore::IKRootJoint>(m_currentChosenEntity);
            if(ikRootJoint && ImGui::CollapsingHeader("IKRootJoint"))
            {

            }

            auto* motionPlanner = ecsRegistry->tryGet<SGCore::MotionPlanner>(m_currentChosenEntity);
            if(motionPlanner && ImGui::CollapsingHeader("MotionPlanner"))
            {
                const auto skeletons = mainAssetManager->getAssetsWithType<SGCore::Skeleton>();
                const auto entitySkeleton = motionPlanner->m_skeleton;

                if(ImGui::BeginCombo("Skeleton", entitySkeleton ? entitySkeleton->getRootBone()->getName().c_str() : "Not chosen"))
                {
                    for(size_t i = 0; i < skeletons.size(); ++i)
                    {
                        const bool isSelected = (entitySkeleton == skeletons[i]);
                        if(ImGui::Selectable(SGCore::Utils::toUTF8(skeletons[i]->getPath().raw().u32string()).c_str(), isSelected))
                        {
                            motionPlanner->m_skeleton = skeletons[i];
                        }

                        if(isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo();
                }
            }

            auto* terrain = ecsRegistry->tryGet<SGCore::Terrain>(m_currentChosenEntity);
            if(terrain && ImGui::CollapsingHeader("Terrain"))
            {
                auto* mesh = ecsRegistry->tryGet<SGCore::Mesh>(m_currentChosenEntity);
                if(mesh)
                {
                    if(ImGui::Button("Generate"))
                    {
                        SGCore::Terrain::generate(*terrain, mesh->m_base.getMeshData(), 10, 10, 10);
                    }
                }
            }
        }

        if(ImGui::Button("Add Transform"))
        {
            if(!ecsRegistry->allOf<SGCore::Transform>(m_currentChosenEntity))
            {
                ecsRegistry->emplace<SGCore::Transform>(m_currentChosenEntity);
            }
        }

        if(ImGui::Button("Add Rigidbody3D"))
        {
            if(!ecsRegistry->allOf<SGCore::Rigidbody3D>(m_currentChosenEntity))
            {
                ecsRegistry->emplace<SGCore::Rigidbody3D>(m_currentChosenEntity, SGCore::MakeRef<SGCore::Rigidbody3D>(currentScene->getSystem<SGCore::PhysicsWorld3D>()));
            }
        }

        if(ImGui::Button("Add IKJoint"))
        {
            if(!ecsRegistry->allOf<SGCore::IKJoint>(m_currentChosenEntity))
            {
                ecsRegistry->emplace<SGCore::IKJoint>(m_currentChosenEntity);
            }
        }

        if(ImGui::Button("Add IKRootJoint"))
        {
            if(!ecsRegistry->allOf<SGCore::IKRootJoint>(m_currentChosenEntity))
            {
                ecsRegistry->emplace<SGCore::IKRootJoint>(m_currentChosenEntity);
            }
        }

        if(ImGui::Button("Add MotionPlanner"))
        {
            if(!ecsRegistry->allOf<SGCore::MotionPlanner>(m_currentChosenEntity))
            {
                ecsRegistry->emplace<SGCore::MotionPlanner>(m_currentChosenEntity);
            }
        }

        if(ImGui::Button("Add Terrain"))
        {
            if(!ecsRegistry->allOf<SGCore::Terrain>(m_currentChosenEntity))
            {
                ecsRegistry->emplace<SGCore::Terrain>(m_currentChosenEntity);
            }
        }

        if(ImGui::CollapsingHeader("Danger zone"))
        {
            if(ImGui::Button("Delete entity"))
            {
                ecsRegistry->get<SGCore::EntityBaseInfo>(m_currentChosenEntity).destroy(*ecsRegistry);
            }
        }
    }
}

void SGE::InspectorView::inspectMaterial() const noexcept
{
    if(!m_currentMaterial) return;

    if(ImGui::CollapsingHeader("Textures"))
    {
        for(size_t i = 0; i < m_currentMaterial->getTextures().size(); ++i)
        {
            const auto& texturesTyped = m_currentMaterial->getTextures()[i];
            const SGTextureSlot textureType = static_cast<SGTextureSlot>(i);
            const std::string textureTypeString = sgStandardTextureTypeToString(textureType);

            if(ImGui::CollapsingHeader(textureTypeString.c_str()))
            {
                for(size_t j = 0; j < texturesTyped.size(); ++j)
                {
                    const auto& texture = texturesTyped[j];

                    ImGui::Text(SGCore::Utils::toUTF8(texture->getPath().resolved().u16string()).c_str());
                    ImGui::Image((ImTextureID) texture->getTextureNativeHandler(), ImVec2(texture->getWidth(), texture->getHeight()));
                }

                ImGui::PushID((textureTypeString + "_" + SGCore::Utils::toUTF8(m_currentMaterial->getPath().raw().u16string())).c_str());
                if(ImGui::Button("Add texture"))
                {
                    const auto textures = SGCore::AssetManager::getInstance()->getAssetsWithType<SGCore::ITexture2D>();

                    for(const auto& texture : textures)
                    {
                        m_assetChooseWindow->setActive(true);
                        // if(ImGui::Button("Choose"))
                    }
                }
                ImGui::PopID();
            }
        }
    }
}
