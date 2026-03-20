//
// Created by stuka on 23.07.2023.
//

#pragma once

#include <sgcore_export.h>

#include "GraphicsDataTypes.h"

namespace SGCore
{
    struct SGCORE_EXPORT IFrameBufferAttachment
    {
        // USE ONLY FOR OPENGL
        std::uint32_t m_handler = 0;

        SGGColorFormat m_format = SGGColorFormat::SGG_RGB;
        SGGColorInternalFormat m_internalFormat = SGGColorInternalFormat::SGG_RGB8;

        int m_mipLevel = 0;
        int m_layer = 0;

        bool m_useMultisampling = false;
        std::uint8_t m_multisamplingSamplesCount = 8;
    };
}
