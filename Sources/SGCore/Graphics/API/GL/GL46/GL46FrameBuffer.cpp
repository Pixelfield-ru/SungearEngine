//
// Created by stuka on 24.04.2023.
//1

#include "OpenGL/include/glad/glad.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "GL46FrameBuffer.h"

Core::Graphics::API::GL::GL46::GL46FrameBuffer::GL46FrameBuffer() noexcept
{
    glGenFramebuffers(1, &m_handler);
}

Core::Graphics::API::GL::GL46::GL46FrameBuffer::~GL46FrameBuffer() noexcept
{
    glDeleteFramebuffers(1, &m_handler);
}