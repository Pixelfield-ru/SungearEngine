//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_DIRECTORIESTREEEXPLORER_H
#define SUNGEARENGINEEDITOR_DIRECTORIESTREEEXPLORER_H

#include "SungearEngineEditor.h"

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Main/CoreGlobals.h>

namespace SGE
{
    struct DirectoriesTreeExplorer : SGCore::ImGuiWrap::IView
    {
        std::filesystem::path m_rootPath;
        
        void renderBody() override;
        
    private:
        std::unordered_map<std::filesystem::path, bool> m_pathsNodes;
        
        ImVec2 m_windowSize;
        ImVec2 m_windowCursorPos;
        
        std::filesystem::path m_currentPath;
        
        // ===== used icons ===============
        
        SGCore::Ref<SGCore::ITexture2D> m_chevronRightIcon = SungearEngineEditor::getAssetManager().loadAsset<SGCore::ITexture2D>("chevron_right16x16");
        SGCore::Ref<SGCore::ITexture2D> m_chevronDownIcon = SungearEngineEditor::getAssetManager().loadAsset<SGCore::ITexture2D>("chevron_down16x16");
        SGCore::Ref<SGCore::ITexture2D> m_folderIcon = SungearEngineEditor::getAssetManager().loadAsset<SGCore::ITexture2D>("folder16x16");
        SGCore::Ref<SGCore::ITexture2D> m_headerIcon = SungearEngineEditor::getAssetManager().loadAsset<SGCore::ITexture2D>("header16x16");
        SGCore::Ref<SGCore::ITexture2D> m_cppIcon = SungearEngineEditor::getAssetManager().loadAsset<SGCore::ITexture2D>("cpp16x16");
        
        // ================================
        
        void renderTreeNode(const std::filesystem::path& parent) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_DIRECTORIESTREEEXPLORER_H
