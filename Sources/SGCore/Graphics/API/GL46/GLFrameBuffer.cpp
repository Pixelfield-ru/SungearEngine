//
// Created by stuka on 24.04.2023.
//1

#include <glad/glad.h>
#include "../IFrameBuffer.h"
#include "GLFrameBuffer.h"

Core::Graphics::API::GL46::GLFrameBuffer::GLFrameBuffer() noexcept
{
    glGenFramebuffers(1, &m_handler);
}

Core::Graphics::API::GL46::GLFrameBuffer::~GLFrameBuffer() noexcept
{
    glDeleteFramebuffers(1, &m_handler);
}