//
// Created by stuka on 06.09.2024.
//

#ifndef SUNGEARENGINEEDITOR_SCENEVIEW_H
#define SUNGEARENGINEEDITOR_SCENEVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>

namespace SGE
{
    struct SceneView : public SGCore::ImGuiWrap::IView
    {
        bool begin() final;
        void renderBody() final;
        void end() final;

    private:
        // accepts files that are drag n dropped from directory explorer
        void acceptFilesFromDirectoryExplorer() noexcept;

        void loadModelByPath(const std::filesystem::path& modelPath) const noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_SCENEVIEW_H
