//
// Created by ilya on 24.02.24.
//
#include "FontSpecialization.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "stb_image_write.h"
#include "SGCore/UI/FontSpecializationRenderer.h"

#include <spdlog/spdlog.h>
#include <set>

bool SGCore::FontSpecializationSettings::operator==
(const SGCore::FontSpecializationSettings& other) const noexcept
{
    return m_height == other.m_height;
}

bool SGCore::FontSpecializationSettings::operator!=(const SGCore::FontSpecializationSettings& other) const noexcept
{
    return !(*this == other);
}

SGCore::FontSpecialization::~FontSpecialization()
{
    if(m_face)
    {
        FT_Done_Face(m_face);
    }
}

void SGCore::FontSpecialization::build(const std::string& path)
{
    destroyFace();
    
    int faceCreateErrCode = FT_New_Face(FontsManager::getFTLibrary(), path.c_str(), 0, &m_face);
    if(faceCreateErrCode)
    {
        spdlog::error("Could not create face by path '{0}'. FreeType error code: {1}", path, faceCreateErrCode);
        return;
    }
    
    // pixel_width = 0 means auto generate pixel width
    FT_Set_Pixel_Sizes(m_face, 0, m_settings.m_height);
    
    // size_t maxCharsToParse = std::numeric_limits<char16_t>::max();
    size_t maxCharsToParse = 1264;
    
    parseFromTo(0, 128);
    
    std::vector<std::uint8_t> buffer;
    buffer.reserve(m_maxAtlasWidth * m_maxAtlasHeight);
    
    // std::unordered_map<unsigned char, std::uint8_t*> charsMarkup;
    
    // second iteration to set up first buffer
    for(char16_t c = 0; c < maxCharsToParse; ++c)
    {
        int loadGlyphErrCode = FT_Load_Char(m_face, c, FT_LOAD_RENDER);
        if(loadGlyphErrCode)
        {
            spdlog::error("Could not load glyph with index '{0}. FreeType error code: {1}", (char) c, faceCreateErrCode);
            continue;
        }
        
        const auto& bitmapWidth = m_face->glyph->bitmap.width;
        const auto& bitmapHeight = m_face->glyph->bitmap.rows;
        
        for(size_t i = 0; i < m_maxAtlasHeight * bitmapWidth; ++i)
        {
            if(i < bitmapWidth * bitmapHeight)
            {
                buffer.push_back(m_face->glyph->bitmap.buffer[i]);
            }
            else
            {
                buffer.push_back(0);
            }
            
            if(i == 0)
            {
                m_glyphs[c].m_unsortedDataOffset = &*buffer.rbegin() - buffer.data();
            }
        }
    }
    
    std::vector<std::uint8_t> sortedBuffer;
    sortedBuffer.reserve(m_maxAtlasWidth * m_maxAtlasHeight);
    
    for(size_t y = 0; y < m_maxAtlasHeight; ++y)
    {
        for(auto& g : m_glyphs)
        {
            size_t finalPos = g.second.m_unsortedDataOffset + y * g.second.m_realSize.x;
            
            for(size_t x = 0; x < g.second.m_realSize.x; ++x)
            {
                sortedBuffer.push_back(buffer[finalPos + x]);
            }
        }
    }
    
    // =========================
    // postprocess
    size_t curXOffset = 0;
    for(auto& g : m_glyphs)
    {
        g.second.m_sortedDataOffset = curXOffset;
        
        g.second.m_uvMin = { curXOffset / m_maxAtlasWidth, 0 };
        g.second.m_uvMax = { curXOffset / m_maxAtlasWidth, g.second.m_realSize.y / m_maxAtlasHeight };
        
        curXOffset += g.second.m_realSize.x;
    }
    
    std::cout << "buffer size: " << buffer.size() << std::endl;
    
    stbi_write_png("font_spec_test.png", m_maxAtlasWidth, m_maxAtlasHeight, 1,
                   sortedBuffer.data(), 1 * m_maxAtlasWidth);
    
    m_atlas = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    
    m_atlas->create(sortedBuffer.data(),
                    m_maxAtlasWidth,
                    m_maxAtlasHeight,
                    1,
                    SGGColorInternalFormat::SGG_R8,
                    SGGColorFormat::SGG_R);
    
    destroyFace();
}

