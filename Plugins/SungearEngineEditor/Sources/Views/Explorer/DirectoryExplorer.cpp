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
    const ImVec4& frameBgCol = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
    
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
            std::filesystem::path extension = curPath.extension();
            
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
            
            auto& drawableFileNameInfo = m_drawableFilesNames[curPath];
            drawableFileNameInfo.m_position = nameStartPos;
            
            SGCore::Ref<SGCore::ITexture2D> fileIcon = ImGuiUtils::getFileIcon(curPath,
                                                    { (std::uint32_t) (m_iconsSize.x * m_UIScale.x),
                                                      (std::uint32_t) (m_iconsSize.y * m_UIScale.y) }, &onIconRender);
             
            glm::ivec2 iconSize = { (std::uint32_t) (m_iconsSize.x * m_UIScale.x), (std::uint32_t) (m_iconsSize.y * m_UIScale.y) };
            
            if(isDirectory)
            {
                fileIcon = StylesManager::getCurrentStyle()->m_folderIcon->getSpecialization(iconSize.x, iconSize.y)->getTexture();
            }
            
            if(extension == ".png" || extension == ".jpg" || extension == ".jpeg")
            {
                bool previewExists = m_previewAssetManager.isAssetExists<SGCore::ITexture2D>(curPath);
                fileIcon = SGCore::Ref<SGCore::ITexture2D>(SGCore::CoreMain::getRenderer()->createTexture2D());
                fileIcon->onLazyLoadDone += [previewExists, iconSize](SGCore::IAsset* self) {
                    if(!previewExists)
                    {
                        auto* tex = (SGCore::ITexture2D*) self;
                        
                        if(tex->getWidth() > tex->getHeight())
                        {
                            float ratio = (float) tex->getWidth() / (float) tex->getHeight();
                            
                            tex->resize(iconSize.x, (std::int32_t) (iconSize.y / ratio));
                        }
                        else if(tex->getHeight() > tex->getWidth())
                        {
                            float ratio = (float) tex->getHeight() / (float) tex->getWidth();
                            
                            tex->resize((std::int32_t) (iconSize.x / ratio), iconSize.y);
                        }
                        else
                        {
                            tex->resize(iconSize.x, iconSize.y);
                        }
                    }
                };
                m_previewAssetManager.loadAsset<SGCore::ITexture2D>(fileIcon, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, curPath);
            }
            
            glm::ivec2 requiredIconSize { (std::uint32_t) (m_iconsSize.x * m_UIScale.x),
                                          (std::uint32_t) (m_iconsSize.y * m_UIScale.y) };
            
            glm::ivec2 hoverOffset =
                    (requiredIconSize - glm::ivec2 { fileIcon->getWidth(), fileIcon->getHeight() }) / 2 + glm::ivec2 { 3, 3 };
            
            cursorPos = ImGui::GetCursorPos();
            ImGui::SetCursorPos({ cursorPos.x + hoverOffset.x, cursorPos.y + hoverOffset.y });
            
            ImClickInfo clickInfo = ImGuiUtils::ImageButton(fileIcon->getTextureNativeHandler(),
                                                            { (float) fileIcon->getWidth(),
                                                              (float) fileIcon->getHeight() },
                                                            ImVec2(-hoverOffset.x, -hoverOffset.y),
                                                            ImVec2(hoverOffset.x, hoverOffset.y));
            
            // ImGui::SetCursorPos(cursorPos);
            
            drawableFileNameInfo.m_isIconHovered = clickInfo.m_isHovered;
            
            if(isDirectory && clickInfo.m_isDoubleClicked)
            {
                setCurrentPath(curPath);
                break;
            }

            m_currentItemsSize = ImVec2(m_iconsSize.x * m_UIScale.x + 3 * 2, m_iconsSize.y * m_UIScale.y + 3 * 2);
            
            ImGui::SameLine();
        }
        
        for(auto& fileNameInfoPair : m_drawableFilesNames)
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
                std::u16string finalTransferredFileName;
                
                size_t curLine = 0;
                for(wchar_t c : fullName)
                {
                    ImVec2 curNameSize = ImGui::CalcTextSize(SGUtils::Utils::toUTF8<char16_t>(finalFileName).c_str());
                    ImVec2 curFullNameSize = ImGui::CalcTextSize(SGUtils::Utils::toUTF8<char16_t>(finalTransferredFileName).c_str());
                    
                    if(curNameSize.x > maxNameSize.x && curLine <= m_nameMaxLinesCount)
                    {
                        auto sz = finalFileName.size() - 1;
                        auto lastChar = finalFileName[sz];
                        finalFileName[sz] = L'\n';
                        finalFileName += lastChar;
                        
                        ++curLine;
                    }
                    
                    if(curFullNameSize.x > maxNameSize.x)
                    {
                        auto sz = finalTransferredFileName.size() - 1;
                        auto lastChar = finalTransferredFileName[sz];
                        finalTransferredFileName[sz] = L'\n';
                        finalTransferredFileName += lastChar;
                        
                        ++curLine;
                    }
                    
                    if(curLine == m_nameMaxLinesCount + 1)
                    {
                        size_t sz = finalFileName.length();
                        
                        finalFileName[sz - 1] = L'.';
                        finalFileName[sz - 2] = L'.';
                        finalFileName[sz - 3] = L'.';
                        
                        finalTransferredFileName += c;
                        
                        continue;
                    }
                    
                    if(curLine <= m_nameMaxLinesCount)
                    {
                        finalFileName += c;
                    }
                    
                    finalTransferredFileName += c;
                }
                
                std::string utf8FinalFileName = SGUtils::Utils::toUTF8<char16_t>(finalFileName);
                std::string utf8TransferredFileName = SGUtils::Utils::toUTF8<char16_t>(finalTransferredFileName);
                
                ImGui::SetCursorPos(drawableNameInfo.m_position);
                ImVec2 finalTextSize = ImGui::CalcTextSize(utf8FinalFileName.c_str());
                ImVec2 finalTransferredTextSize = ImGui::CalcTextSize(utf8TransferredFileName.c_str());
                ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
                
                // check if mouse hovering file name. if hovering then we`ll show the full transferred name. if not then we`ll show abbreviated file name
                if(ImGui::IsMouseHoveringRect(cursorScreenPos, { cursorScreenPos.x + finalTextSize.x,
                                                                 cursorScreenPos.y + finalTextSize.y }) ||
                   drawableNameInfo.m_isIconHovered || drawableNameInfo.m_isFullNameHovered)
                {
                    drawableNameInfo.m_isFullNameHovered = ImGui::IsMouseHoveringRect(cursorScreenPos,
                                                                                      { cursorScreenPos.x + finalTransferredTextSize.x,
                                                                                        cursorScreenPos.y + finalTransferredTextSize.y });
                    
                    ImGui::GetWindowDrawList()->AddRectFilled(cursorScreenPos,
                                                              { cursorScreenPos.x + finalTransferredTextSize.x,
                                                                cursorScreenPos.y + finalTransferredTextSize.y },
                                                              ImGui::ColorConvertFloat4ToU32(frameBgCol));
                    
                    ImGui::GetWindowDrawList()->AddText(cursorScreenPos,
                                                        ImGui::ColorConvertFloat4ToU32({ 1.0, 1.0, 1.0, 1.0 }),
                                                        utf8TransferredFileName.c_str());
                    
                    // TODO: MAKE TEXT IN THE BOTTOM OF WINDOW
                    /*if(ImGui::GetWindowContentRegionMax().y + ImGui::GetWindowPos().y < drawableNameInfo.m_position.y + finalTransferredTextSize.y)
                    {
                        float diff = (drawableNameInfo.m_position.y + finalTransferredTextSize.y) - ImGui::GetScrollY();
                        
                        ImVec2 lastPos = ImGui::GetCursorPos();
                        ImGui::SetCursorPos({ lastPos.x, lastPos.y + finalTransferredTextSize.y});
                        ImGui::Dummy({ finalTransferredTextSize.x, diff });
                        ImGui::SetCursorPos(lastPos);
                    }*/
                }
                else
                {
                    ImGui::Text(utf8FinalFileName.c_str());
                }
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
    
    m_drawableFilesNames.clear();
    m_previewAssetManager.clear();
    
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
