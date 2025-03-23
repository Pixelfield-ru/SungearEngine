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

    /*msdfgen::GeneratorConfig config;

    config.

    m_atlas.atlasGenerator().setAttributes({
        .config =
    })*/
}

void SGCore::UI::FontSpecialization::saveTextAsTexture(const std::filesystem::path& path, const std::u16string& text) const noexcept
{
    /*std::vector<std::uint8_t> textBuf;
    
    size_t finalTextWidth = 0;
    
    for(size_t y = 0; y < m_maxCharacterSize.y; ++y)
    {
        for(char16_t c : text)
        {
            auto foundC = m_glyphs.find(c);
            
            if(foundC == m_glyphs.end()) continue;
            
            size_t leftOffset = foundC->second.m_sortedDataOffset;
            
            size_t finalPos = m_maxAtlasWidth * y + leftOffset;
            for(size_t x = 0; x < foundC->second.m_realSize.x; ++x)
            {
                textBuf.push_back(m_atlasTexture->getData().get()[finalPos + x]);
            }
            
            if(y == 0)
            {
                finalTextWidth += foundC->second.m_realSize.x;
            }
        }
    }
    
    stbi_write_png(Utils::toUTF8(path.u16string()).c_str(), finalTextWidth, m_maxCharacterSize.y, 1,
                   textBuf.data(), 1 * finalTextWidth);*/
}

void SGCore::UI::FontSpecialization::saveAtlasAsTexture(const std::filesystem::path& path) const noexcept
{
    /*stbi_write_png(Utils::toUTF8(path.u16string()).c_str(), m_maxAtlasWidth, m_maxCharacterSize.y, 1,
                   m_atlasTexture->getData().get(), 1 * m_maxAtlasWidth);*/

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

    // int a = m_atlas.atlasGenerator().generate()

    /*const auto& c = character;
    
    int loadGlyphErrCode = FT_Load_Char(m_face, c, FT_LOAD_RENDER);
    if(loadGlyphErrCode)
    {
        LOG_E(SGCORE_TAG, "Could not load glyph with index '{}. FreeType error code: {}", (char) c, loadGlyphErrCode);
        return false;
    }
    
    const auto& bitmapWidth = m_face->glyph->bitmap.width;
    const auto& bitmapHeight = m_face->glyph->bitmap.rows;

    // msdfgen::loa

    // buffer.reserve(buffer.capacity() + bitmapWidth * bitmapHeight);
    
    m_maxAtlasWidth += bitmapWidth;
    
    if(bitmapHeight > m_maxCharacterSize.y)
    {
        m_maxCharacterSize.y = bitmapHeight;
    }
    if(bitmapWidth > m_maxCharacterSize.x)
    {
        m_maxCharacterSize.x = bitmapWidth;
    }
    
    if(m_face->glyph->bitmap_left > m_maxCharacterBearing.x)
    {
        m_maxCharacterBearing.x = m_face->glyph->bitmap_left;

    }
    if(m_face->glyph->bitmap_top > m_maxCharacterBearing.y)
    {
        m_maxCharacterBearing.y = m_face->glyph->bitmap_top;
    }
    
    if(m_face->glyph->advance.x > m_maxCharacterAdvance.x)
    {
        m_maxCharacterAdvance.x = m_face->glyph->advance.x;
        
    }
    if(m_face->glyph->advance.y > m_maxCharacterAdvance.y)
    {
        m_maxCharacterAdvance.y = m_face->glyph->advance.y;
    }
    
    FontGlyph glyph {};
    glyph.m_realSize = { bitmapWidth, bitmapHeight };
    glyph.m_bearing = { m_face->glyph->bitmap_left, m_face->glyph->bitmap_top };
    glyph.m_advance = { m_face->glyph->advance.x, m_face->glyph->advance.y };
    glyph.m_bitmapBuffer = m_face->glyph->bitmap.buffer;
    
    m_glyphs[c] = glyph;*/
    
    return true;
}

