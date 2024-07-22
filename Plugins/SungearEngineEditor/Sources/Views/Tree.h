//
// Created by Ilya on 22.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TREE_H
#define SUNGEARENGINEEDITOR_TREE_H

#include <SGCore/pch.h>

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

    struct Tree
    {
        float m_indentWidth = 40.0f;

        void addTreeNode(const TreeNode& treeNode) noexcept;
        [[nodiscard]] bool tryGetTreeNode(const std::string& name, TreeNode* out = nullptr) noexcept;
        void removeTreeNode(const std::string& name) noexcept;

        void draw(const ImVec2& uiScale);

    private:
        void drawNodes(std::vector<TreeNode>& nodes, const ImVec2& uiScale);

        std::vector<TreeNode> m_treeNodes;
        std::string m_chosenTreeNodeName;

        bool m_drawSelectedNodeRect = false;
        ImVec2 m_clickedRowRectMin { };
        ImVec2 m_clickedRowRectMax { };
        ImVec2 m_windowCursorPos { };
        ImVec2 m_windowContentRegionMax { };
    };
}

#endif //SUNGEARENGINEEDITOR_TREE_H
