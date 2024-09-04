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
        bool m_enableEscapeToCloseWindow = true;

        ImGuiDockNodeFlags m_dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

        void addButton(const Button& button) noexcept;
        /**
         * Copies the found button to 'out' button.
         * @param name
         * @param out
         * @return
         */
        [[nodiscard]] bool tryCopyGetButton(const std::string& name, Button* out = nullptr) noexcept;
        /**
         * Copies the found button to 'out' button.
         * @param index
         * @param out
         * @return
         */
        [[nodiscard]] bool tryCopyGetButton(const std::size_t& index, Button* out = nullptr) noexcept;
        /**
         * Returns pointer to original button instance in vector of buttons. Returns nullptr if button was not found.
         * @param index
         * @param out
         * @return
         */
        [[nodiscard]] Button* tryGetButton(const std::string& name) noexcept;
        /**
         * Returns pointer to original button instance in vector of buttons. Returns nullptr if button was not found.
         * @param index
         * @param out
         * @return
         */
        [[nodiscard]] Button* tryGetButton(const std::size_t& index) noexcept;
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
