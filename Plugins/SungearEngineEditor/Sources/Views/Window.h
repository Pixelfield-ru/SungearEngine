//
// Created by Ilya on 18.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_WINDOW_H
#define SUNGEARENGINEEDITOR_WINDOW_H

#include <SGCore/ImGuiWrap/Views/IView.h>

namespace SGE
{
    struct Window : SGCore::ImGuiWrap::IView
    {
        bool begin() override;
        void end() override;

        bool m_isPopupWindow = false;

    protected:
        ImVec2 m_size { };
    };
}

#endif //SUNGEARENGINEEDITOR_WINDOW_H
