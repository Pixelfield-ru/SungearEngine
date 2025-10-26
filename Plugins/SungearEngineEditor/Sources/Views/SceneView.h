//
// Created by stuka on 06.09.2024.
//

#ifndef SUNGEARENGINEEDITOR_SCENEVIEW_H
#define SUNGEARENGINEEDITOR_SCENEVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/Render/Camera3D.h>
#include <imgui.h>
#include <ImGuizmo/ImGuizmo.h>

#include "Utils/EntitiesManipulator.h"

namespace SGE
{
    struct EditorScene;

    struct SceneView : public SGCore::ImGuiWrap::IView
    {
        EntitiesManipulator m_entitiesManipulator;

        bool begin() final;
        void renderBody() final;
        void end() final;

    private:
        // serialized copied entities
        std::string m_copiedEntities;

        void addPastedEntityToEditorCameraAsPickable(SGCore::ECS::entity_t entity, EditorScene& editorScene);

        // accepts files that are drag n dropped from directory explorer
        void acceptFilesFromDirectoryExplorer() noexcept;

        void loadModelByPath(const std::filesystem::path& modelPath) const noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_SCENEVIEW_H
