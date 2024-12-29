//
// Created by ilya on 23.06.24.
//

#ifndef SUNGEARENGINEEDITOR_INSPECTORVIEW_H
#define SUNGEARENGINEEDITOR_INSPECTORVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>

namespace SGE
{
    struct InspectorView : public SGCore::ImGuiWrap::IView
    {
        SGCore::ECS::entity_t m_currentChosenEntity { };

        bool begin() final;
        void renderBody() final;
        void end() final;
    };
}

#endif //SUNGEARENGINEEDITOR_INSPECTORVIEW_H
