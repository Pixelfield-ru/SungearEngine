//
// Created by ilya on 25.02.24.
//

#ifndef SUNGEARENGINE_DEFAULTFRAMERECEIVER_H
#define SUNGEARENGINE_DEFAULTFRAMERECEIVER_H

#include "SGCore/Graphics/API/IFrameBuffer.h"

namespace SGCore
{
    struct SimpleFrameReceiver
    {
        Ref<IFrameBuffer> m_receiver;
    };
}

#endif //SUNGEARENGINE_DEFAULTFRAMERECEIVER_H
