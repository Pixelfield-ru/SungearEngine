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
    struct Tree;
    
    struct TreeNode
    {
        friend struct Tree;

        SGCore::UniqueName m_name;

        SGCore::AssetRef<SGCore::ITexture2D> m_icon;
        std::any m_userData;

        std::function<void(TreeNode& self)> onClicked;

        std::vector<TreeNode> m_children;

        bool m_isTree = false;
        bool m_useNameAsText = false;
        bool m_isOpened = false;

        SGCore::Ref<SGCore::UniqueNamesManager> m_childrenNamesManager = SGCore::MakeRef<SGCore::UniqueNamesManager>();

        /**
         * Copies the found button to 'out' tree node.\n
         * Recursively iterates through all nodes.
         * @param name
         * @param out
         * @return
         */
        [[nodiscard]] bool tryCopyGetTreeNodeRecursively(const std::string& name, TreeNode* out = nullptr) const noexcept;

        void clear() noexcept;
        void click() noexcept;

        void openBranchToThis() noexcept;
        void openBranchAndSelectThis() noexcept;

        void setText(const std::string& text) noexcept;
        [[nodiscard]] std::string getText() const noexcept;

        void setParentTree(Tree* parentTree) noexcept;

    private:
        bool m_isTextFirstAssignment = true;

        std::string m_text;

        void updateChildrenParentPointers() noexcept;

        Tree* m_parentTree { };
        TreeNode* m_parentNode { };
    };

    struct Tree
    {
        float m_indentWidth = 40.0f;

        void addTreeNode(const TreeNode& treeNode) noexcept;
        /**
         * Copies the found button to 'out' tree node.
         * @param name
         * @param out
         * @return
         */
        [[nodiscard]] bool tryCopyGetTreeNode(const std::string& name, TreeNode* out = nullptr) noexcept;
        /**
         * Copies the found button to 'out' tree node.\n
         * Recursively iterates through all nodes.
         * @param name
         * @param out
         * @return
         */
        [[nodiscard]] bool tryCopyGetTreeNodeRecursively(const std::string& name, TreeNode* out = nullptr) noexcept;
        void removeTreeNode(const std::string& name) noexcept;
        void clear() noexcept;

        void draw(const ImVec2& uiScale);

        std::string m_chosenTreeNodeName;

    private:
        void drawNodes(std::vector<TreeNode>& nodes, Tree* parentTree, TreeNode* parentTreeNode, const ImVec2& uiScale);

        std::vector<TreeNode> m_treeNodes;

        bool m_drawSelectedNodeRect = false;
        ImVec2 m_clickedRowRectMin { };
        ImVec2 m_clickedRowRectMax { };
        ImVec2 m_windowCursorPos { };
        ImVec2 m_windowContentRegionMax { };

        SGCore::Ref<SGCore::UniqueNamesManager> m_childrenNamesManager = SGCore::MakeRef<SGCore::UniqueNamesManager>();
    };
}

#endif //SUNGEARENGINEEDITOR_TREE_H
