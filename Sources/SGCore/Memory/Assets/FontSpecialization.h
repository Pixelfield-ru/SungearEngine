//
// Created by ilya on 24.02.24.
//

#ifndef SUNGEARENGINE_FONTSPECIALIZATION_H
#define SUNGEARENGINE_FONTSPECIALIZATION_H

#include <SGCore/pch.h>

#include <msdf-atlas-gen/msdf-atlas-gen/BitmapAtlasStorage.h>
#include <msdf-atlas-gen/msdf-atlas-gen/DynamicAtlas.h>
#include <msdf-atlas-gen/msdf-atlas-gen/glyph-generators.h>
#include <msdf-atlas-gen/msdf-atlas-gen/ImmediateAtlasGenerator.h>

#include "msdf-atlas-gen/msdf-atlas-gen/Charset.h"
#include "msdf-atlas-gen/msdf-atlas-gen/FontGeometry.h"
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

        using AtlasT = msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<byte, 3>>;
        
        FontSpecialization();
        
        Ref<ITexture2D> m_atlasTexture;

        void saveAtlasAsTexture(const std::filesystem::path& path) const noexcept;

        void parse(const uint32_t& from, const uint32_t& to) noexcept;
        void parse(const std::vector<uint32_t>& characters) noexcept;
        bool parse(const uint32_t& character) noexcept;
        
        void createAtlas() noexcept;
        
        const msdf_atlas::GlyphGeometry* tryGetGlyph(const uint32_t& c) const noexcept;
        
        Ref<FontSpecializationRenderer> getRenderer() noexcept;

        const msdfgen::FontMetrics& getMetrics() const noexcept;
        const msdf_atlas::FontGeometry& getGeometry() const noexcept;
        const FontSpecializationSettings& getSettings() const noexcept;

        glm::ivec2 getSize() const noexcept;
        
    private:
        Ref<FontSpecializationRenderer> m_renderer;
        Font* m_usedFont = nullptr;
        
        FontSpecializationSettings m_settings;

        AtlasT m_atlas;
        std::vector<msdf_atlas::GlyphGeometry> m_glyphs;
        std::unordered_map<uint32_t, size_t> m_glyphsIndices;
        msdf_atlas::Charset m_charset;

        msdf_atlas::FontGeometry m_geometry;

        glm::ivec2 m_atlasSize;

        size_t m_maxAtlasWidth = 0;
        glm::vec<2, size_t, glm::defaultp> m_maxCharacterSize { 0, 0 };
        glm::vec<2, size_t, glm::defaultp> m_maxCharacterAdvance { 0, 0 };
        glm::vec<2, size_t, glm::defaultp> m_maxCharacterBearing { 0, 0 };

        bool m_isCharsetChanged = false;
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
