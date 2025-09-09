//
// Created by ilya on 27.03.24.
//

#ifndef SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
#define SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "FileCreateDialog.h"
#include "ProjectCreateDialog.h"
#include "Views/TopToolbar/Settings/Engine/EngineSettingsView.h"

namespace SGE
{
    struct TopToolbarView : public SGCore::ImGuiWrap::IView
    {
        virtual ~TopToolbarView() = default;

        TopToolbarView();
        
        bool begin() final;
        void renderBody() final;
        void end() final;
        
        SGCore::Ref<FileCreateDialog> m_fileCreateDialog = SGCore::MakeRef<FileCreateDialog>();
        SGCore::Ref<ProjectCreateDialog> m_projectCreateDialog = SGCore::MakeRef<ProjectCreateDialog>();
        SGCore::Ref<EngineSettingsView> m_engineSettingsView = SGCore::MakeRef<EngineSettingsView>();

    private:
        void drawButtonWithPopup(const std::string& name, Popup& buttonPopup) noexcept;

        ImGuiID m_dockID = 0;

        SGCore::AssetRef<SGCore::ITexture2D> m_engineIcon;
        SGCore::AssetRef<SGCore::ITexture2D> m_moreIcon;

        Popup m_fileButtonPopup;
        Popup m_helpButtonPopup;
        Popup m_projectButtonPopup;

        ImVec2 m_areaToHighlightPos { 0, 0 };
        ImVec2 m_areaToHighlightSize { 0, 0 };

        bool m_isAnyButtonHovered = false;

        bool m_showToolsButtons = false;
    };
}

#endif //SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
