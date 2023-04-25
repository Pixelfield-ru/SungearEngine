//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_GLFRAMEBUFFER_H
#define NATIVECORE_GLFRAMEBUFFER_H

namespace Core::Graphics::API::OpenGL
{
    class GLFrameBuffer : public IFrameBuffer
    {
    private:
        GLuint handler;

    public:
        GLFrameBuffer() noexcept;

        ~GLFrameBuffer() noexcept;
    };
}

#endif //NATIVECORE_GLFRAMEBUFFER_H
