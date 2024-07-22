//
// Created by Ilya on 22.07.2024.
//

#include "Tree.h"
#include "Styles/StylesManager.h"
#include "ImGuiUtils.h"

void SGE::Tree::addTreeNode(const SGE::TreeNode& treeNode) noexcept
{
    if(tryGetTreeNode(treeNode.m_name)) return;

    m_treeNodes.push_back(treeNode);
}

bool SGE::Tree::tryGetTreeNode(const std::string& name, TreeNode* out) noexcept
{
    auto foundIt = std::find_if(m_treeNodes.begin(), m_treeNodes.end(), [&name](const TreeNode& node) {
        return name == node.m_name;
    });

    if(out && foundIt != m_treeNodes.end())
    {
        *out = *foundIt;
    }

    return foundIt != m_treeNodes.end();
}

void SGE::Tree::removeTreeNode(const std::string& name) noexcept
{
    std::erase_if(m_treeNodes, [&name](const TreeNode& node) {
        return name == node.m_name;
    });
}

void SGE::Tree::draw(const ImVec2& uiScale)
{
    if (m_drawSelectedNodeRect)
    {
        ImGui::GetWindowDrawList()->AddRectFilled(m_clickedRowRectMin, m_clickedRowRectMax,
                                                  ImGui::ColorConvertFloat4ToU32(
                                                          ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1)), 3.0f);
    }

    m_windowCursorPos = ImGui::GetCursorScreenPos();
    m_windowContentRegionMax = ImGui::GetContentRegionAvail();

    m_drawSelectedNodeRect = false;

    drawNodes(m_treeNodes, uiScale);
}

void SGE::Tree::drawNodes(std::vector<TreeNode>& nodes, const ImVec2& uiScale)
{
    for(auto& node : nodes)
    {
        bool arrowBtnClicked = false;

        auto style = StylesManager::getCurrentStyle();

        if(node.m_isTree)
        {
            if (node.m_isOpened)
            {
                // ImGui::SetCursorPosY(ImGui::GetCursorPosY() );
                arrowBtnClicked = ImGuiUtils::ImageButton(
                        style->m_chevronDownIcon->getSpecialization(16, 16)->getTexture()->getTextureNativeHandler(),
                        ImVec2((16 + 6) * uiScale.x, (16 + 6) * uiScale.y),
                        ImVec2(16 * uiScale.x, 16 * uiScale.y)).m_isLMBClicked;
            }
            else
            {
                // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
                arrowBtnClicked = ImGuiUtils::ImageButton(
                        style->m_chevronRightIcon->getSpecialization(16, 16)->getTexture()->getTextureNativeHandler(),
                        ImVec2((16 + 6) * uiScale.x, (16 + 6) * uiScale.y),
                        ImVec2(16 * uiScale.x, 16 * uiScale.y)).m_isLMBClicked;
            }

            ImGui::SameLine();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
        }

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);

        ImGui::Text(node.m_text.c_str());

        auto mouseScreenPos = ImGui::GetCursorScreenPos();

        auto rectMin = ImVec2(m_windowCursorPos.x, mouseScreenPos.y - 26);
        auto rectMax = ImVec2(m_windowCursorPos.x + ImGui::GetScrollX() + m_windowContentRegionMax.x + 3,
                              mouseScreenPos.y - 4);

        bool rowDoubleClicked = ImGui::IsMouseHoveringRect(rectMin, rectMax) &&
                                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) &&
                                ImGui::IsWindowHovered();

        bool rowClicked = ImGui::IsMouseHoveringRect(rectMin, rectMax) &&
                          ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered();

        if(rowClicked && !arrowBtnClicked)
        {
            m_chosenTreeNodeName = node.m_name;

            node.onClicked(node);
        }

        if(m_chosenTreeNodeName == node.m_name)
        {
            m_clickedRowRectMin = rectMin;
            m_clickedRowRectMax = rectMax;
            m_drawSelectedNodeRect = true;
        }

        if((rowDoubleClicked || arrowBtnClicked) && node.m_isTree)
        {
            node.m_isOpened = !node.m_isOpened;
        }

        if(node.m_isOpened)
        {
            ImGui::Indent(m_indentWidth);
            drawNodes(node.m_children, uiScale);
            ImGui::Unindent();
        }
    }
}
