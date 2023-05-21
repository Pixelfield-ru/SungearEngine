//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GL46FRAMEBUFFER_H
#define SUNGEARENGINE_GL46FRAMEBUFFER_H

namespace Core::Graphics::API::GL::GL46
{
    class GL46FrameBuffer : public IFrameBuffer
    {
    private:
        GLuint m_handler;

    public:
        GL46FrameBuffer() noexcept;

        ~GL46FrameBuffer() noexcept;
    };
}

#endif //SUNGEARENGINE_GL46FRAMEBUFFER_H
