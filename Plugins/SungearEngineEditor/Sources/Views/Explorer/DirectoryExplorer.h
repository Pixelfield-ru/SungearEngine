//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H
#define SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Memory/Assets/SVGImage.h>
#include "SGCore/Utils/Utils.h"

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
        std::int64_t m_index = 0;
        
        [[nodiscard]] const std::filesystem::path& getPath() const noexcept;
        
        void setPath(const std::filesystem::path& mPath) noexcept;
        
        [[nodiscard]] const size_t& getHashedPath() const noexcept;
        
    private:
        size_t m_hashedPath = 0;
        std::filesystem::path m_path;
    };
    
    struct FileInfoPtrCompare
    {
        bool operator()(const FileInfo* lhs, const FileInfo* rhs) const noexcept
        {
            return lhs->getHashedPath() == rhs->getHashedPath();
        }
    };
    
    struct FileInfoPtrHasher
    {
        size_t operator()(const FileInfo* fileInfo) const noexcept
        {
            return fileInfo->getHashedPath();
        }
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
    
    struct SelectedFilesDragNDropData
    {
        std::vector<std::filesystem::path> m_paths;
    };
    
    struct DirectoryExplorer : public SGCore::ImGuiWrap::IView
    {
        DirectoryExplorer();
        
        SGCore::Signal<void(SGCore::AssetRef<SGCore::ITexture2D>& iconTexture,
                           const std::string& fileExtension,
                           const std::string& fileName)> onIconRender;
        
        SGCore::Signal<void(const std::filesystem::path& filePath)> onRightClick;
        
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
                                .m_text = "New...",
                                .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                           18)->getTexture(),
                                .m_drawSeparatorAfter = true,
                                .m_elements = {
                                        SGCore::MakeRef<PopupElement>(PopupElement {
                                                .m_text = "C++ Source File",
                                                .m_ID = "New/C++ Source File",
                                                .m_icon = StylesManager::getCurrentStyle()->m_cppIcon->getSpecialization(
                                                        18, 18)->getTexture()
                                        }),
                                        SGCore::MakeRef<PopupElement>(PopupElement {
                                                .m_text = "C++ Header File",
                                                .m_ID = "New/C++ Header File",
                                                .m_icon = StylesManager::getCurrentStyle()->m_headerIcon->getSpecialization(
                                                        18, 18)->getTexture(),
                                                .m_drawSeparatorAfter = true
                                        }),
                                        SGCore::MakeRef<PopupElement>(PopupElement {
                                                .m_text = "Scene",
                                                .m_ID = "New/Scene",
                                                .m_icon = StylesManager::getCurrentStyle()->m_cubesIcon->getSpecialization(
                                                        18, 18)->getTexture()
                                        }),
                                        SGCore::MakeRef<PopupElement>(PopupElement {
                                                .m_text = "Asset...",
                                                .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                           18)->getTexture(),
                                                .m_drawSeparatorAfter = true,
                                                .m_elements = {
                                                        SGCore::MakeRef<PopupElement>(PopupElement {
                                                                .m_text = "Material",
                                                                .m_ID = "New/Asset/Material",
                                                                .m_icon = StylesManager::getCurrentStyle()->m_cubesIcon->getSpecialization(
                                                                        18, 18)->getTexture()
                                                        })
                                                }
                                        }),
                                        SGCore::MakeRef<PopupElement>(PopupElement {
                                                .m_text = "Directory",
                                                .m_ID = "New/Directory",
                                                .m_icon = StylesManager::getCurrentStyle()->m_folderIcon->getSpecialization(
                                                        18, 18)->getTexture()
                                        }),
                                }
                        },
                        {
                                .m_text = "Rename",
                                .m_hint = "Ctrl + R",
                                .m_icon = StylesManager::getCurrentStyle()->m_pencilIcon->getSpecialization(18, 18)->getTexture(),
                                .m_drawSeparatorAfter = true
                        },
                        {
                                .m_text = "Copy",
                                .m_hint = "Ctrl + C",
                                .m_icon = StylesManager::getCurrentStyle()->m_copyIcon->getSpecialization(18, 18)->getTexture(),
                        },
                        {
                                .m_text = "Paste",
                                .m_hint = "Ctrl + V",
                                .m_icon = StylesManager::getCurrentStyle()->m_pasteIcon->getSpecialization(18, 18)->getTexture(),
                                .m_drawSeparatorAfter = true
                        },
                        {
                                .m_text = "Delete",
                                .m_hint = "Delete",
                                .m_icon = StylesManager::getCurrentStyle()->m_trashBinIcon->getSpecialization(18, 18)->getTexture()
                        },
                }
        };

        [[nodiscard]] const std::string& getDragNDropPayloadName() const noexcept
        {
            return m_filesDragAndDropPayloadName;
        }

        [[nodiscard]] const auto& getSelectedFiles() const noexcept
        {
            return m_selectedFiles;
        }

    private:
        static int onFileNameEditCallback(ImGuiInputTextCallbackData* data) noexcept;
        static int onFindFileNameEditCallback(ImGuiInputTextCallbackData* data) noexcept;
        
        // TODO: MAKE STRUCT
        static inline std::int32_t m_lastCursorPositionInFileNameInputBox = 0;
        static inline std::int32_t m_lastTextLenInFileNameInputBox = 0;
        
        // =====================================================================================
        // =====================================================================================

        using separator_t = std::remove_const_t<decltype(std::filesystem::path::preferred_separator)>;
        const std::string m_utf8Separator = std::is_same_v<separator_t, char> ? std::string(1, std::filesystem::path::preferred_separator)
                                                                              : SGCore::Utils::toUTF8<separator_t>(std::basic_string<separator_t>(1, std::filesystem::path::preferred_separator));

        void renameFile(FileInfo& fileInfo) noexcept;
        
        void autoScrollWhenSelecting() noexcept;
        
        void beginMainWindow();
        
        void drawCurrentPathNavigation();
        void drawFindFilesWindow();
        
        void drawIconsAndSetupNames(bool& isAnyFileRightClicked, bool& isAnyFileHovered,
                                    std::map<std::int64_t, std::int64_t>& highlightNamesIndicesLookup,
                                    std::int64_t& currentFileIdx,
                                    std::int64_t& currentHighlightableFileIdx);
        
        void drawNamesOfFiles(std::map<std::int64_t, std::int64_t>& highlightNamesIndicesLookup);
        
        void drawFileNameInputText();
        
        void endMainWindow();
        
        // utils ====
        
        static void updateSearchResults(FileSearchResults* searchResults, const std::string& inputFileName);
        
        // =====================================================================================
        // =====================================================================================
        
        ImGuiWindow* m_filesViewWindow = nullptr;
        
        bool m_isSkippingOneFrame = false;
        
        bool m_isFilesAreaHovered = false;
        
        bool m_showFindFileChild = false;
        std::int32_t m_currentFindFileIdx = 0;
        float m_currentInputTextForFindFileIdxWidth = 25.0f;
        std::string m_findFileName;
        FileSearchResults m_filesSearchResults;
        
        std::unordered_set<FileInfo*, FileInfoPtrHasher, FileInfoPtrCompare> m_selectedFiles;
        // std::vector<FileInfo*> m_selectedFiles;
        std::vector<FileInfo> m_copyingFiles;
        
        void copySelectedFiles() noexcept;
        void pasteFiles(const std::filesystem::path& toDir) noexcept;
        
        void moveSelectedFiles(const std::filesystem::path& toPath) noexcept;
        
        void fixMaxPathRelativeToPath(const std::filesystem::path& relativeToPath) noexcept;
        
        std::filesystem::path m_currentPath;
        std::filesystem::path m_maxPath;
        
        std::filesystem::path m_rightClickedFile;
        std::int64_t m_selectedFileIdx = -1;
        // saves the last file index, that was clicked with a shift key down
        // -1 means, that no file was clicked
        std::int64_t m_shiftClickedFileIdx = -1;
        std::int64_t m_lastShiftClickedFileIdx = -1;
        
        std::filesystem::path m_currentFileOpsTargetDir;
        
        std::string m_currentEditingFileName;

        SGCore::Ref<SGCore::AssetManager> m_previewAssetManager = SGCore::AssetManager::getAssetManager("SGEDITOR_ASSETS_PREVIEW");
        
        FileInfo* m_currentEditingFile = nullptr;
        
        std::unordered_map<std::filesystem::path, FileInfo> m_drawableFilesNames;
        
        ImVec2 m_currentItemsSize { 0, 0 };
        
        bool m_isSomeFileIconHovered = false;
        
        // =======================================================================
        // files selection by mouse
        bool m_isMouseDown = false;
        bool m_isMouseSelectingFilesByQuad = false;
        bool m_isSelectionStartsInFilesWindow = false;
        ImVec2 m_selectionQuadStartPos { 0, 0 };
        ImVec2 m_selectionQuadEndPos { 0, 0 };
        
        // scroll speedup areas borders
        
        float m_leftAreaSize = 20;
        float m_topAreaSize = 20;
        float m_rightAreaSize = 20;
        float m_bottomAreaSize = 20;
        
        ImVec2 m_selectionAutoScrollMaxSpeed = { 8, 8 };
        
        // =======================================================================
        ImVec2 m_lastScroll { 0, 0 };
        ImVec2 m_currentScroll { 0, 0 };
        ImVec2 m_scrollDelta { 0, 0 };
        
        // =======================================================================
        bool m_isFilesDragging = false;
        std::string m_filesDragAndDropPayloadName = "##DirectoryExplorerDragNDrop";
        
        // =======================================================================
        FileInfo* m_draggingFileInfo = nullptr;
        
        // =======================================================================
        SGCore::Slot<void(const std::filesystem::path& byPath, bool canceled)> onSceneFileCreatedListener;
        SGCore::Slot<void(const std::filesystem::path& byPath, bool canceled)> onMaterialFileCreatedListener;
    };
}

#endif //SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H
