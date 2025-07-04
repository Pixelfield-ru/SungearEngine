//
// Created by stuka on 20.06.2025.
//

#ifndef SUNGEARENGINE_ATLAS_H
#define SUNGEARENGINE_ATLAS_H

#include "AtlasRect.h"
#include "SGCore/Graphics/API/GPUDeviceInfo.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Main/CoreGlobals.h"

#include <rectpack2D/rect_structs.h>
#include <rectpack2D/empty_spaces.h>
#include <rectpack2D/finders_interface.h>

namespace SGCore
{
    class ITexture2D;

    struct Atlas
    {
        std::uint32_t m_maxSideSize = GPUDeviceInfo::getMaxTextureSize().x;

        void findBestRect(glm::ivec2 textureSize, AtlasRect& outputRect) noexcept;
        void packTexture(const AtlasRect& inRect, const ITexture2D* texture) noexcept;

        [[nodiscard]] Ref<ITexture2D> getTexture() const noexcept;

    private:
        using spaces_type = rectpack2D::empty_spaces<false, rectpack2D::default_empty_spaces>;
        using rect_type = rectpack2D::output_rect_t<spaces_type>;

        std::vector<rectpack2D::rect_xywh> m_atlasRects;

        std::int32_t m_currentSideSize = 1000;

        Ref<ITexture2D> m_atlasTexture;

        bool tryPack(glm::ivec2 textureSize, AtlasRect& outputRect) noexcept;

        static void convertTextureFormatToRGBA32INT(const std::uint8_t* srcBuffer, std::uint8_t* dstBuffer, size_t pixelsCount, const std::vector<std::uint8_t>& srcChannelsBits, SGGDataType srcBufferDataType) noexcept;

        void resizeAtlasForTexture(glm::ivec2 textureSize) noexcept;
    };
}

#endif // SUNGEARENGINE_ATLAS_H
