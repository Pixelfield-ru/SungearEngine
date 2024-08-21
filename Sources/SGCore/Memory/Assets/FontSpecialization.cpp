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
#include <set>
#include <SGCore/Logger/Logger.h>

bool SGCore::FontSpecializationSettings::operator==
(const SGCore::FontSpecializationSettings& other) const noexcept
{
    return m_height == other.m_height && m_name == other.m_name;
}

bool SGCore::FontSpecializationSettings::operator!=(const SGCore::FontSpecializationSettings& other) const noexcept
{
    return !(*this == other);
}

SGCore::FontSpecialization::FontSpecialization()
{
    m_renderer = MakeRef<FontSpecializationRenderer>();
}

SGCore::FontSpecialization::~FontSpecialization()
{
    if(m_face)
    {
        FT_Done_Face(m_face);
    }
}

void SGCore::FontSpecialization::prepareToBuild(const std::string& path)
{
    destroyFace();
    
    int faceCreateErrCode = FT_New_Face(FontsManager::getFTLibrary(), path.c_str(), 0, &m_face);
    if(faceCreateErrCode)
    {
        LOG_E("Could not create face by path '{}'. FreeType error code: {}", path, faceCreateErrCode);
        return;
    }
    
    // pixel_width = 0 means auto generate pixel width
    FT_Set_Pixel_Sizes(m_face, 0, m_settings.m_height);
}

void SGCore::FontSpecialization::destroyFace()
{
    if(m_face)
    {
        int errCode = FT_Done_Face(m_face);
        if(errCode)
        {
            LOG_E("Could not delete face. FreeType error code: {}", errCode);
            return;
        }
        
        m_face = nullptr;
    }
}

void SGCore::FontSpecialization::saveTextAsTexture(const std::string& path, const std::u16string& text) const noexcept
{
    std::vector<std::uint8_t> textBuf;
    
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
                textBuf.push_back(m_atlas->getData().get()[finalPos + x]);
            }
            
            if(y == 0)
            {
                finalTextWidth += foundC->second.m_realSize.x;
            }
        }
    }
    
    stbi_write_png(path.c_str(), finalTextWidth, m_maxCharacterSize.y, 1,
                   textBuf.data(), 1 * finalTextWidth);
}

void SGCore::FontSpecialization::saveAtlasAsTexture(const std::string& path) const noexcept
{
    stbi_write_png(path.c_str(), m_maxAtlasWidth, m_maxCharacterSize.y, 1,
                   m_atlas->getData().get(), 1 * m_maxAtlasWidth);
}

const SGCore::FontGlyph* SGCore::FontSpecialization::tryGetGlyph(const char16_t& c) const noexcept
{
    auto foundGlyph = m_glyphs.find(c);
    
    return foundGlyph == m_glyphs.end() ? nullptr : &m_glyphs.at(c);
}

void SGCore::FontSpecialization::parse(const char16_t& from, const char16_t& to) noexcept
{
    for(char16_t c = from; c <= to; ++c)
    {
        parse(c);
    }
}

void SGCore::FontSpecialization::parse(const std::vector<uint16_t>& characters) noexcept
{
    for(const auto& c : characters)
    {
        parse(c);
    }
}

bool SGCore::FontSpecialization::parse(const uint16_t& character) noexcept
{
    const auto& c = character;
    
    int loadGlyphErrCode = FT_Load_Char(m_face, c, FT_LOAD_RENDER);
    if(loadGlyphErrCode)
    {
        LOG_E("Could not load glyph with index '{}. FreeType error code: {}", (char) c, loadGlyphErrCode);
        return false;
    }
    
    const auto& bitmapWidth = m_face->glyph->bitmap.width;
    const auto& bitmapHeight = m_face->glyph->bitmap.rows;
    
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
    
    m_glyphs[c] = glyph;
    
    return true;
}

void SGCore::FontSpecialization::createAtlas() noexcept
{
    std::vector<std::uint8_t> unsortedBuffer;
    std::cout << m_maxCharacterSize.y << std::endl;
    unsortedBuffer.reserve(m_maxAtlasWidth * m_maxCharacterSize.y);
    
    for(auto& g : m_glyphs)
    {
        auto& glyph = g.second;
        
        int loadGlyphErrCode = FT_Load_Char(m_face, g.first, FT_LOAD_RENDER);
        if(loadGlyphErrCode)
        {
            LOG_E("Could not load glyph with index '{}. FreeType error code: {}", (char) g.first, loadGlyphErrCode);
            continue;
        }
        
        for(size_t i = 0; i < m_maxCharacterSize.y * glyph.m_realSize.x; ++i)
        {
            if(i < glyph.m_realSize.x * glyph.m_realSize.y)
            {
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
        
        /*if(g.first == u'g')
        {
            std::cout << "glyph size: " << std::to_string(g.second.m_realSize.x) << ", " << std::to_string(g.second.m_realSize.y) << std::endl;
            std::cout << "atlas size: " << std::to_string(m_maxAtlasWidth) << ", " << std::to_string(m_maxCharacterHeight) << std::endl;
            std::cout << "min: " << std::to_string(g.second.m_uvMin.x) << ", " << std::to_string(g.second.m_uvMin.y) << std::endl;
            std::cout << "max: " << std::to_string(g.second.m_uvMax.x) << ", " << std::to_string(g.second.m_uvMax.y) << std::endl;
        }*/
        
        curXOffset += g.second.m_realSize.x;
    }
    
    std::cout << "buffer size: " << unsortedBuffer.size() << std::endl;
    
    m_atlas = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    
    m_atlas->create(sortedBuffer.data(),
                    m_maxAtlasWidth,
                    m_maxCharacterSize.y,
                    1,
                    SGGColorInternalFormat::SGG_R8,
                    SGGColorFormat::SGG_R);
    
    destroyFace();
}

SGCore::Ref<SGCore::FontSpecializationRenderer> SGCore::FontSpecialization::getRenderer() noexcept
{
    m_renderer->m_parentSpecialization = shared_from_this();
    
    return m_renderer;
}

size_t SGCore::FontSpecialization::getMaxAtlasWidth() const noexcept
{
    return m_maxAtlasWidth;
}

glm::vec<2, size_t, glm::defaultp> SGCore::FontSpecialization::getMaxCharacterSize() const noexcept
{
    return m_maxCharacterSize;
}

glm::vec<2, size_t, glm::defaultp> SGCore::FontSpecialization::getMaxCharacterAdvance() const noexcept
{
    return m_maxCharacterAdvance;
}

glm::vec<2, size_t, glm::defaultp> SGCore::FontSpecialization::getMaxCharacterBearing() const noexcept
{
    return m_maxCharacterBearing;
}
