//
// Created by ilya on 30.04.24.
//

#include <imgui.h>
#include <imgui_internal.h>
#include <SGUtils/Utils.h>

#include "DirectoryExplorer.h"
#include "Views/MainView.h"
#include "DirectoriesTreeExplorer.h"
#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"

void SGE::DirectoryExplorer::renderBody()
{
    m_drawableFilesNames.clear();
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);
    
    ImGui::Begin("DirectoryExplorer", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    // =================================================
    
    if(std::filesystem::exists(m_currentPath) && std::filesystem::is_directory(m_currentPath))
    {
        ImGui::Text(m_currentPath.string().c_str());
        
        for(auto it = std::filesystem::directory_iterator(m_currentPath);
            it != std::filesystem::directory_iterator(); ++it)
        {
            std::filesystem::path curPath = *it;
            
            bool isDirectory = std::filesystem::is_directory(curPath);
            
            SGCore::Ref<SGCore::ITexture2D> iconTexture;
            
            ImVec2 cursorPos = ImGui::GetCursorPos();
            
            ImVec2 windowSize = ImGui::GetWindowSize();
            
            if(cursorPos.x + m_currentItemsSize.x >= windowSize.x)
            {
                cursorPos.x = ImGui::GetStyle().WindowPadding.x;
                cursorPos.y += m_currentItemsSize.y + 3 + m_iconsPadding.y * m_UIScale.y;
            }
            
            ImGui::SetCursorPos(ImVec2(cursorPos.x + m_iconsPadding.x * m_UIScale.x, cursorPos.y));
            
            ImVec2 nameStartPos = ImGui::GetCursorScreenPos();
            
            m_drawableFilesNames[curPath] = {
                    .m_position = nameStartPos,
                    .m_overdraw = false
            };
            
            ImGui::SetWindowSize(ImGui::GetWindowContentRegionMax());
            
            auto fileIcon = ImGuiUtils::getFileIcon(curPath,
                                                    { (std::uint32_t) (50 * m_UIScale.x),
                                                      (std::uint32_t) (50 * m_UIScale.y) }, &onIconRender);
            
            if(isDirectory)
            {
                fileIcon = StylesManager::getCurrentStyle()->m_folderIcon->getSpecialization((std::uint32_t) (50 * m_UIScale.x), (std::uint32_t) (50 * m_UIScale.y))->getTexture();
            }
            
            ImGuiUtils::ImageButton(fileIcon->getTextureNativeHandler(), ImVec2(50 * m_UIScale.x, 50 * m_UIScale.y));
            
            m_currentItemsSize = ImVec2(50 * m_UIScale.x + 3 * 2, 50 * m_UIScale.y + 3 * 2);
            
            ImGui::SameLine();
        }
    }
    
    for(const auto& fileNameInfoPair : m_drawableFilesNames)
    {
        auto& path = fileNameInfoPair.first;
        auto& drawableNameInfo = fileNameInfoPair.second;
        
        std::string fileName = path.stem().string();
        std::string fileExt = path.extension().string();
        std::string fullName = fileName + fileExt;
        
        ImVec2 nameSize = ImGui::CalcTextSize(fileName.c_str());
        
        // y = 3 lines
        ImVec2 maxNameSize = ImVec2(50 * m_UIScale.x + 3 * 2 + m_iconsPadding.x / 4, nameSize.y * m_nameMaxLinesCount);
        
        std::string finalFileName;
        std::string rawFinalFileName;
        
        size_t curLine = 1;
        for(char c : fileName)
        {
            ImVec2 curNameSize = ImGui::CalcTextSize(rawFinalFileName.c_str());
            if(curNameSize.x > maxNameSize.x * curLine)
            {
                finalFileName += "/";
                ++curLine;
            }
            
            if(curLine > m_nameMaxLinesCount + 1)
            {
                size_t sz = finalFileName.length();
                
                finalFileName[sz - 1] = '.';
                finalFileName[sz - 2] = '.';
                finalFileName[sz - 3] = '.';
                
                break;
            }
            
            finalFileName += c;
            rawFinalFileName += c;
        }
        
        ImGui::GetWindowDrawList()->AddText(drawableNameInfo.m_position, ImGui::ColorConvertFloat4ToU32(ImVec4(1.0, 1.0, 1.0, 1.0)), finalFileName.c_str());
    }
    
    // =================================================
    ImGui::End();
    
    ImGui::PopStyleVar();
}
