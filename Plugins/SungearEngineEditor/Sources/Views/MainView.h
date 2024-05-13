//
// Created by ilya on 27.03.24.
//

#ifndef SUNGEARENGINEEDITOR_MAINVIEW_H
#define SUNGEARENGINEEDITOR_MAINVIEW_H

#include <SGCore/Main/CoreGlobals.h>
#include <imgui.h>
#include <SGCore/ImGuiWrap/Views/IView.h>

#include "TopToolbar/TopToolbarView.h"
#include "Explorer/Explorer.h"

namespace SGE
{
    struct DirectoriesTreeExplorer;
    struct DirectoryExplorer;
    
    struct MainView : SGCore::ImGuiWrap::IView
    {
        MainView();
        ~MainView();
        
        bool begin() override;
        void renderBody() override;
        void end() override;
        
        ImGuiID& getDockID() noexcept;
        
        SGCore::Ref<DirectoriesTreeExplorer> getDirectoriesTreeExplorer() const noexcept;
        SGCore::Ref<DirectoryExplorer> getDirectoryExplorer() const noexcept;
        
    private:
        SGCore::Ref<TopToolbarView> m_topToolbarView;
        
        SGCore::Ref<Explorer> m_explorerWindow;
        SGCore::Ref<DirectoryExplorer> m_directoryExplorer;
        SGCore::Ref<DirectoriesTreeExplorer> m_directoriesTreeExplorer;
        
        ImGuiID m_dockID = 0;
    };
}

#endif //SUNGEARENGINEEDITOR_MAINVIEW_H
