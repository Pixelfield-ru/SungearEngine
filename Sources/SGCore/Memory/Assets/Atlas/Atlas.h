//
// Created by stuka on 20.06.2025.
//

#ifndef SUNGEARENGINE_ATLAS_H
#define SUNGEARENGINE_ATLAS_H

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

        void findBestRect(glm::ivec2 textureSize, rectpack2D::rect_xywh& outputRect, size_t rectHash) noexcept;
        void packTexture(const rectpack2D::rect_xywh& inRect, const ITexture2D* texture) noexcept;

        /**
         * @param hash Hash of rect that was passed to findBestRect function.
         * @return Nullptr if not found or rect by hash.
         */
        const rectpack2D::rect_xywh* getRectByHash(size_t hash) const noexcept;

        [[nodiscard]] bool contains(size_t rectHash) const noexcept;

        [[nodiscard]] Ref<ITexture2D> getTexture() const noexcept;
        [[nodiscard]] glm::u32vec2 getSize() const noexcept;

    private:
        using spaces_type = rectpack2D::empty_spaces<false, rectpack2D::default_empty_spaces>;
        using rect_type = rectpack2D::output_rect_t<spaces_type>;

        std::vector<rectpack2D::rect_xywh> m_atlasRects;
        // first - rect hash. second - index in m_atlasRects
        std::unordered_map<size_t, size_t> m_atlasRectsMap;

        glm::u32vec2 m_atlasSize { 0, 0 };

        Ref<ITexture2D> m_atlasTexture;

        bool tryPackLastInsertedRect() noexcept;

        void sortRects() noexcept;

        static void convertTextureFormatToRGBA32INT(const std::uint8_t* srcBuffer, std::uint8_t* dstBuffer, size_t pixelsCount, const std::vector<std::uint8_t>& srcChannelsBits, SGGDataType srcBufferDataType) noexcept;

        void resizeAtlasForTexture(glm::ivec2 textureSize) noexcept;
    };
}

#endif // SUNGEARENGINE_ATLAS_H
