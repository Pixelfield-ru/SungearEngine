//
// Created by ilya on 27.03.24.
//

#include "SGCore/Utils/Singleton.h"
#include <SGCore/Main/CoreMain.h>
#include <imgui_internal.h>

#include "MainView.h"
#include "PluginMain.h"

#include "Views/Explorer/DirectoryExplorer.h"
#include "Views/Explorer/DirectoriesTreeExplorer.h"
#include "DialogWindowsManager.h"

SGE::MainView::MainView()
{
    m_topToolbarView = SGCore::MakeRef<TopToolbarView>();
    m_topToolbarView->m_name = "SGE_TOP_TOOLBAR";
    
    m_explorerWindow = SGCore::MakeRef<Explorer>();
    m_explorerWindow->m_name = "SGE_EXPLORER_WINDOW";
    
    m_directoryExplorer = SGCore::MakeRef<DirectoryExplorer>();
    m_directoryExplorer->m_name = "SGE_DIRECTORY_EXPLORER_WINDOW";
    
    m_directoriesTreeExplorer = SGCore::MakeRef<DirectoriesTreeExplorer>();
    m_directoriesTreeExplorer->m_name = "SGE_DIRECTORIES_TREE_EXPLORER_WINDOW";
    
    m_sceneTreeView = SGCore::MakeRef<SceneTreeView>();
    m_sceneTreeView->m_name = "SGE_SCENE_TREE_VIEW_WINDOW";

    m_sceneView = SGCore::MakeRef<SceneView>();
    m_sceneView->m_name = "SGE_SCENE_VIEW_WINDOW";
    
    m_inspectorView = SGCore::MakeRef<InspectorView>();
    m_inspectorView->m_name = "SGE_INSPECTOR_VIEW_WINDOW";

    m_logsWindow = SGCore::MakeRef<LogsWindow>();
    m_logsWindow->m_name = "SGE_LOGS_WINDOW";
    
    addChild(m_topToolbarView);
    addChild(m_explorerWindow);
    addChild(m_directoryExplorer);
    addChild(m_directoriesTreeExplorer);
    addChild(m_sceneTreeView);
    addChild(m_sceneView);
    addChild(m_inspectorView);
    addChild(m_logsWindow);

    // TEST
    /*auto projectBuiltDialogWindow = DialogWindowsManager::createOneButtonWindow("Project Build", "OK");
    projectBuiltDialogWindow.onCustomBodyRenderListener = []() {
        ImGui::SameLine();
        ImGui::TextWrapped("Hello info text! Hello info text! Hello info text! Hello info text! Hello info text! Hello info text!");
    };
    DialogWindowsManager::addDialogWindow(projectBuiltDialogWindow);*/
}

/*SGE::MainView::~MainView()
{
    removeChild(m_topToolbarView);
}*/

bool SGE::MainView::begin()
{
    ImGuizmo::BeginFrame();

    return true;
}

void SGE::MainView::renderBody()
{
    DialogWindowsManager::renderWindows();

    // m_dockID = ImGui::GetID("MainDock");
    m_dockID = ImGui::DockSpaceOverViewport(m_dockID, ImGui::GetMainViewport());
    
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

SGCore::Ref<SGE::DirectoriesTreeExplorer> SGE::MainView::getDirectoriesTreeExplorer() const noexcept
{
    return m_directoriesTreeExplorer;
}

SGCore::Ref<SGE::DirectoryExplorer> SGE::MainView::getDirectoryExplorer() const noexcept
{
    return m_directoryExplorer;
}

SGCore::Ref<SGE::TopToolbarView> SGE::MainView::getTopToolbarView() const noexcept
{
    return m_topToolbarView;
}

SGCore::Ref<SGE::SceneTreeView> SGE::MainView::getSceneTreeView() const noexcept
{
    return m_sceneTreeView;
}

SGCore::Ref<SGE::InspectorView> SGE::MainView::getInspectorView() const noexcept
{
    return m_inspectorView;
}
