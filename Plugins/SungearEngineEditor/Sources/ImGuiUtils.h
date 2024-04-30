//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_IMGUIUTILS_H
#define SUNGEARENGINEEDITOR_IMGUIUTILS_H

#include <imgui.h>

namespace SGE
{
    struct ImGuiUtils
    {
        static bool ImageButton(void* imageNativeHandler, const ImVec2& imageSize,
                                const ImVec2& hoverMinOffset = ImVec2(-3, -3),
                                const ImVec2& hoverMaxOffset = ImVec2(3, 3),
                                const ImVec4& hoverBgColor = ImVec4(0.3, 0.3, 0.3, 0.3)) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_IMGUIUTILS_H
