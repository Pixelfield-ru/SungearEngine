//
// Created by Ilya on 18.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_WINDOW_H
#define SUNGEARENGINEEDITOR_WINDOW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "Button.h"

namespace SGE
{
    struct Window : SGCore::ImGuiWrap::IView
    {
        ImVec2 m_bodyPadding { 7, 8 };

        bool begin() override;
        void end() override;
        virtual void postRenderBody() { };
        virtual void footerRender() { };

        void onActiveChangedListener() override;

        bool m_isPopupWindow = false;
        bool m_enableDocking = false;

        ImGuiDockNodeFlags m_dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

        void addButton(const Button& button) noexcept;
        Button* getButton(const std::string& name) noexcept;
        void removeButton(const std::string& name) noexcept;

    protected:
        ImGuiID m_dockspaceID;

        ImVec2 m_bodyMinSize { };
        ImVec2 m_size { };
        ImVec2 m_headerSize { };

        std::vector<Button> m_buttons;

    private:
        ImVec2 m_footerSize { };
        ImVec2 m_lastFooterSize { };
    };
}

#endif //SUNGEARENGINEEDITOR_WINDOW_H
