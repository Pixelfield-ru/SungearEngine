//
// Created by ilya on 24.02.24.
//
#include "FontSpecialization.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/Graphics/API/ITexture2D.h"

#include <spdlog/spdlog.h>

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
    
    std::uint8_t* buffer = nullptr;
    size_t currentBufferSize = 0;
    
    size_t currentAtlasWidth = 0;
    size_t maxGlyphHeight = 0;
    size_t maxGlyphWidth = 0;
    
    // todo: make support for wchar_t
    for(unsigned char c = 0; c < 128; ++c)
    {
        int loadGlyphErrCode = FT_Load_Char(m_face, c, FT_LOAD_RENDER);
        if(loadGlyphErrCode)
        {
            spdlog::error("Could not load glyph with index '{0}. FreeType error code: {1}", (char) c, faceCreateErrCode);
            continue;
        }
        
        size_t glyphBufferSize = m_face->glyph->bitmap.width * m_face->glyph->bitmap.rows;
        
        const auto& bitmapWidth = m_face->glyph->bitmap.width;
        const auto& bitmapHeight = m_face->glyph->bitmap.rows;
        
        auto* tmpBuf = new std::uint8_t[currentBufferSize];
        
        if(buffer)
        {
            std::memcpy(tmpBuf, buffer, currentBufferSize);
        }
        
        size_t lastBufferSize = currentBufferSize;
        currentBufferSize += glyphBufferSize;
        
        currentAtlasWidth += bitmapWidth;
        
        if(bitmapHeight > maxGlyphHeight)
        {
            maxGlyphHeight = bitmapHeight;
        }
        if(bitmapWidth > maxGlyphWidth)
        {
            maxGlyphWidth = bitmapWidth;
        }
        
        buffer = new std::uint8_t[currentBufferSize];
        std::memcpy(buffer, tmpBuf, lastBufferSize);
        std::memcpy(buffer + lastBufferSize, m_face->glyph->bitmap.buffer, bitmapWidth * bitmapHeight);
        
        delete[] tmpBuf;
        
        FontGlyph glyph {};
        glyph.m_size = { bitmapWidth, bitmapHeight };
        glyph.m_bearing = { m_face->glyph->bitmap_left, m_face->glyph->bitmap_top };
        glyph.m_advance = { m_face->glyph->advance.x, m_face->glyph->advance.y };
        // glyph.m_uv
        
        m_glyphs[c] = glyph;
    }
    
    m_atlas = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    
    m_atlas->create(buffer,
                    currentAtlasWidth,
                    1,
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
