//
// Created by ilya on 24.02.24.
//

#ifndef SUNGEARENGINE_FONTSPECIALIZATION_H
#define SUNGEARENGINE_FONTSPECIALIZATION_H

#include <SGCore/pch.h>

#include <msdf-atlas-gen/BitmapAtlasStorage.h>
#include <msdf-atlas-gen/DynamicAtlas.h>
#include <msdf-atlas-gen/glyph-generators.h>
#include <msdf-atlas-gen/ImmediateAtlasGenerator.h>
#include "SGCore/Main/CoreGlobals.h"

#include "SGCore/Utils/SDFTexture.h"

namespace SGCore
{
    class ITexture2D;
}

namespace SGCore::UI
{
    struct FontGlyph
    {
        friend struct FontSpecialization;
        
        glm::ivec2 m_realSize;
        glm::ivec2 m_bearing; // offset from baseline to left/top of glyph
        glm::ivec2 m_advance;
        
        glm::vec2 m_uvMin;
        glm::vec2 m_uvMax;
    };
    
    struct FontSpecializationSettings
    {
        size_t m_height = 16;
        std::string m_name = "default";
        
        bool operator==(const FontSpecializationSettings& other) const noexcept;
        bool operator!=(const FontSpecializationSettings& other) const noexcept;
    };
    
    struct FontSpecializationRenderer;
    struct Font;
    
    struct FontSpecialization : public std::enable_shared_from_this<FontSpecialization>
    {
        friend struct Font;

        using DynamicAtlas = msdf_atlas::DynamicAtlas<msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<byte, 3>>>;
        
        FontSpecialization();

        void destroyFace();
        
        Ref<ITexture2D> m_atlasTexture;
        
        void saveTextAsTexture(const std::filesystem::path& path, const std::u16string& text) const noexcept;
        void saveAtlasAsTexture(const std::filesystem::path& path) const noexcept;

        void parse(const char16_t& from, const char16_t& to) noexcept;
        void parse(const std::vector<uint16_t>& characters) noexcept;
        bool parse(const uint16_t& character) noexcept;
        
        void createAtlas() noexcept;
        
        const msdf_atlas::GlyphGeometry* tryGetGlyph(const char16_t& c) const noexcept;
        
        Ref<FontSpecializationRenderer> getRenderer() noexcept;
        
        [[nodiscard]] size_t getMaxAtlasWidth() const noexcept;
        [[nodiscard]] glm::vec<2, size_t, glm::defaultp> getMaxCharacterSize() const noexcept;
        [[nodiscard]] glm::vec<2, size_t, glm::defaultp> getMaxCharacterAdvance() const noexcept;
        [[nodiscard]] glm::vec<2, size_t, glm::defaultp> getMaxCharacterBearing() const noexcept;
        
    private:
        Ref<FontSpecializationRenderer> m_renderer;
        Font* m_usedFont = nullptr;
        
        FontSpecializationSettings m_settings;

        FT_Face m_face = nullptr;

        DynamicAtlas m_atlas;
        std::vector<msdf_atlas::GlyphGeometry> m_glyphs;

        size_t m_maxAtlasWidth = 0;
        glm::vec<2, size_t, glm::defaultp> m_maxCharacterSize { 0, 0 };
        glm::vec<2, size_t, glm::defaultp> m_maxCharacterAdvance { 0, 0 };
        glm::vec<2, size_t, glm::defaultp> m_maxCharacterBearing { 0, 0 };

        bool m_isAtlasChanged = false;

        std::unordered_map<char16_t, size_t> m_glyphsIndices;
    };
}

template<>
struct std::hash<SGCore::UI::FontSpecializationSettings>
{
    std::size_t operator()(const SGCore::UI::FontSpecializationSettings& k) const noexcept
    {
        return std::hash<size_t>()(k.m_height);
    }
};

#endif //SUNGEARENGINE_FONTSPECIALIZATION_H
