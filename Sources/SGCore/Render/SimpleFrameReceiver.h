//
// Created by ilya on 25.02.24.
//

#pragma once

#include "SGCore/Graphics/API/IFrameBuffer.h"

namespace SGCore
{
    struct SGCORE_EXPORT SimpleFrameReceiver
    {
        Ref<IFrameBuffer> m_receiver;
    };
}
