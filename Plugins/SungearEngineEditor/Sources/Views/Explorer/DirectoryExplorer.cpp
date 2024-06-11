//
// Created by ilya on 30.04.24.
//

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include <SGUtils/Utils.h>

#include "DirectoryExplorer.h"
#include "Views/MainView.h"
#include "DirectoriesTreeExplorer.h"
#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"

SGE::DirectoryExplorer::DirectoryExplorer()
{
    onRightClick += [this](const std::filesystem::path& filePath) {
        bool isOneFileSelected = m_selectedFiles.size() <= 1;
        auto* newFilesElem = m_popup.tryGetElement("New...");
        
        if(std::filesystem::is_directory(filePath) && isOneFileSelected)
        {
            newFilesElem->setAllElementsActive(true);
            newFilesElem->m_drawSeparatorAfter = true;
        }
        
        if(m_currentPath != filePath)
        {
            if(isOneFileSelected)
            {
                m_popup.tryGetElement("Rename")->m_isActive = true;
            }
            m_popup.tryGetElement("Delete")->m_isActive = true;
        }
        else
        {
            newFilesElem->m_drawSeparatorAfter = false;
        }
    };
    
    m_popup.onElementClicked += [this](PopupElement& element) {
        bool rightClickedFileExists = std::filesystem::exists(m_rightClickedFile);
        
        if(rightClickedFileExists)
        {
            auto& fileInfo = m_drawableFilesNames[m_rightClickedFile];
            if(element.m_id == "Rename")
            {
                renameFile(fileInfo);
            }
            else if(element.m_id == "Delete")
            {
                if(&fileInfo == m_currentEditingFile)
                {
                    m_currentEditingFile = nullptr;
                }
                
                if(m_selectedFiles.size() > 1)
                {
                    for(auto* fi : m_selectedFiles)
                    {
                        std::filesystem::remove_all(fi->m_path);
                    }
                    
                    m_selectedFiles.clear();
                }
                else
                {
                    std::filesystem::remove_all(m_rightClickedFile);
                    m_rightClickedFile = "";
                }
                
                m_drawableFilesNames.clear();
                
                std::erase_if(m_selectedFiles, [&fileInfo](const FileInfo* fi) {
                    return fi == &fileInfo;
                });
            }
        }
        
        if(element.m_id == "C++ Source File")
        {
            std::string utf8Path = SGUtils::Utils::toUTF8<char16_t>(m_currentFileOpsTargetDir.u16string());
            
            auto fileCreateDialog = SungearEngineEditor::getInstance()->getMainView()->getTopToolbarView()->m_fileCreateDialog;
            fileCreateDialog->m_dialogTitle = "New C++ Source File";
            fileCreateDialog->m_defaultPath = utf8Path;
            fileCreateDialog->m_currentChosenDirPath = utf8Path;
            fileCreateDialog->m_ext = ".cpp";
            fileCreateDialog->m_isCreatingDirectory = false;
            fileCreateDialog->setActive(true);
        }
        else if(element.m_id == "C++ Header File")
        {
            std::string utf8Path = SGUtils::Utils::toUTF8<char16_t>(m_currentFileOpsTargetDir.u16string());
            
            auto fileCreateDialog = SungearEngineEditor::getInstance()->getMainView()->getTopToolbarView()->m_fileCreateDialog;
            fileCreateDialog->m_dialogTitle = "New C++ Header File";
            fileCreateDialog->m_defaultPath = utf8Path;
            fileCreateDialog->m_currentChosenDirPath = utf8Path;
            fileCreateDialog->m_ext = ".h";
            fileCreateDialog->m_isCreatingDirectory = false;
            fileCreateDialog->setActive(true);
        }
        else if(element.m_id == "CreateNewDir")
        {
            std::string utf8Path = SGUtils::Utils::toUTF8<char16_t>(m_currentFileOpsTargetDir.u16string());
            
            auto fileCreateDialog = SungearEngineEditor::getInstance()->getMainView()->getTopToolbarView()->m_fileCreateDialog;
            fileCreateDialog->m_dialogTitle = "New Directory";
            fileCreateDialog->m_defaultPath = utf8Path;
            fileCreateDialog->m_currentChosenDirPath = utf8Path;
            fileCreateDialog->m_ext = "";
            fileCreateDialog->m_isCreatingDirectory = true;
            fileCreateDialog->setActive(true);
        }
    };
}

