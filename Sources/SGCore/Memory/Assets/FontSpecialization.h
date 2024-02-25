//
// Created by ilya on 24.02.24.
//

#ifndef SUNGEARENGINE_FONTSPECIALIZATION_H
#define SUNGEARENGINE_FONTSPECIALIZATION_H

#include <map>
#include <cstddef>
#include <freetype/freetype.h>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <vector>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class ITexture2D;
    
    struct FontGlyph
    {
        friend struct FontSpecialization;
        
        glm::ivec2 m_realSize;
        glm::ivec2 m_bearing; // offset from baseline to left/top of glyph
        glm::ivec2 m_advance;
        
        glm::vec2 m_uvMin;
        glm::vec2 m_uvMax;
        
        size_t m_unsortedDataOffset = 0;
        size_t m_sortedDataOffset = 0;
        
    private:
        // INVALID AFTER SORTING STAGE! BE CAREFUL
        std::uint8_t* m_bitmapBuffer = nullptr;
    };
    
    struct FontSpecializationSettings
    {
        size_t m_height = 12;
        std::string m_name = "default";
        
        bool operator==(const FontSpecializationSettings& other) const noexcept;
        bool operator!=(const FontSpecializationSettings& other) const noexcept;
    };
    
    struct FontSpecializationRenderer;
    
    struct FontSpecialization : public std::enable_shared_from_this<FontSpecialization>
    {
        friend struct Font;
        
        FontSpecialization();
        ~FontSpecialization();
        
        void prepareToBuild(const std::string& path);
        void destroyFace();
        
        Ref<ITexture2D> m_atlas;
        
        void saveTextAsTexture(const std::string& path, const std::u16string& text) const noexcept;
        void saveAtlasAsTexture(const std::string& path) const noexcept;
        
        void parse(const char16_t& from, const char16_t& to) noexcept;
        void parse(const std::vector<uint16_t>& characters) noexcept;
        bool parse(const uint16_t& character) noexcept;
        
        void createAtlas() noexcept;
        
        const FontGlyph* tryGetGlyph(const char16_t& c) const noexcept;
        
        Ref<FontSpecializationRenderer> getRenderer() noexcept;
        
        std::unordered_map<char16_t, FontGlyph> m_glyphs;
    private:
        Ref<FontSpecializationRenderer> m_renderer;
        
        FontSpecializationSettings m_settings;
        
        FT_Face m_face = nullptr;
        
        size_t m_maxAtlasWidth = 0;
        size_t m_maxAtlasHeight = 0;
    };
}

template<>
struct std::hash<SGCore::FontSpecializationSettings>
{
    std::size_t operator()(const SGCore::FontSpecializationSettings& k) const
    {
        return std::hash<size_t>()(k.m_height);
    }
};

#endif //SUNGEARENGINE_FONTSPECIALIZATION_H