void SGCore::UI::FontSpecialization::createAtlas() noexcept
{
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
        const double maxCornerAngle = 3.0;

        for(auto& glyph : m_glyphs)
        {
            // Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
            glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
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
        // packer.setMinimumScale(m_settings.m_height);
        // Compute atlas layout - pack glyphs
        packer.pack(m_glyphs.data(), m_glyphs.size());
        // Get final atlas dimensions
        int width = 0, height = 0;
        packer.getDimensions(width, height);

        msdf_atlas::GeneratorAttributes attributes;
        attributes.scanlinePass = true;
        attributes.config.overlapSupport = true;
        m_atlas.setAttributes(attributes);
        m_atlas.setThreadCount(1);

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

    /*std::vector<std::uint8_t> unsortedBuffer;
    std::cout << m_maxCharacterSize.y << std::endl;
    unsortedBuffer.reserve(m_maxAtlasWidth * m_maxCharacterSize.y);

    std::vector<msdf_atlas::GlyphGeometry> glyphs;
    
    for(auto& g : m_glyphs)
    {
        auto& glyph = g.second;
        
        int loadGlyphErrCode = FT_Load_Char(m_face, g.first, FT_LOAD_RENDER);
        if(loadGlyphErrCode)
        {
            LOG_E(SGCORE_TAG, "Could not load glyph with index '{}. FreeType error code: {}", (char) g.first, loadGlyphErrCode);
            continue;
        }
        
        for(size_t i = 0; i < m_maxCharacterSize.y * glyph.m_realSize.x; ++i)
        {
            if(i < glyph.m_realSize.x * glyph.m_realSize.y)
            {
                // unsortedBuffer.push_back(glyph.m_bitmapBuffer[i]);
                unsortedBuffer.push_back(m_face->glyph->bitmap.buffer[i]);
            }
            else
            {
                unsortedBuffer.push_back(0);
            }
            
            if(i == 0)
            {
                glyph.m_unsortedDataOffset = &*unsortedBuffer.rbegin() - unsortedBuffer.data();
            }
        }
        
        // invalidate
        glyph.m_bitmapBuffer = nullptr;
    }
    
    std::vector<std::uint8_t> sortedBuffer;
    sortedBuffer.reserve(m_maxAtlasWidth * m_maxCharacterSize.y);
    
    for(size_t y = 0; y < m_maxCharacterSize.y; ++y)
    {
        for(auto& g : m_glyphs)
        {
            size_t finalPos = g.second.m_unsortedDataOffset + y * g.second.m_realSize.x;
            
            for(size_t x = 0; x < g.second.m_realSize.x; ++x)
            {
                sortedBuffer.push_back(unsortedBuffer[finalPos + x]);
            }
        }
    }
    
    // =========================
    // postprocess
    size_t curXOffset = 0;
    for(auto& g : m_glyphs)
    {
        g.second.m_sortedDataOffset = curXOffset;
        
        g.second.m_uvMin = { (float) curXOffset / (float) m_maxAtlasWidth, 0 };
        g.second.m_uvMax = { ((float) curXOffset + (float) g.second.m_realSize.x) / (float) m_maxAtlasWidth, g.second.m_realSize.y / (float) m_maxCharacterSize.y };
        
        curXOffset += g.second.m_realSize.x;
    }
    
    std::cout << "buffer size: " << unsortedBuffer.size() << std::endl;
    
    m_atlasTexture = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    
    m_atlasTexture->create(sortedBuffer.data(),
                    m_maxAtlasWidth,
                    m_maxCharacterSize.y,
                    1,
                    SGGColorInternalFormat::SGG_R8,
                    SGGColorFormat::SGG_R);
    
    destroyFace();*/
}

SGCore::Ref<SGCore::UI::FontSpecializationRenderer> SGCore::UI::FontSpecialization::getRenderer() noexcept
{
    m_renderer->m_parentSpecialization = shared_from_this();
    
    return m_renderer;
}

size_t SGCore::UI::FontSpecialization::getMaxAtlasWidth() const noexcept
{
    return m_maxAtlasWidth;
}

glm::vec<2, size_t, glm::defaultp> SGCore::UI::FontSpecialization::getMaxCharacterSize() const noexcept
{
    return m_maxCharacterSize;
}

glm::vec<2, size_t, glm::defaultp> SGCore::UI::FontSpecialization::getMaxCharacterAdvance() const noexcept
{
    return m_maxCharacterAdvance;
}

glm::vec<2, size_t, glm::defaultp> SGCore::UI::FontSpecialization::getMaxCharacterBearing() const noexcept
{
    return m_maxCharacterBearing;
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
