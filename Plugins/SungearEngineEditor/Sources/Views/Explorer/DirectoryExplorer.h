//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H
#define SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Memory/Assets/SVGImage.h>

#include "SungearEngineEditor.h"
#include "Resources.h"
#include "Views/Popup.h"
#include "Styles/StylesManager.h"

namespace SGE
{
    struct FileInfo
    {
        // IT IS NOT SCREEN SPACE POSITION
        ImVec2 m_namePosition { 0, 0 };
        ImVec2 m_nameScreenPosition { 0, 0 };
        ImVec2 m_imagePosition { 0, 0 };
        ImVec2 m_imageClickableSize { 0, 0 };
        bool m_isFullNameHovered = false;
        bool m_isIconHovered = false;
        std::string m_formattedName;
        std::filesystem::path m_path;
    };
    
    struct FoundPathEntry
    {
        std::filesystem::path m_path;
        std::u16string m_subName;
        std::string::size_type m_entryPosition = 0;
    };
    
    struct FileSearchResults
    {
        std::filesystem::path m_directoryExplorerCurrentPath;
        // if field is empty
        std::int32_t m_foundFilesCount = 0;
        std::vector<FoundPathEntry> m_foundEntries;
    };
    
    struct DirectoryExplorer : SGCore::ImGuiWrap::IView
    {
        DirectoryExplorer();
        
        SGCore::Event<void(SGCore::Ref<SGCore::ITexture2D>& iconTexture,
                           const std::string& fileExtension,
                           const std::string& fileName)> onIconRender;
        
        SGCore::Event<void(const std::filesystem::path& filePath)> onRightClick;
        
        void renderBody() override;
        
        ImVec2 m_iconsPadding = ImVec2(40, 70);
        ImVec2 m_iconsSize = ImVec2(65, 65);
        
        std::uint8_t m_nameMaxLinesCount = 3;
        
        void setCurrentPath(const std::filesystem::path& path) noexcept;
        std::filesystem::path getCurrentPath() const noexcept;
        
        Popup m_popup {
                "FilesActions",
                {
                        {
                                .m_name = "New...",
                                .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18, 18)->getTexture(),
                                .m_drawSeparatorAfter = true,
                                .m_elements = {
                                        {
                                                .m_name = "C++ Source File",
                                                .m_icon = StylesManager::getCurrentStyle()->m_cppIcon->getSpecialization(18, 18)->getTexture()
                                        },
                                        {
                                                .m_name = "C++ Header File",
                                                .m_icon = StylesManager::getCurrentStyle()->m_headerIcon->getSpecialization(18, 18)->getTexture(),
                                                .m_drawSeparatorAfter = true
                                        },
                                        {
                                                .m_name = "Directory",
                                                .m_id = "CreateNewDir",
                                                .m_icon = StylesManager::getCurrentStyle()->m_folderIcon->getSpecialization(18, 18)->getTexture()
                                        },
                                }
                        },
                        {
                                .m_name = "Rename",
                                .m_hint = "Ctrl + R",
                                .m_icon = StylesManager::getCurrentStyle()->m_pencilIcon->getSpecialization(18, 18)->getTexture(),
                                .m_drawSeparatorAfter = true
                        },
                        {
                                .m_name = "Delete",
                                .m_hint = "Delete",
                                .m_icon = StylesManager::getCurrentStyle()->m_trashBinIcon->getSpecialization(18, 18)->getTexture()
                        },
                }
        };
        
    private:
        static int onFileNameEditCallback(ImGuiInputTextCallbackData* data) noexcept;
        static int onFindFileNameEditCallback(ImGuiInputTextCallbackData* data) noexcept;
        
        // TODO: MAKE STRUCT
        static inline std::int32_t m_lastCursorPositionInFileNameInputBox = 0;
        static inline std::int32_t m_lastTextLenInFileNameInputBox = 0;
        
        // =====================================================================================
        
        void renameFile(FileInfo& fileInfo) noexcept;
        
        bool m_isSkippingOneFrame = false;
        
        bool m_isFilesAreaHovered = false;
        
        bool m_showFindFileChild = false;
        std::int32_t m_currentFindFileIdx = 0;
        std::string m_findFileName;
        FileSearchResults m_filesSearchResults;
        
        std::vector<FileInfo*> m_selectedFiles;
        
        std::filesystem::path m_currentPath;
        std::filesystem::path m_maxPath;
        
        std::filesystem::path m_rightClickedFile;
        
        std::filesystem::path m_currentFileOpsTargetDir;
        
        std::string m_currentEditingFileName;
        
        SGCore::AssetManager m_previewAssetManager;
        
        FileInfo* m_currentEditingFile = nullptr;
        
        std::unordered_map<std::filesystem::path, FileInfo> m_drawableFilesNames;
        
        ImVec2 m_currentItemsSize = ImVec2(0, 0);
    };
}

#endif //SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H