void SGE::DirectoryExplorer::renderBody()
{
    if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_F))
    {
        m_showFindFileChild = true;
    }
    
    const ImVec4& frameBgCol = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);
    
    ImGui::Begin("DirectoryExplorer", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    
    ImVec2 directoryExplorerWndSize = ImGui::GetWindowSize();
    
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
                
                bool isWindowHovered = ImGui::IsWindowHovered();
                
                if(isHovering && mouseClicked && isWindowHovered)
                {
                    setCurrentPath(concatPathCanonical);
                }
                
                if(concatPathCanonical == m_currentPath)
                {
                    ImGui::TextColored(ImVec4(50 / 255.0f, 120 / 255.0f, 170 / 255.0f, 1.0f), u8DirName.c_str());
                }
                else if(isHovering && isWindowHovered)
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
        
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 8, 5 });
        
        if(m_showFindFileChild)
        {
            ImGui::BeginChildFrame(ImGui::GetID("DirectoryExplorerFindFile"),
                                   { ImGui::GetContentRegionAvail().x, 35 },
                                   ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
            
            ImVec2 cursorStartPos = ImGui::GetCursorScreenPos();
            
            if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_F))
            {
                ImGui::SetKeyboardFocusHere(0);
            }
            
            if (ImGui::IsWindowAppearing())
            {
                ImGui::SetKeyboardFocusHere(0);
            }
            
            ImVec2 findFileWndSize = ImGui::GetWindowSize();
            
            //ImGui::SetWindowPos({ directoryExplorerWndSize.x - 50, 0 });
            
            m_filesSearchResults.m_directoryExplorerCurrentPath = m_currentPath;
   
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
            bool noFilesFound = m_filesSearchResults.m_foundFilesCount == 0;
            if(noFilesFound)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, { 1, 0, 0, 1 });
                ImGui::PushStyleColor(ImGuiCol_TextDisabled, { 1, 0, 0, 1 });
            }
            ImGui::SetNextItemWidth(200);
            ImGui::InputTextWithHint("##DirectoryExplorer_FindFile", "Search...",
                                     &m_findFileName, ImGuiInputTextFlags_CallbackAlways,
                                     onFindFileNameEditCallback,
                                     &m_filesSearchResults);
            
            if(noFilesFound)
            {
                ImGui::PopStyleColor(2);
            }
            ImGui::PopStyleColor(1);
            
            ImGui::SameLine();
            
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (findFileWndSize.y - 9 * 2 - ImGui::GetStyle().FramePadding.y) / 2);
            
            ImClickInfo clearFindNameBtnClickInfo = ImGuiUtils::ImageButton(StylesManager::getCurrentStyle()
                                                                                    ->m_crossIcon
                                                                                    ->getSpecialization(16, 16)
                                                                                    ->getTexture()
                                                                                    ->getTextureNativeHandler(), 9,
                                                                            { 16, 16 });
            
            ImGui::GetForegroundDrawList()->AddLine({ cursorStartPos.x + 230, cursorStartPos.y - 7 },
                                                { cursorStartPos.x + 230, cursorStartPos.y + findFileWndSize.y - 2 },
                                                ImGui::ColorConvertFloat4ToU32({ 0.3, 0.3, 0.3, 1.0 }));
            
            ImGui::SameLine();
            ImGui::SetCursorPosX(250);
            std::int32_t foundFilesCnt = std::max(m_filesSearchResults.m_foundFilesCount, 0);
            if(foundFilesCnt == 0)
            {
                if(m_filesSearchResults.m_foundFilesCount != -1)
                {
                    ImGui::PushStyleColor(ImGuiCol_TextDisabled, { 1, 0, 0, 1 });
                }
                ImGui::TextDisabled("0 results");
                if(m_filesSearchResults.m_foundFilesCount != -1)
                {
                    ImGui::PopStyleColor();
                }
            }
            else
            {
                ImGui::TextDisabled("%i/%i", m_currentFindFileIdx, std::max(m_filesSearchResults.m_foundFilesCount, 0));
            }
            
            if(clearFindNameBtnClickInfo.m_isLMBClicked)
            {
                m_findFileName = "";
                m_filesSearchResults.m_foundFilesCount = -1;
            }
            
            ImGui::EndChildFrame();
            
            ImGui::Separator();
            
            if(ImGui::IsKeyPressed(ImGuiKey_Escape))
            {
                m_showFindFileChild = false;
                m_isSkippingOneFrame = true;
            }
            
            // ImGui::EndPopup();
        }
        
        ImGui::PopStyleVar();
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 8, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(m_iconsPadding.x * m_UIScale.x, m_iconsPadding.y * m_UIScale.y));
        
        ImGui::BeginChildFrame(ImGui::GetID("DirectoryExplorerFilesView"),
                               ImGui::GetContentRegionAvail(),
                               ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
        
        m_isFilesAreaHovered = ImGui::IsWindowHovered();
        
        if(ImGui::IsKeyReleased(ImGuiKey_Delete) && m_isFilesAreaHovered)
        {
            for(auto* fileInfo : m_selectedFiles)
            {
                std::filesystem::remove_all(fileInfo->m_path);
            }
            
            m_drawableFilesNames.clear();
            m_selectedFiles.clear();
        }
        
        // HOT KEYS FOR RENAMING
        if((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyReleased(ImGuiKey_R) && m_isFilesAreaHovered)
        {
            if(m_selectedFiles.size() == 1)
            {
                renameFile(*m_selectedFiles[0]);
            }
        }
        
        for(auto* fileInfo : m_selectedFiles)
        {
            ImGui::GetWindowDrawList()->AddRectFilled(
                    ImVec2(fileInfo->m_imagePosition.x, fileInfo->m_imagePosition.y),
                    ImVec2(fileInfo->m_imagePosition.x + fileInfo->m_imageClickableSize.x, fileInfo->m_imagePosition.y + fileInfo->m_imageClickableSize.y),
                    ImGui::ColorConvertFloat4ToU32(ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1)), 3);
        }
        
        // ================ DRAWING POPUPS ==================
        
        m_popup.draw();
        
        // ==================================================
        
        bool isAnyFileRightClicked = false;
        bool isAnyFileHovered = false;
        
        for(auto it = std::filesystem::directory_iterator(m_currentPath);
            it != std::filesystem::directory_iterator(); ++it)
        {
            std::filesystem::path curPath = *it;
            std::filesystem::path extension = curPath.extension();
            std::string u8curPath = SGUtils::Utils::toUTF8<char16_t>(curPath.u16string());
            
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
            drawableFileNameInfo.m_namePosition = nameStartPos;
            drawableFileNameInfo.m_nameScreenPosition = ImGui::GetCursorScreenPos();
            drawableFileNameInfo.m_nameScreenPosition.y += m_iconsSize.y * m_UIScale.y;
            
            ImVec2 requiredIconSize { (m_iconsSize.x * m_UIScale.x),
                                      (m_iconsSize.y * m_UIScale.y) };
            
            SGCore::Ref<SGCore::ITexture2D> fileIcon = ImGuiUtils::getFileIcon(curPath,
                                                                               { (std::uint32_t) requiredIconSize.x,
                                                                                 (std::uint32_t) requiredIconSize.y },
                                                                               &onIconRender);
             
            glm::ivec2 iconSize = { (std::uint32_t) (m_iconsSize.x * m_UIScale.x), (std::uint32_t) (m_iconsSize.y * m_UIScale.y) };
            const ImVec2 iconPadding { 3, 3 };
            
            if(isDirectory)
            {
                fileIcon = StylesManager::getCurrentStyle()->m_folderIcon->getSpecialization(iconSize.x, iconSize.y)->getTexture();
            }
            
            if(extension == ".png" || extension == ".jpg" || extension == ".jpeg")
            {
                bool previewExists = m_previewAssetManager.isAssetExists<SGCore::ITexture2D>(u8curPath);
                fileIcon = SGCore::Ref<SGCore::ITexture2D>(SGCore::CoreMain::getRenderer()->createTexture2D());
                fileIcon->onLazyLoadDone += [previewExists, iconSize, iconPadding](SGCore::IAsset* self) {
                    if(!previewExists)
                    {
                        // NEXT WE ARE COMPRESSING TEXTURE SAVING ITS ASPECT RATIO
                        auto* tex = (SGCore::ITexture2D*) self;
                        
                        if(tex->getWidth() > tex->getHeight())
                        {
                            float ratio = (float) tex->getWidth() / (float) tex->getHeight();
                            
                            tex->resize(iconSize.x - iconPadding.x * 2,
                                        std::max<std::int32_t>((std::int32_t) ((float) iconSize.y / ratio) - iconPadding.y * 2, 1));
                        }
                        else if(tex->getHeight() > tex->getWidth())
                        {
                            float ratio = (float) tex->getHeight() / (float) tex->getWidth();
                            
                            tex->resize(std::max<std::int32_t>((std::int32_t) ((float) iconSize.x / ratio) - iconPadding.x * 2, 1),
                                        iconSize.y - iconPadding.y * 2);
                        }
                        else
                        {
                            tex->resize(iconSize.x - iconPadding.x * 2, iconSize.y - iconPadding.y * 2);
                        }
                    }
                };
                
                m_previewAssetManager.loadAsset<SGCore::ITexture2D>(fileIcon,
                                                                    SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD,
                                                                    u8curPath);
            }
            
            ImClickInfo clickInfo = ImGuiUtils::ImageButton(fileIcon->getTextureNativeHandler(),
                                                            { (float) requiredIconSize.x, (float) requiredIconSize.y },
                                                            { (float) fileIcon->getWidth(),
                                                              (float) fileIcon->getHeight() });
            
            // ImGui::SetCursorPos(cursorPos);
            
            drawableFileNameInfo.m_isIconHovered = clickInfo.m_isHovered;
            
            drawableFileNameInfo.m_imagePosition = clickInfo.m_elementPosition;
            drawableFileNameInfo.m_imageClickableSize = clickInfo.m_elementClickableSize;
            
            if(isDirectory)
            {
                if(clickInfo.m_isLMBDoubleClicked)
                {
                    // MOVING TO TARGET DOUBLE-CLICKED DIRECTORY
                    m_popup.setOpened(false);
                    setCurrentPath(curPath);
                    break;
                }
            }
            
            if(clickInfo.m_isHovered && m_isFilesAreaHovered)
            {
                isAnyFileHovered = true;
            }
            
            if(clickInfo.m_isRMBClicked)
            {
                isAnyFileRightClicked = true;
                
                m_rightClickedFile = curPath;
                
                m_popup.setOpened(true);
                m_popup.setAllElementsActive(false);
                
                m_currentFileOpsTargetDir = m_rightClickedFile;
                
                onRightClick(m_rightClickedFile);
            }
            
            if(clickInfo.m_isLMBClicked)
            {
                bool leftCtrlDown = ImGui::IsKeyDown(ImGuiKey_LeftCtrl);
                
                if(!leftCtrlDown)
                {
                    m_selectedFiles.clear();
                }
                
                auto foundInfo = std::find_if(m_selectedFiles.begin(), m_selectedFiles.end(), [&drawableFileNameInfo](const FileInfo* fileInfo) {
                    return fileInfo == &drawableFileNameInfo;
                });
                
                if(foundInfo != m_selectedFiles.end())
                {
                    m_selectedFiles.erase(foundInfo);
                }
                else
                {
                    m_selectedFiles.push_back(&drawableFileNameInfo);
                }
            }

            m_currentItemsSize = ImVec2(m_iconsSize.x * m_UIScale.x + 3 * 2, m_iconsSize.y * m_UIScale.y + 3 * 2);
            
            ImGui::SameLine();
        }
        
        // IF MOUSE RIGHT-CLICKED ON FILES AREA BUT NOT ON ANY FILE
        if(!isAnyFileRightClicked && m_isFilesAreaHovered)
        {
            if(ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                m_selectedFiles.clear();
                
                // OPENING POPUP AND DISPLAY ACTIONS THAT CAN BE USED
                m_popup.setOpened(true);
                m_popup.setAllElementsActive(false);
                
                m_currentFileOpsTargetDir = m_currentPath;
                
                onRightClick(m_currentPath);
            }
        }
        
        if(!isAnyFileHovered && m_isFilesAreaHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            m_selectedFiles.clear();
        }
        
        if(m_isFilesAreaHovered && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape) && !m_isSkippingOneFrame)
        {
            m_selectedFiles.clear();
        }
        
        for(auto& fileNameInfoPair : m_drawableFilesNames)
        {
            try
            {
                auto& path = fileNameInfoPair.first;
                if(!std::filesystem::exists(path)) continue;
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
                
                std::vector<ImRect> foundFilesRects;
                
                // FINDING THIS FILE IN SEARCH RESULTS ==============================
                
                auto foundFileIt = std::find_if(m_filesSearchResults.m_foundEntries.begin(),
                                                m_filesSearchResults.m_foundEntries.end(),
                                                [&path](const FoundPathEntry& foundPathEntry) {
                                                    return foundPathEntry.m_path == path;
                                                });
                
                // ==================================================================
                
                ImFont* font = ImGui::GetFont();
                
                size_t curLine = 0;
                std::string::size_type curCharIdx = 0;
                ImVec2 charPosOffset { };
                
                for(char16_t c : fullName)
                {
                    std::string utf8CharStr = SGUtils::Utils::toUTF8<char16_t>({ c });
                    
                    ImVec2 charSize = ImGui::CalcTextSize(utf8CharStr.c_str());
                    charSize.x -= charSize.x - font->GetCharAdvance(c);
                    
                    ImVec2 curNameSize = ImGui::CalcTextSize(SGUtils::Utils::toUTF8<char16_t>(finalFileName).c_str());
                    ImVec2 curFullNameSize = ImGui::CalcTextSize(SGUtils::Utils::toUTF8<char16_t>(finalTransferredFileName).c_str());
                    
                    if(curNameSize.x > maxNameSize.x && curLine <= m_nameMaxLinesCount)
                    {
                        auto sz = finalFileName.length() - 1;
                        auto lastChar = finalFileName[sz];
                        finalFileName[sz] = L'\n';
                        finalFileName += lastChar;
                        
                        std::string utf8LastCharStr = SGUtils::Utils::toUTF8<char16_t>({ lastChar });
                        
                        ImVec2 prvCharSize = ImGui::CalcTextSize(utf8LastCharStr.c_str());
                        prvCharSize.x -= prvCharSize.x - font->GetCharAdvance(lastChar);
                        
                        charPosOffset.y += curNameSize.y;
                        charPosOffset.x = 0;
                        
                        if(foundFileIt != m_filesSearchResults.m_foundEntries.end())
                        {
                            if(curCharIdx - 1 >= foundFileIt->m_entryPosition &&
                               curCharIdx - 1 < foundFileIt->m_entryPosition + foundFileIt->m_subName.length())
                            {
                                auto& prevRect = *foundFilesRects.rbegin();
                                prevRect.Min =
                                        { drawableNameInfo.m_nameScreenPosition.x + charPosOffset.x,
                                          drawableNameInfo.m_nameScreenPosition.y + charPosOffset.y };
                                
                                prevRect.Max =
                                        { prevRect.Min.x + prvCharSize.x,
                                          prevRect.Min.y + prvCharSize.y };
                            }
                            
                            charPosOffset.x += prvCharSize.x;
                            
                            if(curCharIdx >= foundFileIt->m_entryPosition &&
                               curCharIdx < foundFileIt->m_entryPosition + foundFileIt->m_subName.length())
                            {
                                ImRect foundFileHighlightRect;
                                foundFileHighlightRect.Min = {
                                        drawableNameInfo.m_nameScreenPosition.x + charPosOffset.x,
                                        drawableNameInfo.m_nameScreenPosition.y + charPosOffset.y };
                                foundFileHighlightRect.Max = { foundFileHighlightRect.Min.x + charSize.x,
                                                               foundFileHighlightRect.Min.y + charSize.y };
                                foundFilesRects.push_back(foundFileHighlightRect);
                            }
                        }
                        
                        ++curLine;
                    }
                    else
                    {
                        if(foundFileIt != m_filesSearchResults.m_foundEntries.end())
                        {
                            if(curCharIdx >= foundFileIt->m_entryPosition &&
                               curCharIdx < foundFileIt->m_entryPosition + foundFileIt->m_subName.length())
                            {
                                ImRect foundFileHighlightRect;
                                foundFileHighlightRect.Min = {
                                        drawableNameInfo.m_nameScreenPosition.x + charPosOffset.x,
                                        drawableNameInfo.m_nameScreenPosition.y + charPosOffset.y };
                                foundFileHighlightRect.Max = { foundFileHighlightRect.Min.x + charSize.x,
                                                               foundFileHighlightRect.Min.y + charSize.y };
                                foundFilesRects.push_back(foundFileHighlightRect);
                            }
                        }
                    }
                    
                    if(curFullNameSize.x > maxNameSize.x)
                    {
                        auto sz = finalTransferredFileName.length() - 1;
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
                        ++curCharIdx;
                        finalFileName += c;
                        charPosOffset.x += charSize.x;
                    }
                    
                    finalTransferredFileName += c;
                }
                
                for(const ImRect& rect : foundFilesRects)
                {
                    ImGui::GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, ImGui::ColorConvertFloat4ToU32({ 0, 1, 0, 0.4 }));
                }
                
                std::string utf8FinalFileName = SGUtils::Utils::toUTF8<char16_t>(finalFileName);
                std::string utf8TransferredFileName = SGUtils::Utils::toUTF8<char16_t>(finalTransferredFileName);
                
                drawableNameInfo.m_formattedName = utf8TransferredFileName;
                drawableNameInfo.m_path = path;
                
                ImGui::SetCursorPos(drawableNameInfo.m_namePosition);
                ImVec2 finalTextSize = ImGui::CalcTextSize(utf8FinalFileName.c_str());
                ImVec2 finalTransferredTextSize = ImGui::CalcTextSize(utf8TransferredFileName.c_str());
                ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
                
                if(m_currentEditingFile != &drawableNameInfo)
                {
                    
                    
                    // check if mouse hovering file name. if hovering then we`ll show the full transferred name. if not then we`ll show abbreviated file name
                    if(ImGui::IsMouseHoveringRect(cursorScreenPos, { cursorScreenPos.x + finalTextSize.x,
                                                                     cursorScreenPos.y + finalTextSize.y }) ||
                       drawableNameInfo.m_isIconHovered || drawableNameInfo.m_isFullNameHovered)
                    {
                        drawableNameInfo.m_isFullNameHovered = ImGui::IsMouseHoveringRect(cursorScreenPos,
                                                                                          { cursorScreenPos.x +
                                                                                            finalTransferredTextSize.x,
                                                                                            cursorScreenPos.y +
                                                                                            finalTransferredTextSize.y });
                        
                        ImGui::GetWindowDrawList()->AddRectFilled(cursorScreenPos,
                                                                  { cursorScreenPos.x + finalTransferredTextSize.x,
                                                                    cursorScreenPos.y + finalTransferredTextSize.y },
                                                                  ImGui::ColorConvertFloat4ToU32(frameBgCol));
                        
                        ImGui::GetWindowDrawList()->AddText(cursorScreenPos,
                                                            ImGui::ColorConvertFloat4ToU32({ 1.0, 1.0, 1.0, 1.0 }),
                                                            utf8TransferredFileName.c_str());
                        
                        if(ImGui::IsMouseHoveringRect(cursorScreenPos,
                                                      { cursorScreenPos.x + finalTransferredTextSize.x,
                                                        cursorScreenPos.y + finalTransferredTextSize.y }) &&
                           ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                        {
                            renameFile(drawableNameInfo);
                        }
                        
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
            }
            catch(const std::exception& e)
            {
                std::printf("Exception while drawing DirectoriesExplorer: %s\n", e.what());
            }
        }
        
        // WE ARE ITERATING THROUGH INFOS TO CHECK CURRENT EDITING FILE AND OVERDRAW INPUT TEXT
        for(auto& fileNameInfoPair : m_drawableFilesNames)
        {
            auto& path = fileNameInfoPair.first;
            auto& drawableNameInfo = fileNameInfoPair.second;
            
            if(m_currentEditingFile == &drawableNameInfo)
            {
                ImVec2 nameSize = ImGui::CalcTextSize(m_currentEditingFileName.c_str());
                
                // y = 3 lines
                ImVec2 maxNameSize = ImVec2(m_iconsSize.x * m_UIScale.x + 3 * 2 + m_iconsPadding.x / 4 - ImGui::GetStyle().FramePadding.x + 7,
                                            nameSize.y);
                
                ImGui::SetCursorPos(drawableNameInfo.m_namePosition);
                ImGui::InputTextMultiline(("##" + drawableNameInfo.m_formattedName).c_str(),
                                          &m_currentEditingFileName,
                                          { maxNameSize.x +
                                            ImGui::GetStyle().FramePadding.x + 13,
                                            nameSize.y },
                                          ImGuiInputTextFlags_CallbackAlways,
                                          onFileNameEditCallback,
                                          (void*) &maxNameSize.x);
                
                if(!m_isSkippingOneFrame)
                {
                    if(ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape) ||
                    (!ImGui::IsItemHovered() &&
                    (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right))))
                    {
                        m_currentEditingFile = nullptr;
                    }
                }
                
                if(ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Enter))
                {
                    std::filesystem::path resultPath = drawableNameInfo.m_path.parent_path();
                    resultPath += std::filesystem::path::preferred_separator + SGUtils::Utils::replaceAll<char>(m_currentEditingFileName, "\n", "");
                    std::filesystem::rename(drawableNameInfo.m_path, resultPath);
                    m_currentEditingFile = nullptr;
                }
                
                drawableNameInfo.m_formattedName = m_currentEditingFileName;
            }
        }
        
        ImGui::EndChildFrame();
        ImGui::PopStyleVar(3);
    }
    
    // =================================================
    ImGui::End();
    
    ImGui::PopStyleVar(2);
    
    // =================================================
    m_isSkippingOneFrame = false;
}

