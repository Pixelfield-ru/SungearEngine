//
// Created by stuka on 21.05.2023.
//
#pragma once

#ifndef SUNGEARENGINE_GLINDEXBUFFER_H
#define SUNGEARENGINE_GLINDEXBUFFER_H

#include "../IIndexBuffer.h"

#include "OpenGL/include/glad/glad.h"

namespace Core::Graphics::API::GL
{
    class GLIndexBuffer : public IIndexBuffer
    {
    private:
        GLuint m_handler = 0;
        std::vector<int> m_data;

    public:
        ~GLIndexBuffer() noexcept override;

        GLIndexBuffer* create() noexcept override;

        void destroy() noexcept override;

        GLIndexBuffer* putData(std::vector<int> data) noexcept override;
        GLIndexBuffer* subData(std::vector<int> data, const int& offset) noexcept override;
        GLIndexBuffer* bind() noexcept override;

        GLIndexBuffer* setUsage(SGGBufferUsage) noexcept override;
    };
}

#endif //SUNGEARENGINE_GLINDEXBUFFER_H
