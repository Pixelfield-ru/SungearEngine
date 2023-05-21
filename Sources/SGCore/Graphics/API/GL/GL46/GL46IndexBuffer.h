//
// Created by stuka on 21.05.2023.
//
#pragma once

#ifndef SUNGEARENGINE_GL46INDEXBUFFER_H
#define SUNGEARENGINE_GL46INDEXBUFFER_H

#include "SGCore/Graphics/API/IIndexBuffer.h"

namespace Core::Graphics::API::GL::GL46
{
    class GL46IndexBuffer : public IIndexBuffer
    {
    private:
        GLuint m_handler = 0;
        std::vector<int> m_data;
        GLenum m_usage = GL_STATIC_DRAW;

    public:
        ~GL46IndexBuffer() noexcept override;

        GL46IndexBuffer* create() noexcept override;

        void destroy() noexcept override;

        GL46IndexBuffer* putData(std::vector<int> data, bool subData) noexcept override;

        GL46IndexBuffer* bind() noexcept override;

        void setUsage(const int&) noexcept override;
    };
}

#endif //SUNGEARENGINE_GL46INDEXBUFFER_H
