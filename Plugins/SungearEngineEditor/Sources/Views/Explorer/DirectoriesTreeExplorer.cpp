//
// Created by ilya on 30.04.24.
//

#include <imgui.h>

#include "DirectoriesTreeExplorer.h"
#include "ImGuiUtils.h"

void SGE::DirectoriesTreeExplorer::renderBody()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);
    
    ImGui::Begin("Tree Explorer");
    
    m_windowSize = ImGui::GetWindowSize();
    m_windowCursorPos = ImGui::GetCursorScreenPos();
    
    ImGui::TreePush("##DirectoryExplorerTree");
    
    /*auto rust = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>("skybox0");
    ImGui::Image(rust->getTextureNativeHandler(), ImVec2(rust->m_width, rust->m_height));*/
    
    if(std::filesystem::exists(m_rootPath))
    {
        if(std::filesystem::exists(m_currentPath))
        {
            ImGui::Text(m_currentPath.string().c_str());
        }
        
        renderTreeNode(m_rootPath);
    }
    
    ImGui::TreePop();
    
    ImGui::End();
    
    ImGui::PopStyleVar();
}

void SGE::DirectoriesTreeExplorer::renderTreeNode(const std::filesystem::path& parent) noexcept
{
    bool isDirectory = std::filesystem::is_directory(parent);
    
    ImGui::Indent();
    
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
        
        if(fileExt == ".h")
        {
            ImGui::Image(m_headerIcon->getTextureNativeHandler(), ImVec2(16, 16));
        }
        else if(fileExt == ".cpp")
        {
            ImGui::Image(m_cppIcon->getTextureNativeHandler(), ImVec2(16, 16));
        }
        else if(fileExt == ".cmake" || parent.filename() == "CMakeLists.txt")
        {
            ImGui::Image(m_cmakeIcon->getTextureNativeHandler(), ImVec2(16, 16));
        }
        else if(fileExt == ".txt" || fileExt == ".log")
        {
            ImGui::Image(m_txtFileIcon->getTextureNativeHandler(), ImVec2(16, 16));
        }
        else if(fileExt == ".dll" || fileExt == ".so")
        {
            ImGui::Image(m_libraryFileIcon->getTextureNativeHandler(), ImVec2(16, 16));
        }
        else
        {
            ImGui::Image(m_unknownFileIcon->getTextureNativeHandler(), ImVec2(16, 16));
        }
        
        ImGui::SameLine();
    }
    
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
    ImGui::Text(parent.filename().string().c_str());
    
    auto mouseScreenPos = ImGui::GetCursorScreenPos();
    
    auto rectMin = ImVec2(m_windowCursorPos.x, mouseScreenPos.y - 23.0f);
    auto rectMax = ImVec2(m_windowCursorPos.x + m_windowSize.x, mouseScreenPos.y - 3.0f);
    
    bool doubleClicked = ImGui::IsMouseHoveringRect(rectMin, rectMax) &&
                         ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
    
    bool selected = ImGui::IsMouseHoveringRect(rectMin, rectMax) &&
                    ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    
    // TEST
    // ImGui::GetForegroundDrawList()->AddRectFilled(rectMin, rectMax, ImGui::ColorConvertFloat4ToU32(ImVec4(1, 0, 0, 0.1)));
    
    if(selected)
    {
        m_currentPath = parent;
    }
    
    bool lastIsCurrentNodeOpened = isCurrentNodeOpened;
    if(doubleClicked || arrowBtnClicked)
    {
        isCurrentNodeOpened = !isCurrentNodeOpened;
    }
    
    if(isCurrentNodeOpened)
    {
        for(auto it = std::filesystem::directory_iterator(parent); it != std::filesystem::directory_iterator(); ++it)
        {
            renderTreeNode(*it);
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
    
    ImGui::Unindent();
}
