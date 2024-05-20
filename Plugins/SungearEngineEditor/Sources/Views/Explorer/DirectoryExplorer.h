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
    struct DirectoryExplorer : SGCore::ImGuiWrap::IView
    {
        SGCore::Event<void(SGCore::Ref<SGCore::ITexture2D>& iconTexture,
                           const std::string& fileExtension,
                           const std::string& fileName)> onIconRender;
        
        void renderBody() override;
        
        std::filesystem::path m_currentPath;
        
    private:
        SGCore::Ref<SGCore::ITexture2D> m_unknownFileIcon = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("question")->getSpecialization(50, 50)->getTexture();
    };
}

#endif //SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H
