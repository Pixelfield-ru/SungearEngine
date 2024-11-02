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
#include <SGCore/Memory/Assets/SVGImage.h>

namespace SGE
{
    struct DirectoriesTreeExplorer : SGCore::ImGuiWrap::IView
    {
        SGCore::Event<void(SGCore::AssetRef<SGCore::ITexture2D>& iconTexture,
                           const std::string& fileExtension,
                           const std::string& fileName)> onIconRender;
        
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
        
        void renderTreeNode(const std::filesystem::path& parent) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_DIRECTORIESTREEEXPLORER_H
