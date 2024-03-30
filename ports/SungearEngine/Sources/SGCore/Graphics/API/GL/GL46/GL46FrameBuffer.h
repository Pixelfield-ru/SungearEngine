//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GL46FRAMEBUFFER_H
#define SUNGEARENGINE_GL46FRAMEBUFFER_H

#include "SGCore/Graphics/API/GL/GL4/GL4FrameBuffer.h"

namespace SGCore
{
    class GL46FrameBuffer : public GL4FrameBuffer
    {
    public:
        ~GL46FrameBuffer() noexcept;
    };
}

#endif //SUNGEARENGINE_GL46FRAMEBUFFER_H
