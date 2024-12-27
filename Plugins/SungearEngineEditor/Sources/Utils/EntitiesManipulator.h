//
// Created by stuka on 23.12.2024.
//

#ifndef SUNGEARENGINEEDITOR_ENTITIESMANIPULATOR_H
#define SUNGEARENGINEEDITOR_ENTITIESMANIPULATOR_H

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>
#include <SGCore/Scene/Scene.h>
#include <SGCore/Render/RenderingBase.h>
#include <SGCore/Transformations/Transform.h>
#include <SGCore/Scene/EntityBaseInfo.h>

namespace SGE
{
    struct EntitiesManipulator
    {
        bool m_isOrtho = false;
        ImVec2 m_rectPos { };
        ImVec2 m_rectSize { };

        ImGuizmo::MODE m_mode = ImGuizmo::MODE::WORLD;

        ImGuizmo::OPERATION m_manipulationOperation =
                ImGuizmo::OPERATION::TRANSLATE |
                ImGuizmo::OPERATION::ROTATE |
                ImGuizmo::OPERATION::SCALE;

        std::unordered_set<SGCore::entity_t> m_manipulatingEntities;

        bool m_isWholeModelPicking = true;

        template<typename ContainerT>
        void collectEntities(const ContainerT& entitiesContainer) noexcept
        {
            for(const auto& e : entitiesContainer)
            {
                m_manipulatingEntities.insert(e);
            }
        }

        void manipulateEntities(const SGCore::Scene& forScene,
                                const SGCore::entity_t& camera3DEntity) const noexcept;

    private:
        static void getDeltaBetweenMatrices(const glm::mat4& originalMatrix, const glm::mat4& updatedMatrix,
                                            glm::vec3& deltaTranslation, glm::quat& deltaRotation, glm::vec3& deltaScale) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_ENTITIESMANIPULATOR_H
