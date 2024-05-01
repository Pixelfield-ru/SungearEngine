//
// Created by ilya on 27.03.24.
//

#include <SGUtils/Singleton.h>
#include <SGCore/Main/CoreMain.h>
#include <imgui_internal.h>

#include "MainView.h"
#include "PluginMain.h"

#include "Views/Explorer/DirectoryExplorer.h"
#include "Views/Explorer/DirectoriesTreeExplorer.h"

SGE::MainView::MainView()
{
    m_topToolbarView = SGCore::MakeRef<TopToolbarView>();
    m_topToolbarView->m_name = "SGE_TOP_TOOLBAR";
    
    m_explorerWindow = SGCore::MakeRef<Explorer>();
    m_explorerWindow->m_name = "SGE_EXPLORER_WINDOW";
    
    m_directoryExplorerWindow = SGCore::MakeRef<DirectoryExplorer>();
    m_directoryExplorerWindow->m_name = "SGE_DIRECTORY_EXPLORER_WINDOW";
    
    m_directoriesTreeExplorerWindow = SGCore::MakeRef<DirectoriesTreeExplorer>();
    m_directoriesTreeExplorerWindow->m_name = "SGE_DIRECTORIES_TREE_EXPLORER_WINDOW";
    
    addChild(m_topToolbarView);
    addChild(m_explorerWindow);
    addChild(m_directoryExplorerWindow);
    addChild(m_directoriesTreeExplorerWindow);
}

SGE::MainView::~MainView()
{
    removeChild(m_topToolbarView);
}

bool SGE::MainView::begin()
{
    return true;
}

void SGE::MainView::renderBody()
{
    m_dockID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    
    /*ImGui::Begin("test");
    ImGui::Text("hello!");
    ImGui::End();*/
}

void SGE::MainView::end()
{
    IView::end();
}

ImGuiID& SGE::MainView::getDockID() noexcept
{
    return m_dockID;
}

SGCore::Ref<SGE::DirectoriesTreeExplorer> SGE::MainView::getDirectoriesTreeExplorerWindow() const noexcept
{
    return m_directoriesTreeExplorerWindow;
}
