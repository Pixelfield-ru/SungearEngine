//
// Created by ilya on 21.05.24.
//
#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>
#include "SVGImageSpecialization.h"

void SGCore::SVGImageSpecialization::regenerate(std::uint8_t channelsCount,
                                                SGGColorInternalFormat internalColorFormat,
                                                SGGColorFormat colorFormat) noexcept
{
    if(!m_bitmap.valid())
    {
        LOG_E(SGCORE_TAG,
              "Cannot create specialization (with width: '{}', height: '{}'): bitmap was not loaded successfully (not valid).",
              m_bitmap.width(),
              m_bitmap.height());
        
        return;
    }
    
    m_size = { m_bitmap.width(), m_bitmap.height() };
    
    m_texture = SGCore::Ref<SGCore::ITexture2D>(SGCore::CoreMain::getRenderer()->createTexture2D());
    m_texture->create(m_bitmap.data(), m_size.x, m_size.y, channelsCount, internalColorFormat, colorFormat);
}

lunasvg::Bitmap& SGCore::SVGImageSpecialization::getBitmap() noexcept
{
    return m_bitmap;
}

SGCore::Ref<SGCore::ITexture2D> SGCore::SVGImageSpecialization::getTexture() noexcept
{
    return m_texture;
}

SGCore::uivec2_32 SGCore::SVGImageSpecialization::getSize() const noexcept
{
    return m_size;
}
