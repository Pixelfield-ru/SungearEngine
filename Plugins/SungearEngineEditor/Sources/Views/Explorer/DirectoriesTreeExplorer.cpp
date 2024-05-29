//
// Created by ilya on 30.04.24.
//

#include <imgui.h>

#include "DirectoriesTreeExplorer.h"
#include "DirectoryExplorer.h"
#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"

void SGE::DirectoriesTreeExplorer::renderBody()
{
    /*{
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(cursorPos.x + 8))
    }*/
    
    ImVec2 originalSpacing = ImGui::GetStyle().ItemSpacing;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, { 0.5, 0.5 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);

    ImGui::Begin("Tree Explorer", nullptr, ImGuiWindowFlags_HorizontalScrollbar);

    try
    {
        if (std::filesystem::exists(m_rootPath))
        {
            if (std::filesystem::exists(m_currentPath))
            {
                ImFont* font = StylesManager::getCurrentStyle()->m_fonts["default_18"];
                
                assert(font && "Can not find default font (18 px) to render DirectoryExplorer");
                
                std::string text = SGUtils::Utils::toUTF8<char16_t>(m_currentPath.u16string());
                ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
                
                // ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, originalItemsSpacing);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 4 });
                
                ImGui::BeginChildFrame(ImGui::GetID("DirectoriesExplorerTreeCurrentChosenDir"),
                                       ImVec2(ImGui::GetContentRegionAvail().x, textSize.y + 9),
                                       ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
                
                ImGui::SetScrollY(0);
                
                ImGui::PushFont(font);
                
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2);
                ImGui::Text(text.c_str());
                
                ImGui::PopFont();
                
                ImGui::EndChildFrame();
                ImGui::PopStyleVar(1);
                
                ImGui::Separator();
            }
            
            ImGui::PopStyleVar(1);
            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 5 });
            
            ImGui::BeginChildFrame(ImGui::GetID("DirectoriesExplorerTreeFilesView"),
                                   ImGui::GetContentRegionAvail(),
                                   ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
            
            m_windowCursorPos = ImGui::GetCursorScreenPos();
            m_windowContentRegionMax = ImGui::GetContentRegionAvail();
            
            if(m_drawSelectedRect)
            {
                ImGui::GetWindowDrawList()->AddRectFilled(m_clickedRowRectMin, m_clickedRowRectMax,
                                                          ImGui::ColorConvertFloat4ToU32(
                                                                  ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1)), 3.0f);
            }
            
            m_drawSelectedRect = false;
            
            renderTreeNode(m_rootPath);
            
            ImGui::EndChildFrame();
            ImGui::PopStyleVar(1);
        }
    }
    catch(const std::exception& e)
    {
        std::string what = e.what();
        std::printf("Error while DirectoriesTreeExplorer::renderBody. Error is: %s\n", what.c_str());
        spdlog::error("Error while DirectoriesTreeExplorer::renderBody. Error is: {0}", what);
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
}

void SGE::DirectoriesTreeExplorer::renderTreeNode(const std::filesystem::path& parent) noexcept
{
    bool isDirectory = std::filesystem::is_directory(parent);
    
    auto& isCurrentNodeOpened = m_pathsNodes[parent];
    
    bool arrowBtnClicked = false;
    
    auto style = StylesManager::getCurrentStyle();
    
    if(isDirectory)
    {
        if(isCurrentNodeOpened)
        {
            arrowBtnClicked = ImGuiUtils::ImageButton(style->m_chevronDownIcon->getSpecialization(16, 16)->getTexture()->getTextureNativeHandler(), ImVec2(16 * m_UIScale.x, 16 * m_UIScale.y));
        }
        else
        {
            arrowBtnClicked = ImGuiUtils::ImageButton(style->m_chevronRightIcon->getSpecialization(16, 16)->getTexture()->getTextureNativeHandler(), ImVec2(16 * m_UIScale.x, 16 * m_UIScale.y));
        }
        
        ImGui::SameLine();
        ImGui::Image(style->m_folderIcon->getSpecialization(16, 16)->getTexture()->getTextureNativeHandler(), ImVec2(16, 16));
        ImGui::SameLine();
    }
    else
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 23);
        
        auto fileExt = parent.extension();
        
        SGCore::Ref<SGCore::ITexture2D> iconTexture = ImGuiUtils::getFileIcon(parent, { 16, 16 }, &onIconRender);
        
        ImGui::Image(iconTexture->getTextureNativeHandler(), ImVec2(16, 16));
        
        ImGui::SameLine();
    }
    
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
    ImGui::Text(SGUtils::Utils::toUTF8<char16_t>(parent.filename().u16string()).c_str());
    
    auto mouseScreenPos = ImGui::GetCursorScreenPos();
    
    auto rectMin = ImVec2(m_windowCursorPos.x, mouseScreenPos.y - 25.0f);
    auto rectMax = ImVec2(m_windowCursorPos.x + ImGui::GetScrollX() + m_windowContentRegionMax.x + 3, mouseScreenPos.y - 3.0f);
    
    bool rowDoubleClicked = ImGui::IsMouseHoveringRect(rectMin, rectMax) &&
                            ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
    
    bool rowClicked = ImGui::IsMouseHoveringRect(rectMin, rectMax) &&
                      ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    
    if(rowClicked && isDirectory && !arrowBtnClicked)
    {
        SungearEngineEditor::getInstance()->getMainView()->getDirectoryExplorer()->setCurrentPath(parent);
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
