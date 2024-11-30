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
        TopToolbarView();
        
        bool begin() final;
        void renderBody() final;
        void end() final;
        
        SGCore::Ref<FileCreateDialog> m_fileCreateDialog = SGCore::MakeRef<FileCreateDialog>();
        SGCore::Ref<ProjectCreateDialog> m_projectCreateDialog = SGCore::MakeRef<ProjectCreateDialog>();
        SGCore::Ref<EngineSettingsView> m_engineSettingsView = SGCore::MakeRef<EngineSettingsView>();

    private:
        ImGuiID m_dockID = 0;

        SGCore::AssetRef<SGCore::ITexture2D> m_engineIcon;
        SGCore::AssetRef<SGCore::ITexture2D> m_moreIcon;

        Popup m_popup {
                "ToolbarFileActions",
                {
                        {
                                .m_name = "New...",
                                .m_elements = {
                                        SGCore::MakeRef<PopupElement>(PopupElement {
                                                .m_name = "Project",
                                                .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                           18)->getTexture()
                                        }),
                                        SGCore::MakeRef<PopupElement>(PopupElement {
                                                .m_name = "File",
                                                .m_icon = StylesManager::getCurrentStyle()->m_linesIcon->getSpecialization(18,
                                                                                                                           18)->getTexture()
                                        }),
                                }
                        },
                        {
                                .m_name = "Open...",
                                .m_drawSeparatorAfter = true,
                                .m_elements = {
                                        SGCore::MakeRef<PopupElement>(PopupElement {
                                                .m_name = "Project",
                                                .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                           18)->getTexture()
                                        }),
                                        SGCore::MakeRef<PopupElement>(PopupElement {
                                                .m_name = "File",
                                                .m_icon = StylesManager::getCurrentStyle()->m_linesIcon->getSpecialization(18,
                                                                                                                           18)->getTexture()
                                        }),
                                }
                        },
                        {
                                .m_name = "Engine Settings",
                        }
                }
        };
    };
}

#endif //SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
