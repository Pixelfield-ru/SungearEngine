//
// Created by ilya on 24.02.24.
//
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "FontSpecialization.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "stb_image_write.h"
#include "SGCore/UI/FontSpecializationRenderer.h"

#include <spdlog/spdlog.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <set>
#include <SGCore/Logger/Logger.h>

#include "Font.h"

bool SGCore::UI::FontSpecializationSettings::operator==(const SGCore::UI::FontSpecializationSettings& other) const noexcept
{
    return m_height == other.m_height && m_name == other.m_name;
}

bool SGCore::UI::FontSpecializationSettings::operator!=(const SGCore::UI::FontSpecializationSettings& other) const noexcept
{
    return !(*this == other);
}

SGCore::UI::FontSpecialization::FontSpecialization()
{
    m_renderer = MakeRef<FontSpecializationRenderer>();
}

void SGCore::UI::FontSpecialization::saveAtlasAsTexture(const std::filesystem::path& path) const noexcept
{
    stbi_write_png(Utils::toUTF8(path.u16string()).c_str(), m_atlasTexture->getWidth(), m_atlasTexture->getHeight(), m_atlasTexture->m_channelsCount,
                   m_atlasTexture->getData().get(), m_atlasTexture->m_channelsCount * m_maxAtlasWidth);
}

const msdf_atlas::GlyphGeometry* SGCore::UI::FontSpecialization::tryGetGlyph(const uint32_t& c) const noexcept
{
    auto it = m_glyphsIndices.find(c);
    
    return it == m_glyphsIndices.end() ? nullptr : &m_glyphs.at(it->second);
}

void SGCore::UI::FontSpecialization::parse(const uint32_t& from, const uint32_t& to) noexcept
{
    if(!m_usedFont)
    {
        LOG_E(SGCORE_TAG,
              "Can not load bunch of characters in font specialization with settings: name: '{}', height: {}. Parent font is nullptr!",
              m_settings.m_name,
              m_settings.m_height);

        return;
    }

    for(uint32_t c = from; c <= to; ++c)
    {
        if(m_glyphsIndices.contains(c)) continue;

        m_charset.add(c);

        m_isCharsetChanged = true;
    }
}

void SGCore::UI::FontSpecialization::parse(const std::vector<uint32_t>& characters) noexcept
{
    if(!m_usedFont)
    {
        LOG_E(SGCORE_TAG,
              "Can not load bunch of characters in font specialization with settings: name: '{}', height: {}. Parent font is nullptr!",
              m_settings.m_name,
              m_settings.m_height);

        return;
    }

    for(const auto& character : characters)
    {
        if(m_glyphsIndices.contains(character)) continue;

        m_charset.add(character);

        m_isCharsetChanged = true;
    }
}

bool SGCore::UI::FontSpecialization::parse(const uint32_t& character) noexcept
{
    if(!m_usedFont)
    {
        LOG_E(SGCORE_TAG,
              "Can not load character '{}' in font specialization with settings: name: '{}', height: {}. Parent font is nullptr!",
              character,
              m_settings.m_name,
              m_settings.m_height);

        return false;
    }

    if(m_glyphsIndices.contains(character)) return false;

    m_charset.add(character);

    m_isCharsetChanged = true;
    
    return true;
}

