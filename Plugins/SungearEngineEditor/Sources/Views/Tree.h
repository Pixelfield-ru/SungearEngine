//
// Created by Ilya on 22.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TREE_H
#define SUNGEARENGINEEDITOR_TREE_H

#include <SGCore/pch.h>
#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/Utils/UniqueNamesManager.h>
#include <SGCore/Graphics/API/ITexture2D.h>

namespace SGE
{
    struct TreeNode
    {
        std::string m_text;
        SGCore::UniqueName m_name = m_text;

        SGCore::Ref<SGCore::ITexture2D> m_icon;
        std::any m_userData;

        std::function<void(TreeNode& self)> onClicked;

        std::vector<TreeNode> m_children;

        bool m_isTree = false;
        bool m_useNameAsText = false;
        bool m_isOpened = false;

        SGCore::Ref<SGCore::UniqueNamesManager> m_childrenNamesManager = SGCore::MakeRef<SGCore::UniqueNamesManager>();
    };

    struct Tree
    {
        float m_indentWidth = 40.0f;

        void addTreeNode(const TreeNode& treeNode) noexcept;
        [[nodiscard]] bool tryGetTreeNode(const std::string& name, TreeNode* out = nullptr) noexcept;
        void removeTreeNode(const std::string& name) noexcept;

        void draw(const ImVec2& uiScale);

    private:
        void drawNodes(std::vector<TreeNode>& nodes, Tree* parentTree, TreeNode* parentTreeNode, const ImVec2& uiScale);

        std::vector<TreeNode> m_treeNodes;
        std::string m_chosenTreeNodeName;

        bool m_drawSelectedNodeRect = false;
        ImVec2 m_clickedRowRectMin { };
        ImVec2 m_clickedRowRectMax { };
        ImVec2 m_windowCursorPos { };
        ImVec2 m_windowContentRegionMax { };

        SGCore::Ref<SGCore::UniqueNamesManager> m_childrenNamesManager = SGCore::MakeRef<SGCore::UniqueNamesManager>();
    };
}

#endif //SUNGEARENGINEEDITOR_TREE_H
