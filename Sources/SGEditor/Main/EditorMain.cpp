//
// Created by stuka on 28.11.2023.
//

#include <memory>

#include "EditorMain.h"
#include "SGEditor/Views/Base/Window.h"
#include "SGEditor/Views/StandardEditor/SceneHierarchy.h"
#include "SGEditor/Views/ViewsManager.h"
#include "imgui.h"

void SGEditor::EditorMain::start() noexcept
{
    auto sceneHierarchyWnd = std::make_shared<Window>();
    sceneHierarchyWnd->m_name = "Scene hierarchy";

    auto sceneHierarchy = std::make_shared<SceneHierarchy>();
    sceneHierarchy->m_name = "Scene hierarchy tree";

    sceneHierarchyWnd->m_subViews.push_back(sceneHierarchy);

    getMainViewsManager()->m_rootViews.push_back(sceneHierarchyWnd);
}