void SGE::DirectoryExplorer::setCurrentPath(const std::filesystem::path& path) noexcept
{
    m_currentPath = path;
    
    m_drawableFilesNames.clear();
    m_previewAssetManager.clear();
    m_currentEditingFile = nullptr;
    
    m_selectedFiles.clear();
    
    m_rightClickedFile = "";
    
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

int SGE::DirectoryExplorer::onFileNameEditCallback(ImGuiInputTextCallbackData* data) noexcept
{
    std::int32_t dif = data->CursorPos - m_lastCursorPositionInFileNameInputBox;
    
    // if cursor just moving =================
    // if cursor going on left
    /*if(dif < 0 && m_lastTextLenInFileNameInputBox == data->BufTextLen)
    {
        if(data->Buf[data->CursorPos] == '\n')
        {
            --data->CursorPos;
        }
    }
    else if(dif > 0 && m_lastTextLenInFileNameInputBox == data->BufTextLen) // if cursor going on right
    {
        if(data->Buf[data->CursorPos - 1] == '\n')
        {
            ++data->CursorPos;
        }
    }*/
    // =========================================
    
    float controlWidth = *(float*) data->UserData;
    float textWidth = ImGui::CalcTextSize(data->Buf).x;
    
    int cursorPos = data->CursorPos;
    
    std::u16string strBuf = SGUtils::Utils::fromUTF8<char16_t>(std::string(data->Buf, data->BufTextLen));
    std::u16string finalTransferredFileName;
    
    strBuf = SGUtils::Utils::replaceAll<char16_t>(strBuf, u"\n", u"");
    
    for(char16_t c : strBuf)
    {
        ImVec2 curFullNameSize = ImGui::CalcTextSize(SGUtils::Utils::toUTF8(finalTransferredFileName + c).c_str());
        
        if(curFullNameSize.x >= controlWidth)
        {
            finalTransferredFileName += u'\n';
        }
        
        finalTransferredFileName += c;
    }
    
    std::string utf8Str = SGUtils::Utils::toUTF8(finalTransferredFileName);
    
    bool prettified = false;
    if(textWidth > controlWidth || m_lastTextLenInFileNameInputBox != data->BufTextLen)
    {
        data->DeleteChars(0, data->BufTextLen);
        data->InsertChars(0, utf8Str.c_str());
        
        data->CursorPos = cursorPos;
        
        data->BufDirty = true;
        prettified = true;
    }
    
    if(prettified && data->Buf[data->CursorPos - 1] == '\n')
    {
        ++data->CursorPos;
    }
    
    m_lastTextLenInFileNameInputBox = data->BufTextLen;
    m_lastCursorPositionInFileNameInputBox = data->CursorPos;
    
    return 0;
}

void SGE::DirectoryExplorer::renameFile(FileInfo& fileInfo) noexcept
{
    m_currentEditingFileName = fileInfo.m_formattedName;
    m_currentEditingFile = &fileInfo;
    m_isSkippingOneFrame = true;
}

int SGE::DirectoryExplorer::onFindFileNameEditCallback(ImGuiInputTextCallbackData* data) noexcept
{
    std::string inputString = data->Buf;
    
    auto* searchResults = (FileSearchResults*) data->UserData;
    searchResults->m_foundFilesCount = 0;
    searchResults->m_foundEntries.clear();
    
    // std::cout << inputString << std::endl;
    
    for(auto it = std::filesystem::directory_iterator(searchResults->m_directoryExplorerCurrentPath);
        it != std::filesystem::directory_iterator(); ++it)
    {
        const std::filesystem::path& filePath = *it;
        
        std::string utf8Name = SGUtils::Utils::toUTF8<char16_t>(filePath.filename().u16string());
        
        std::string::size_type substrPos = SGUtils::Utils::findInString(filePath.filename().u16string(), SGUtils::Utils::fromUTF8<char16_t>(inputString), true);
        if(substrPos != std::string::npos)
        {
            searchResults->m_foundEntries.emplace_back(filePath, SGUtils::Utils::fromUTF8<char16_t>(inputString), substrPos);
            ++searchResults->m_foundFilesCount;
        }
    }
    
    if(data->BufTextLen == 0)
    {
        searchResults->m_foundFilesCount = -1;
    }
    
    return 0;
}
