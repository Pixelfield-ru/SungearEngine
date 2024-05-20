//
// Created by ilya on 30.04.24.
//

#include <imgui.h>

#include "DirectoriesTreeExplorer.h"
#include "DirectoryExplorer.h"
#include "ImGuiUtils.h"

void SGE::DirectoriesTreeExplorer::renderBody()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);
    
    ImGui::Begin("Tree Explorer", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    
    auto windowSize = ImGui::GetWindowSize();
    m_windowContentRegionMax = ImGui::GetContentRegionAvail();
    m_windowCursorPos = ImGui::GetCursorScreenPos();
    
    // ImGui::TreePush("##DirectoryExplorerTree");
    
    if(m_drawSelectedRect)
    {
        ImGui::GetWindowDrawList()->AddRectFilled(m_clickedRowRectMin, m_clickedRowRectMax,
                                                  ImGui::ColorConvertFloat4ToU32(
                                                          ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1)), 3.0f);
    }
    
    m_drawSelectedRect = false;
    
    if(std::filesystem::exists(m_rootPath))
    {
        if(std::filesystem::exists(m_currentPath))
        {
            ImGui::Text(m_currentPath.string().c_str());
        }
        
        renderTreeNode(m_rootPath);
    }
    
    // ImGui::TreePop();
    
    ImGui::End();
    
    ImGui::PopStyleVar();
}

void SGE::DirectoriesTreeExplorer::renderTreeNode(const std::filesystem::path& parent) noexcept
{
    bool isDirectory = std::filesystem::is_directory(parent);
    
    auto& isCurrentNodeOpened = m_pathsNodes[parent];
    
    bool arrowBtnClicked = false;
    
    if(isDirectory)
    {
        if(isCurrentNodeOpened)
        {
            arrowBtnClicked = ImGuiUtils::ImageButton(m_chevronDownIcon->getTextureNativeHandler(), ImVec2(16, 16));
        }
        else
        {
            arrowBtnClicked = ImGuiUtils::ImageButton(m_chevronRightIcon->getTextureNativeHandler(), ImVec2(16, 16));
        }
        
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5);
        ImGui::Image(m_folderIcon->getTextureNativeHandler(), ImVec2(16, 16));
        ImGui::SameLine();
    }
    else
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 23);
        
        auto fileExt = parent.extension();
        
        SGCore::Ref<SGCore::ITexture2D> iconTexture;
        
        if(fileExt == ".h")
        {
            iconTexture = m_headerIcon;
        }
        else if(fileExt == ".cpp")
        {
            iconTexture = m_cppIcon;
        }
        else if(fileExt == ".cmake" || parent.filename() == "CMakeLists.txt")
        {
            iconTexture = m_cmakeIcon;
        }
        else if(fileExt == ".txt" || fileExt == ".log")
        {
            iconTexture = m_txtFileIcon;
        }
        else if(fileExt == ".dll" || fileExt == ".so")
        {
            iconTexture = m_libraryFileIcon;
        }
        
        onIconRender(iconTexture, fileExt, parent.filename());
        
        if(!iconTexture)
        {
            iconTexture = m_unknownFileIcon;
        }
        
        ImGui::Image(iconTexture->getTextureNativeHandler(), ImVec2(16, 16));
        
        ImGui::SameLine();
    }
    
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
    ImGui::Text(parent.filename().string().c_str());
    
    auto mouseScreenPos = ImGui::GetCursorScreenPos();
    
    auto rectMin = ImVec2(m_windowCursorPos.x, mouseScreenPos.y - 25.0f);
    auto rectMax = ImVec2(m_windowCursorPos.x + ImGui::GetScrollX() + m_windowContentRegionMax.x + 3, mouseScreenPos.y - 3.0f);
    
    bool rowDoubleClicked = ImGui::IsMouseHoveringRect(rectMin, rectMax) &&
                            ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
    
    bool rowClicked = ImGui::IsMouseHoveringRect(rectMin, rectMax) &&
                      ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    
    if(rowClicked && isDirectory && !arrowBtnClicked)
    {
        SungearEngineEditor::getInstance()->getMainView()->getDirectoryExplorer()->m_currentPath = parent;
    }
    
    if(rowClicked)
    {
        m_currentPath = parent;
        
        if(!arrowBtnClicked)
        {
            m_selectedPath = parent;
        }
    }
    
    if(m_selectedPath == parent)
    {
        m_clickedRowRectMin = rectMin;
        m_clickedRowRectMax = rectMax;
        m_drawSelectedRect = true;
    }
    
    bool lastIsCurrentNodeOpened = isCurrentNodeOpened;
    if(rowDoubleClicked || arrowBtnClicked)
    {
        isCurrentNodeOpened = !isCurrentNodeOpened;
    }
    
    if(isCurrentNodeOpened && isDirectory)
    {
        for(auto it = std::filesystem::directory_iterator(parent); it != std::filesystem::directory_iterator(); ++it)
        {
            ImGui::Indent();
            renderTreeNode(*it);
            ImGui::Unindent();
        }
    }
    
    if(!isCurrentNodeOpened && lastIsCurrentNodeOpened)
    {
        for(auto& pathPair : m_pathsNodes)
        {
            if(pathPair.first.parent_path().string().contains(parent.string()))
            {
                pathPair.second = false;
            }
        }
    }
}
