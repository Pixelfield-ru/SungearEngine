//
// Created by ilya on 21.05.24.
//

#ifndef SUNGEARENGINE_SVGIMAGESPECIALIZATION_H
#define SUNGEARENGINE_SVGIMAGESPECIALIZATION_H

#include <lunasvg.h>

#include "SGCore/Graphics/API/ITexture2D.h"

namespace SGCore
{
    struct SVGImageSpecialization
    {
        friend struct SVGImage;
        
        void regenerate(std::uint8_t channelsCount = 4,
                        SGGColorInternalFormat internalColorFormat = SGGColorInternalFormat::SGG_RGBA8,
                        SGGColorFormat colorFormat = SGGColorFormat::SGG_RGBA) noexcept;
        
        [[nodiscard]] lunasvg::Bitmap& getBitmap() noexcept;
        
        [[nodiscard]] Ref<ITexture2D> getTexture() noexcept;
        
        [[nodiscard]] uivec2_32 getSize() const noexcept;
        
    private:
        lunasvg::Bitmap m_bitmap;
        
        Ref<ITexture2D> m_texture;
        
        uivec2_32 m_size { 0, 0 };
    };
}

#endif // SUNGEARENGINE_SVGIMAGESPECIALIZATION_H
