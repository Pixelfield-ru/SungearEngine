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
    struct FontSpecializationSettings
    {
        size_t m_height = 16;
        std::string m_name = "default";
        
        bool operator==(const FontSpecializationSettings& other) const noexcept;
        bool operator!=(const FontSpecializationSettings& other) const noexcept;
    };

    struct FontGlyph
    {
        friend struct FontSpecialization;

        msdf_atlas::GlyphGeometry m_geometry;

        glm::vec2 getUVMin() const noexcept
        {
            return m_uvMin;
        }

        glm::vec2 getUVMax() const noexcept
        {
            return m_uvMax;
        }

        glm::vec2 getQuadMin() const noexcept
        {
            return m_quadMin;
        }

        glm::vec2 getQuadMax() const noexcept
        {
            return m_quadMax;
        }

    private:
        std::uint32_t m_character = '?';

        glm::vec2 m_uvMin { 0 };
        glm::vec2 m_uvMax { 0 };
        glm::vec2 m_quadMin { 0 };
        glm::vec2 m_quadMax { 0 };
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
        
        const FontGlyph* tryGetGlyph(const uint32_t& c) const noexcept;
        
        Ref<FontSpecializationRenderer> getRenderer() noexcept;

        const msdfgen::FontMetrics& getMetrics() const noexcept;
        const msdf_atlas::FontGeometry& getGeometry() const noexcept;
        const FontSpecializationSettings& getSettings() const noexcept;

        float getGlyphsHeightScale() const noexcept;
        float getDescenderYPos() const noexcept;
        float getAscenderYPos() const noexcept;
        float getLineHeight() const noexcept;

        glm::ivec2 getSize() const noexcept;
        
    private:
        Ref<FontSpecializationRenderer> m_renderer;
        Font* m_usedFont = nullptr;
        
        FontSpecializationSettings m_settings;

        AtlasT m_atlas;
        std::vector<FontGlyph> m_glyphs;
        // omg i need to store this field because constructor of msdf_atlas::FontGeometry stores pointer to vector of msdf_atlas::GlyphGeometry...
        std::vector<msdf_atlas::GlyphGeometry> m_glyphsGeometry;
        std::unordered_map<uint32_t, size_t> m_glyphsIndices;
        msdf_atlas::Charset m_charset;

        msdf_atlas::FontGeometry m_geometry;

        glm::ivec2 m_atlasSize;

        float m_glyphsHeightScale = 0.0f;
        float m_descenderYPos = 0.0f;
        float m_ascenderYPos = 0.0f;
        float m_lineHeight = 0.0f;

        size_t m_maxAtlasWidth = 0;

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
