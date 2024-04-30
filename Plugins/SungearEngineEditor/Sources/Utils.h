//
// Created by ilya on 29.04.24.
//

#ifndef SUNGEARENGINEEDITOR_UTILS_H
#define SUNGEARENGINEEDITOR_UTILS_H

#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <lunasvg.h>
#include <stb_image_write.h>
#include <SGCore/Memory/AssetManager.h>

namespace SGE
{
    struct Utils
    {
        // todo: SGCORE CANDIDATE
        static SGCore::Ref<SGCore::ITexture2D> loadSVG(const std::string& path,
                                                       const std::string& alias,
                                                       const std::uint32_t& width,
                                                       const std::uint32_t& height) noexcept
        {
            if(!std::filesystem::exists(path))
            {
                return nullptr;
            }
            
            if(SungearEngineEditor::getAssetManager().isAssetExists<SGCore::ITexture2D>(alias))
            {
                return SungearEngineEditor::getAssetManager().loadAsset<SGCore::ITexture2D>(alias, path);
            }
            
            auto document = lunasvg::Document::loadFromFile(path);
            auto bitmap = document->renderToBitmap(width, height);
            bitmap.convertToRGBA();
            
            auto svgTexture = SGCore::Ref<SGCore::ITexture2D>(SGCore::CoreMain::getRenderer()->createTexture2D());
            svgTexture->create(bitmap.data(), bitmap.width(), bitmap.height(), 4, SGGColorInternalFormat::SGG_RGBA8, SGGColorFormat::SGG_RGBA);
            
            SungearEngineEditor::getAssetManager().addAsset(alias, svgTexture);
            
            std::cout << "LOADED SVG: " << path << " WITH ALIAS: " << alias << std::endl;
            
            return svgTexture;
        }
        
        static SGCore::Ref<SGCore::ITexture2D> loadSVG(const std::string& path,
                                                       const std::string& alias,
                                                       const glm::ivec2& size) noexcept
        {
            return loadSVG(path, alias, size.x, size.y);
        }
    };
}

#endif //SUNGEARENGINEEDITOR_UTILS_H
