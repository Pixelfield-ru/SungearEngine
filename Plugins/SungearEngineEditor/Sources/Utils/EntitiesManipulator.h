//
// Created by stuka on 23.12.2024.
//

#ifndef SUNGEARENGINEEDITOR_ENTITIESMANIPULATOR_H
#define SUNGEARENGINEEDITOR_ENTITIESMANIPULATOR_H

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>
#include <SGCore/Render/Camera3D.h>
#include <SGCore/Scene/Scene.h>

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


        void manipulateEntities(const SGCore::Scene& forScene,
                                const SGCore::entity_t& camera3DEntity,
                                const SGCore::Camera3D& forCamera3D) noexcept;

    private:
        static void getDeltaBetweenMatrices(const glm::mat4& originalMatrix, const glm::mat4& updatedMatrix,
                                            glm::vec3& deltaTranslation, glm::quat& deltaRotation, glm::vec3& deltaScale) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_ENTITIESMANIPULATOR_H
