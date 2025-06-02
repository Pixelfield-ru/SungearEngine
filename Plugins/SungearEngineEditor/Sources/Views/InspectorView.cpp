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

#include <BulletCollision/CollisionShapes/btBoxShape.h>

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

        auto currentRenderPipeline = SGCore::RenderPipelinesManager::getCurrentRenderPipeline();

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
        auto debugRenderPass = SGCore::RenderPipelinesManager::getCurrentRenderPipeline()->getRenderPass<SGCore::DebugDraw>();

        static const char* debugModes[] {
            "No debug", "Wireframe"
        };

        ImGui::Combo("Debug Draw Mode", (int*) &debugRenderPass->m_mode, debugModes, 2);
    }

    ImGui::Separator();

    if(m_currentChosenEntity != entt::null && SGCore::Scene::getCurrentScene())
    {
        auto ecsRegistry = SGCore::Scene::getCurrentScene()->getECSRegistry();
        auto currentScene = SGCore::Scene::getCurrentScene();

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

            auto* tmpRigidbody3D = ecsRegistry->tryGet<SGCore::Rigidbody3D>(m_currentChosenEntity);
            if(tmpRigidbody3D && ImGui::CollapsingHeader("Rigidbody3D"))
            {
                auto& rigidbody = *tmpRigidbody3D;

                ImGui::BeginGroup();

                ImGui::Text("Rigidbody type");

                if(ImGui::RadioButton("Static", rigidbody->getType() == SGCore::PhysicalObjectType::OT_STATIC))
                {
                    rigidbody->setType(SGCore::PhysicalObjectType::OT_STATIC);
                }
                if(ImGui::RadioButton("Dynamic", rigidbody->getType() == SGCore::PhysicalObjectType::OT_DYNAMIC))
                {
                    rigidbody->setType(SGCore::PhysicalObjectType::OT_DYNAMIC);
                    rigidbody->reAddToWorld();
                }
                if(ImGui::RadioButton("Kinematic", rigidbody->getType() == SGCore::PhysicalObjectType::OT_KINEMATIC))
                {
                    rigidbody->setType(SGCore::PhysicalObjectType::OT_KINEMATIC);
                }

                ImGui::EndGroup();

                ImGui::Text("Body mass");
                ImGui::SameLine();
                btScalar bodyMass = rigidbody->m_body->getMass();
                if(ImGui::DragFloat(fmt::format("##BodyMass_{}", entityUnderlying).c_str(), &bodyMass))
                {
                    btVector3 localInertia(0, 0, 0);
                    rigidbody->m_body->getCollisionShape()->calculateLocalInertia(bodyMass, localInertia);
                    rigidbody->m_body->setMassProps(bodyMass, localInertia);
                    rigidbody->reAddToWorld();
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
                        ImGui::DragFloat3(fmt::format("##ShapePos_{}_{}", entityUnderlying, i).c_str(), shapeTransform.getOrigin().m_floats);
                        // ImGui::DragFloat3("Shape rotation", shapeTransform.getRotation().m_floats);
                    }
                }

                if(ImGui::Button("Add Box Shape"))
                {
                    auto boxShape = SGCore::MakeRef<btBoxShape>(btVector3(1, 1, 1));
                    btTransform boxTransform;
                    boxTransform.setIdentity();
                    rigidbody->addShape(boxTransform, boxShape);
                    rigidbody->reAddToWorld();
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
    }

    ImGui::End();

    ImGui::PopStyleVar(2);
}

void SGE::InspectorView::end()
{
    IView::end();
}
