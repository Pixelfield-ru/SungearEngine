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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);
    
    ImGui::Begin("DirectoryExplorer", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    
    // =================================================
    
    if(std::filesystem::exists(m_maxPath) && std::filesystem::is_directory(m_maxPath))
    {
        {
            ImFont* font = StylesManager::getCurrentStyle()->m_fonts["default_18"];
            
            assert(font && "Can not find default font (18 px) to render DirectoryExplorer");
            
            std::string text = SGUtils::Utils::toUTF8<char16_t>(m_maxPath.u16string());
            ImVec2 pathSize = ImGui::CalcTextSize(text.c_str());
            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 4 });
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            
            ImGui::BeginChildFrame(ImGui::GetID("DirectoryExplorerCurrentChosenDir"),
                                   ImVec2(ImGui::GetContentRegionAvail().x, pathSize.y + 11),
                                   ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
            
            ImGui::SetScrollY(0);
            
            std::vector<std::filesystem::path> subpaths(m_maxPath.begin(), m_maxPath.end());
            
            std::filesystem::path concatPath;
            
            ImGui::PushFont(font);
            
            for(auto it = subpaths.begin(); it != subpaths.end(); ++it)
            {
                bool isLastDirectory = it == subpaths.end() - 1;

                concatPath += *it;
                concatPath += std::filesystem::path::preferred_separator;

                if(*it == "/" || *it == "\\") continue;
                
                std::string u8DirName = SGUtils::Utils::toUTF8<char16_t>(it->u16string());
                ImVec2 dirNameTextSize = ImGui::CalcTextSize(u8DirName.c_str());
                
                ImVec2 curCursorPos = ImGui::GetCursorScreenPos();
                
                bool isHovering = ImGui::IsMouseHoveringRect(curCursorPos, { curCursorPos.x + dirNameTextSize.x, curCursorPos.y + dirNameTextSize.y });
                bool mouseClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                
                std::filesystem::path concatPathCanonical = std::filesystem::canonical(concatPath);
                
                if(isHovering && mouseClicked)
                {
                    setCurrentPath(concatPathCanonical);
                }
                
                if(concatPathCanonical == m_currentPath)
                {
                    ImGui::TextColored(ImVec4(50 / 255.0f, 120 / 255.0f, 170 / 255.0f, 1.0f), u8DirName.c_str());
                }
                else if(isHovering)
                {
                    ImGui::TextColored(ImVec4(0.8, 0.8, 0.8, 1.0), u8DirName.c_str());
                }
                else
                {
                    ImGui::TextColored(ImVec4(1.0, 1.0, 1.0, 1.0), u8DirName.c_str());
                }
                
                ImGui::SameLine();
                if(!isLastDirectory)
                {
                    ImGui::Text("/");
                    ImGui::SameLine();
                }
            }
            
            ImGui::PopFont();
            
            ImGui::EndChildFrame();
            
            ImGui::PopStyleVar(1);
            
            ImGui::Separator();
        }
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 8, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(m_iconsPadding.x * m_UIScale.x, m_iconsPadding.y * m_UIScale.y));
        
        ImGui::BeginChildFrame(ImGui::GetID("DirectoryExplorerFilesView"),
                               ImGui::GetContentRegionAvail(),
                               ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
        
        for(auto it = std::filesystem::directory_iterator(m_currentPath);
            it != std::filesystem::directory_iterator(); ++it)
        {
            std::filesystem::path curPath = *it;
            
            bool isDirectory = std::filesystem::is_directory(curPath);
            
            SGCore::Ref<SGCore::ITexture2D> iconTexture;
            
            ImVec2 cursorPos = ImGui::GetCursorPos();
            
            ImVec2 regionMax = ImGui::GetWindowSize();
            
            // -m_iconsPadding.x / 4, +m_iconsPadding.x / 4 is name of file area
            // m_iconsPadding is distance between icons
            if(cursorPos.x + m_currentItemsSize.x + m_iconsPadding.x / 4 >= regionMax.x - ImGui::GetStyle().ScrollbarSize)
            {
                ImGui::NewLine();
            }
            
            ImVec2 nameStartPos = ImGui::GetCursorPos();
            nameStartPos.y += m_iconsSize.y * m_UIScale.y;
            
            m_drawableFilesNames[curPath] = {
                    .m_position = nameStartPos,
                    .m_overdraw = false
            };
            
            auto fileIcon = ImGuiUtils::getFileIcon(curPath,
                                                    { (std::uint32_t) (m_iconsSize.x * m_UIScale.x),
                                                      (std::uint32_t) (m_iconsSize.y * m_UIScale.y) }, &onIconRender);
            
            if(isDirectory)
            {
                fileIcon = StylesManager::getCurrentStyle()->m_folderIcon->getSpecialization((std::uint32_t) (m_iconsSize.x * m_UIScale.x), (std::uint32_t) (m_iconsSize.y * m_UIScale.y))->getTexture();
            }
            
            ImGuiUtils::ImageButton(fileIcon->getTextureNativeHandler(), ImVec2(m_iconsSize.y * m_UIScale.x, m_iconsSize.y * m_UIScale.y));

            m_currentItemsSize = ImVec2(m_iconsSize.x * m_UIScale.x + 3 * 2, m_iconsSize.y * m_UIScale.y + 3 * 2);
            
            ImGui::SameLine();
        }
        
        for(const auto& fileNameInfoPair : m_drawableFilesNames)
        {
            try
            {
                auto& path = fileNameInfoPair.first;
                auto& drawableNameInfo = fileNameInfoPair.second;
                
                std::u16string fileName = path.stem().u16string();
                std::u16string fileExt = path.extension().u16string();
                std::u16string fullName = fileName + fileExt;
                
                ImVec2 nameSize = ImGui::CalcTextSize(SGUtils::Utils::toUTF8<char16_t>(fileName).c_str());
                
                // y = 3 lines
                ImVec2 maxNameSize = ImVec2(m_iconsSize.x * m_UIScale.x + 3 * 2 + m_iconsPadding.x / 4,
                                            nameSize.y * m_nameMaxLinesCount);
                
                std::u16string finalFileName;
                std::u16string rawFinalFileName;
                
                size_t curLine = 1;
                for(wchar_t c : fileName)
                {
                    ImVec2 curNameSize = ImGui::CalcTextSize(SGUtils::Utils::toUTF8<char16_t>(rawFinalFileName).c_str());
                    if(curNameSize.x > maxNameSize.x * curLine)
                    {
                        finalFileName += L'\n';
                        ++curLine;
                    }
                    
                    if(curLine > m_nameMaxLinesCount + 1)
                    {
                        size_t sz = finalFileName.length();
                        
                        finalFileName[sz - 1] = L'.';
                        finalFileName[sz - 2] = L'.';
                        finalFileName[sz - 3] = L'.';
                        
                        break;
                    }
                    
                    finalFileName += c;
                    rawFinalFileName += c;
                }
                
                std::string utf8FinalFileName = SGUtils::Utils::toUTF8<char16_t>(finalFileName);
                
                ImGui::SetCursorPos(drawableNameInfo.m_position);
                ImGui::Text(utf8FinalFileName.c_str());
            }
            catch(const std::exception& e)
            {
                std::printf("Exception while drawing DirectoriesExplorer: %s\n", e.what());
            }
        }
        
        ImGui::EndChildFrame();
        ImGui::PopStyleVar(3);
    }
    
    // =================================================
    ImGui::End();
    
    ImGui::PopStyleVar(2);
}

void SGE::DirectoryExplorer::setCurrentPath(const std::filesystem::path& path) noexcept
{
    m_lastPath = m_currentPath;
    m_currentPath = path;
    
    if(!SGUtils::Utils::isSubpath(m_maxPath, path))
    {
        std::cout << "m_maxPath: " << m_maxPath << ", path: " << path << std::endl;

        m_maxPath = path;
    }
}

std::filesystem::path SGE::DirectoryExplorer::getCurrentPath() const noexcept
{
    return m_currentPath;
}
