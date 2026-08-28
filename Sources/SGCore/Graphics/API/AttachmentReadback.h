//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <vector>

#include "GraphicsDataTypes.h"

namespace SGCore
{
    /// Result of a GPU → CPU pixel readback (IFrameBuffer::readAttachmentPixels,
    /// IRenderer::readScreenPixels): raw pixels plus the layout they are in.
    struct AttachmentReadback
    {
        std::int32_t m_width { };
        std::int32_t m_height { };
        SGGColorFormat m_format = SGGColorFormat::SGG_RGBA;
        SGGDataType m_dataType = SGGDataType::SGG_UNSIGNED_BYTE;
        std::int8_t m_channelsCount { };
        std::vector<std::uint8_t> m_data;
    };
}
