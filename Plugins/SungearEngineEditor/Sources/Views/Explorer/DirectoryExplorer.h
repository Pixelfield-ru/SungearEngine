//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H
#define SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/Graphics/API/ITexture2D.h>

#include "SungearEngineEditor.h"
#include "Resources.h"

namespace SGE
{
    struct DirectoryExplorer : SGCore::ImGuiWrap::IView
    {
        ImVec2 m_iconsMaxSize = ImVec2(50, 50);
        
        void renderBody() override;
        
        std::filesystem::path m_currentPath;
        
    private:
        SGCore::Ref<SGCore::ITexture2D> m_unknownFileIcon = Resources::getMainAssetManager().loadAsset<SGCore::ITexture2D>("question50x50");
    };
}

#endif //SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H
