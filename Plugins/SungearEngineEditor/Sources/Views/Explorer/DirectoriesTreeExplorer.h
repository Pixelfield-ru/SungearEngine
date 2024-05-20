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
        SGCore::Event<void(SGCore::Ref<SGCore::ITexture2D>& iconTexture,
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
        
        // ===== used icons ===============
        
        SGCore::Ref<SGCore::ITexture2D> m_chevronRightIcon = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("chevron_right")->getSpecialization(16, 16)->getTexture();
        SGCore::Ref<SGCore::ITexture2D> m_chevronDownIcon = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("chevron_down")->getSpecialization(16, 16)->getTexture();
        SGCore::Ref<SGCore::ITexture2D> m_folderIcon = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("folder")->getSpecialization(16, 16)->getTexture();
        SGCore::Ref<SGCore::ITexture2D> m_headerIcon = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("header")->getSpecialization(16, 16)->getTexture();
        SGCore::Ref<SGCore::ITexture2D> m_cppIcon = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("cpp")->getSpecialization(16, 16)->getTexture();
        SGCore::Ref<SGCore::ITexture2D> m_cmakeIcon = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("cmake")->getSpecialization(16, 16)->getTexture();
        SGCore::Ref<SGCore::ITexture2D> m_txtFileIcon = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("lines")->getSpecialization(16, 16)->getTexture();
        SGCore::Ref<SGCore::ITexture2D> m_unknownFileIcon = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("question")->getSpecialization(16, 16)->getTexture();
        SGCore::Ref<SGCore::ITexture2D> m_libraryFileIcon = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("library")->getSpecialization(16, 16)->getTexture();
        
        // ================================
        
        void renderTreeNode(const std::filesystem::path& parent) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_DIRECTORIESTREEEXPLORER_H