void SGCore::UI::FontSpecialization::createAtlas() noexcept
{
    if(!m_glyphsIndices.contains('?'))
    {
        m_charset.add('?');

        m_isCharsetChanged = true;
    }

    if(m_isCharsetChanged)
    {
        m_glyphs.clear();
        m_glyphsIndices.clear();

        m_geometry = msdf_atlas::FontGeometry(&m_glyphs);

        if(!m_geometry.loadCharset(m_usedFont->getFontHandler(), 1, m_charset))
        {
            LOG_E(SGCORE_TAG,
                  "Can not load bunch of characters in font specialization with settings: name: '{}', height: {}. Character was not found in font!",
                  m_settings.m_name,
                  m_settings.m_height);

            return;
        }

        const double pixelRange = 2.0;
        const double miterLimit = 1.0;

        // TODO: IDK WHAT IS THIS. NEED TO FIGURE IT OUT ===
        // Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
        const double maxCornerAngle = 5.0;

        for(size_t i = 0; i < m_glyphs.size(); ++i)
        {
            auto& glyph = m_glyphs[i];
            // Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
            glyph.edgeColoring(&msdfgen::edgeColoringByDistance, maxCornerAngle, 0);
            // Finalize glyph box size based on the parameters
            // glyph.wrapBox(m_settings.m_height, pixelRange / m_settings.m_height, miterLimit);
            // =====
        }

        // TightAtlasPacker class computes the layout of the atlas.
        msdf_atlas::TightAtlasPacker packer;
        // Set atlas parameters:
        // setDimensions or setDimensionsConstraint to find the best value
        packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
        // setScale for a fixed size or setMinimumScale to use the largest that fits
        packer.setScale(m_settings.m_height);
        // setPixelRange or setUnitRange
        packer.setPixelRange(pixelRange);
        packer.setMiterLimit(miterLimit);
        // packer.setUnitRange(0.9);
        // packer.setMinimumScale(m_settings.m_height);
        // Compute atlas layout - pack glyphs
        packer.pack(m_glyphs.data(), m_glyphs.size());
        // Get final atlas dimensions
        int width = 0, height = 0;
        packer.getDimensions(width, height);

        msdf_atlas::GeneratorAttributes attributes;
        attributes.scanlinePass = true;
        attributes.config.overlapSupport = true;
        /*attributes.config.errorCorrection.mode = msdfgen::ErrorCorrectionConfig::Mode::INDISCRIMINATE;
        attributes.config.errorCorrection.distanceCheckMode = msdfgen::ErrorCorrectionConfig::DistanceCheckMode::ALWAYS_CHECK_DISTANCE;*/

        m_atlas.setAttributes(attributes);
        m_atlas.setThreadCount(6);

        std::cout << "scale: " << packer.getScale() << std::endl;

        m_atlas.resize(width, height);
        m_atlas.generate(m_glyphs.data(), m_glyphs.size());

        // adding glyphs indices to map only after generating atlas because generate() function makes positioning of glyphs
        for(size_t i = 0; i < m_glyphs.size(); ++i)
        {
            m_glyphsIndices[m_glyphs[i].getCodepoint()] = i;
        }

        m_atlasSize = { width, height };

        // ============================================================
        // generating texture

        // TODO: saving to texture...
        const msdf_atlas::BitmapAtlasStorage<byte, 3>& atlasStorage = m_atlas.atlasStorage();
        msdfgen::Bitmap<byte, 3> bitmap(atlasStorage);
        const byte* atlasBytes(bitmap);

        m_atlasTexture = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());

        m_atlasTexture->create(atlasBytes,
                    bitmap.width(),
                    bitmap.height(),
                    3,
                    SGGColorInternalFormat::SGG_RGB8,
                    SGGColorFormat::SGG_RGB);

        m_isCharsetChanged = false;
    }
}

SGCore::Ref<SGCore::UI::FontSpecializationRenderer> SGCore::UI::FontSpecialization::getRenderer() noexcept
{
    m_renderer->m_parentSpecialization = shared_from_this();
    
    return m_renderer;
}

const msdfgen::FontMetrics& SGCore::UI::FontSpecialization::getMetrics() const noexcept
{
    return m_geometry.getMetrics();
}

const msdf_atlas::FontGeometry& SGCore::UI::FontSpecialization::getGeometry() const noexcept
{
    return m_geometry;
}

const SGCore::UI::FontSpecializationSettings& SGCore::UI::FontSpecialization::getSettings() const noexcept
{
    return m_settings;
}

glm::ivec2 SGCore::UI::FontSpecialization::getSize() const noexcept
{
    return m_atlasSize;
}
