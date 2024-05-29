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

namespace SGE
{
    struct DrawableFileNameInfo
    {
        ImVec2 m_position = ImVec2(0, 0);
        bool m_overdraw = false;
    };
    
    struct DirectoryExplorer : SGCore::ImGuiWrap::IView
    {
        SGCore::Event<void(SGCore::Ref<SGCore::ITexture2D>& iconTexture,
                           const std::string& fileExtension,
                           const std::string& fileName)> onIconRender;
        
        void renderBody() override;
        
        ImVec2 m_iconsPadding = ImVec2(40, 70);
        ImVec2 m_iconsSize = ImVec2(65, 65);
        
        std::uint8_t m_nameMaxLinesCount = 3;
        
        void setCurrentPath(const std::filesystem::path& path) noexcept;
        std::filesystem::path getCurrentPath() const noexcept;
        
    private:
        std::filesystem::path m_lastPath;
        std::filesystem::path m_currentPath;
        std::filesystem::path m_maxPath;
        
        std::unordered_map<std::filesystem::path, DrawableFileNameInfo> m_drawableFilesNames;
        
        ImVec2 m_currentItemsSize = ImVec2(0, 0);
    };
}

#endif //SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H
