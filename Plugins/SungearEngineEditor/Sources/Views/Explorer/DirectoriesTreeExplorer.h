//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_DIRECTORIESTREEEXPLORER_H
#define SUNGEARENGINEEDITOR_DIRECTORIESTREEEXPLORER_H

#include "SungearEngineEditor.h"
#include "Resources.h"

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Main/CoreGlobals.h>

namespace SGE
{
    struct DirectoriesTreeExplorer : SGCore::ImGuiWrap::IView
    {
        std::filesystem::path m_rootPath;
        
        void renderBody() override;
        
        const std::filesystem::path& getCurrentPath() const noexcept
        {
            return m_currentPath;
        }
        
    private:
        std::unordered_map<std::filesystem::path, bool> m_pathsNodes;
        
        ImVec2 m_windowContentRegionMax;
        ImVec2 m_windowCursorPos;
        
        std::filesystem::path m_currentPath;
        std::filesystem::path m_selectedPath;
        
        ImVec2 m_clickedRowRectMin;
        ImVec2 m_clickedRowRectMax;
        
        bool m_drawSelectedRect = false;
        
        // ===== used icons ===============
        
        SGCore::Ref<SGCore::ITexture2D> m_chevronRightIcon = Resources::getMainAssetManager().loadAsset<SGCore::ITexture2D>("chevron_right16x16");
        SGCore::Ref<SGCore::ITexture2D> m_chevronDownIcon = Resources::getMainAssetManager().loadAsset<SGCore::ITexture2D>("chevron_down16x16");
        SGCore::Ref<SGCore::ITexture2D> m_folderIcon = Resources::getMainAssetManager().loadAsset<SGCore::ITexture2D>("folder16x16");
        SGCore::Ref<SGCore::ITexture2D> m_headerIcon = Resources::getMainAssetManager().loadAsset<SGCore::ITexture2D>("header16x16");
        SGCore::Ref<SGCore::ITexture2D> m_cppIcon = Resources::getMainAssetManager().loadAsset<SGCore::ITexture2D>("cpp16x16");
        SGCore::Ref<SGCore::ITexture2D> m_cmakeIcon = Resources::getMainAssetManager().loadAsset<SGCore::ITexture2D>("cmake16x16");
        SGCore::Ref<SGCore::ITexture2D> m_txtFileIcon = Resources::getMainAssetManager().loadAsset<SGCore::ITexture2D>("lines16x16");
        SGCore::Ref<SGCore::ITexture2D> m_unknownFileIcon = Resources::getMainAssetManager().loadAsset<SGCore::ITexture2D>("question16x16");
        SGCore::Ref<SGCore::ITexture2D> m_libraryFileIcon = Resources::getMainAssetManager().loadAsset<SGCore::ITexture2D>("library16x16");
        
        // ================================
        
        void renderTreeNode(const std::filesystem::path& parent) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_DIRECTORIESTREEEXPLORER_H
