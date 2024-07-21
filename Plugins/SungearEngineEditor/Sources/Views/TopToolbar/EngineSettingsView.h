//
// Created by Ilya on 17.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H
#define SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "Views/Window.h"

namespace SGE
{
    struct TreeNode
    {
        std::string m_text;
        std::string m_name = m_text;

        std::function<void(TreeNode& self)> onClicked;

        std::vector<TreeNode> m_children;

        bool m_isTree = false;

        bool m_isOpened = false;
    };

    struct EngineSettingsView : Window
    {
        EngineSettingsView();
        EngineSettingsView(const EngineSettingsView&) = default;
        EngineSettingsView(EngineSettingsView&&) = default;

        void renderBody() final;
        void postRenderBody() final;

    private:
        ImVec2 m_currentTreeSize { };
        ImVec2 m_currentSettingsContentSize { };

        float m_minTreeWindowWidth = 300.0f;
        float m_minSettingsContentWindowWidth = 300.0f;

        ImGuiID m_treeDockNodeID { };
        ImGuiID m_settingsContentDockNodeID { };

        bool m_firstTime = true;

        std::vector<TreeNode> m_treeNodes;
        std::string m_chosenTreeNodeName;

        void renderTreeNodes(std::vector<TreeNode>& nodes) noexcept;

        ImVec2 m_treeWindowCursorPos { };
        ImVec2 m_treeWindowContentRegionMax { };

        ImVec2 m_clickedTreeRowRectMin { };
        ImVec2 m_clickedTreeRowRectMax { };

        bool m_drawTreeSelectedNodeRect = false;

        std::function<void()> onSettingsContentDraw;
    };
}

#endif //SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H
