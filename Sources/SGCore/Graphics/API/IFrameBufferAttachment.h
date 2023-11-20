//
// Created by stuka on 23.07.2023.
//

#ifndef SUNGEARENGINE_IFRAMEBUFFERATTACHMENT_H
#define SUNGEARENGINE_IFRAMEBUFFERATTACHMENT_H

#include "GraphicsDataTypes.h"

namespace SGCore
{
    struct IFrameBufferAttachment
    {

        SGGColorFormat m_format = SGGColorFormat::SGG_RGB;
        SGGColorInternalFormat m_internalFormat = SGGColorInternalFormat::SGG_RGB8;

        int m_mipLevel = 0;
        int m_layer = 0;
    };
}

#endif //SUNGEARENGINE_IFRAMEBUFFERATTACHMENT_H
