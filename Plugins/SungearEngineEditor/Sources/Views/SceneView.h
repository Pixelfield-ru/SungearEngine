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
    };
}

#endif //SUNGEARENGINEEDITOR_SCENEVIEW_H
