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

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class ITexture2D;
    
    struct FontGlyph
    {
        glm::ivec2 m_realSize;
        glm::ivec2 m_bearing; // offset from baseline to left/top of glyph
        glm::ivec2 m_advance;
        
        glm::vec2 m_uvMin;
        glm::vec2 m_uvMax;
        
        size_t m_unsortedDataOffset = 0;
        size_t m_sortedDataOffset = 0;
    };
    
    struct FontSpecializationSettings
    {
        size_t m_height = 12;
        
        bool operator==(const FontSpecializationSettings& other) const noexcept;
        bool operator!=(const FontSpecializationSettings& other) const noexcept;
    };
    
    struct FontSpecialization
    {
        friend struct Font;
        
        ~FontSpecialization();
        
        void build(const std::string& path);
        void destroyFace();
        
        Ref<ITexture2D> m_atlas;
        
        void saveTextAsTexture(const std::string& path, const std::u16string& text) const noexcept;
        
        const FontGlyph* tryGetGlyph(const char16_t& c) const noexcept;
    
    private:
        // todo: make modes like PARSE_RUS, PARSE_ENG etc
        void parseFromTo(const char16_t& from, const char16_t& to, std::vector<std::uint8_t>& unsortedBuf) noexcept;
        
        FontSpecializationSettings m_settings;
        
        FT_Face m_face = nullptr;
        
        size_t m_maxAtlasWidth = 0;
        size_t m_maxAtlasHeight = 0;
        
        std::unordered_map<char16_t, FontGlyph> m_glyphs;
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
