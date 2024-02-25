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
    return m_height == other.m_height && m_name == other.m_name;
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

void SGCore::FontSpecialization::prepareToBuild(const std::string& path)
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

void SGCore::FontSpecialization::saveAtlasAsTexture(const std::string& path) const noexcept
{
    stbi_write_png(path.c_str(), m_maxAtlasWidth, m_maxAtlasHeight, 1,
                   m_atlas->getData().get(), 1 * m_maxAtlasWidth);
}

const SGCore::FontGlyph* SGCore::FontSpecialization::tryGetGlyph(const char16_t& c) const noexcept
{
    auto foundGlyph = m_glyphs.find(c);
    auto qGlyph = m_glyphs.find(u'?');
    
    return foundGlyph == m_glyphs.end() ? (qGlyph == m_glyphs.end() ? nullptr : &qGlyph->second) : &foundGlyph->second;
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
        spdlog::error("Could not load glyph with index '{0}. FreeType error code: {1}", (char) c, loadGlyphErrCode);
        return false;
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
    glyph.m_bitmapBuffer = m_face->glyph->bitmap.buffer;
    
    m_glyphs[c] = glyph;
    
    return true;
}

void SGCore::FontSpecialization::createAtlas() noexcept
{
    std::vector<std::uint8_t> unsortedBuffer;
    unsortedBuffer.reserve(m_maxAtlasWidth * m_maxAtlasHeight);
    
    for(auto& g : m_glyphs)
    {
        auto& glyph = g.second;
        
        int loadGlyphErrCode = FT_Load_Char(m_face, g.first, FT_LOAD_RENDER);
        if(loadGlyphErrCode)
        {
            spdlog::error("Could not load glyph with index '{0}. FreeType error code: {1}", (char) g.first, loadGlyphErrCode);
            continue;
        }
        
        for(size_t i = 0; i < m_maxAtlasHeight * glyph.m_realSize.x; ++i)
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
    sortedBuffer.reserve(m_maxAtlasWidth * m_maxAtlasHeight);
    
    for(size_t y = 0; y < m_maxAtlasHeight; ++y)
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
        g.second.m_uvMax = { ((float) curXOffset + (float) g.second.m_realSize.x) / (float) m_maxAtlasWidth, g.second.m_realSize.y / (float) m_maxAtlasHeight };
        
        /*if(g.first == u'g')
        {
            std::cout << "glyph size: " << std::to_string(g.second.m_realSize.x) << ", " << std::to_string(g.second.m_realSize.y) << std::endl;
            std::cout << "atlas size: " << std::to_string(m_maxAtlasWidth) << ", " << std::to_string(m_maxAtlasHeight) << std::endl;
            std::cout << "min: " << std::to_string(g.second.m_uvMin.x) << ", " << std::to_string(g.second.m_uvMin.y) << std::endl;
            std::cout << "max: " << std::to_string(g.second.m_uvMax.x) << ", " << std::to_string(g.second.m_uvMax.y) << std::endl;
        }*/
        
        curXOffset += g.second.m_realSize.x;
    }
    
    std::cout << "buffer size: " << unsortedBuffer.size() << std::endl;
    
    /*stbi_write_png("font_spec_test.png", m_maxAtlasWidth, m_maxAtlasHeight, 1,
                   sortedBuffer.data(), 1 * m_maxAtlasWidth);*/
    
    m_atlas = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    
    m_atlas->create(sortedBuffer.data(),
                    m_maxAtlasWidth,
                    m_maxAtlasHeight,
                    1,
                    SGGColorInternalFormat::SGG_R8,
                    SGGColorFormat::SGG_R);
    
    destroyFace();
}
