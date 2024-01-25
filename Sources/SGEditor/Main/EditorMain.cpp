//
// Created by stuka on 28.11.2023.
//

#include <memory>

#include "EditorMain.h"
#include "SGEditor/Views/StandardEditor/SceneHierarchy.h"
#include "imgui.h"
#include "SGCore/ImGuiWrap/Views/Base/Window.h"
#include "SGUtils/Singleton.h"
#include "SGCore/ImGuiWrap/ViewsInjector.h"

void SGEditor::EditorMain::start() noexcept
{
    static auto sceneHierarchyWnd = std::make_shared<SGCore::ImGuiWrap::Window>();
    sceneHierarchyWnd->m_name = "Scene hierarchy";

    static auto sceneHierarchy = std::make_shared<SceneHierarchy>();
    sceneHierarchy->m_name = "Scene hierarchy tree";

    auto& viewsInjector = *SGUtils::Singleton::getSharedPtrInstance<SGCore::ImGuiWrap::ViewsInjector>();
    viewsInjector[sceneHierarchyWnd->m_name].m_rootView = sceneHierarchyWnd;
    viewsInjector[sceneHierarchyWnd->m_name].m_childrenViews.push_back(sceneHierarchy);
}