void SGCore::FontSpecialization::destroyFace()
{
    if(m_face)
    {
        int errCode = FT_Done_Face(m_face);
        if(errCode)
        {
            spdlog::error("Could not delete face. FreeType error code: {0}", errCode);
            return;
        }
        
        m_face = nullptr;
    }
}

void SGCore::FontSpecialization::saveTextAsTexture(const std::string& path, const std::u16string& text) const noexcept
{
    std::vector<std::uint8_t> textBuf;
    
    size_t finalTextWidth = 0;
    
    for(size_t y = 0; y < m_maxAtlasHeight; ++y)
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
    
    stbi_write_png(path.c_str(), finalTextWidth, m_maxAtlasHeight, 1,
                   textBuf.data(), 1 * finalTextWidth);
}

const SGCore::FontGlyph* SGCore::FontSpecialization::tryGetGlyph(const char16_t& c) const noexcept
{
    auto foundGlyph = m_glyphs.find(c);
    auto qGlyph = m_glyphs.find(u'?');
    
    return foundGlyph == m_glyphs.end() ? (qGlyph == m_glyphs.end() ? nullptr : &qGlyph->second) : &foundGlyph->second;
}

void SGCore::FontSpecialization::parseFromTo(const char16_t& from, const char16_t& to,
                                             std::vector<std::uint8_t>& unsortedBuf) noexcept
{
    for(char16_t c = from; c < to; ++c)
    {
        int loadGlyphErrCode = FT_Load_Char(m_face, c, FT_LOAD_RENDER);
        if(loadGlyphErrCode)
        {
            spdlog::error("Could not load glyph with index '{0}. FreeType error code: {1}", (char) c, loadGlyphErrCode);
            continue;
        }
        
        if(c == u'А')
        {
            std::cout << "found rus" << std::endl;
        }
        
        const auto& bitmapWidth = m_face->glyph->bitmap.width;
        const auto& bitmapHeight = m_face->glyph->bitmap.rows;
        
        // buffer.reserve(buffer.capacity() + bitmapWidth * bitmapHeight);
        
        m_maxAtlasWidth += bitmapWidth;
        
        if(bitmapHeight > m_maxAtlasHeight)
        {
            m_maxAtlasHeight = bitmapHeight;
        }
        
        FontGlyph glyph {};
        glyph.m_realSize = { bitmapWidth, bitmapHeight };
        glyph.m_bearing = { m_face->glyph->bitmap_left, m_face->glyph->bitmap_top };
        glyph.m_advance = { m_face->glyph->advance.x, m_face->glyph->advance.y };
        // glyph.m_uv
        
        m_glyphs[c] = glyph;
    }
    
    for(char16_t c = from; c < to; ++c)
    {
        int loadGlyphErrCode = FT_Load_Char(m_face, c, FT_LOAD_RENDER);
        if(loadGlyphErrCode)
        {
            spdlog::error("Could not load glyph with index '{0}. FreeType error code: {1}", (char) c, loadGlyphErrCode);
            continue;
        }
        
        if(c == u'А')
        {
            std::cout << "found rus" << std::endl;
        }
        
        const auto& bitmapWidth = m_face->glyph->bitmap.width;
        const auto& bitmapHeight = m_face->glyph->bitmap.rows;
        
        for(size_t i = 0; i < m_maxAtlasHeight * bitmapWidth; ++i)
        {
            if(i < bitmapWidth * bitmapHeight)
            {
                unsortedBuf.push_back(m_face->glyph->bitmap.buffer[i]);
            }
            else
            {
                unsortedBuf.push_back(0);
            }
            
            if(i == 0)
            {
                m_glyphs[c].m_unsortedDataOffset = &*unsortedBuf.rbegin() - unsortedBuf.data();
            }
        }
    }
}
