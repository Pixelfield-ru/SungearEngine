//
// Created by stuka on 21.05.2023.
//
#pragma once

#ifndef SUNGEARENGINE_GLINDEXBUFFER_H
#define SUNGEARENGINE_GLINDEXBUFFER_H

#include "../IIndexBuffer.h"
#include "memory"

#include <glad/glad.h>

namespace Core::Graphics::API::GL
{
    class GLIndexBuffer : public IIndexBuffer
    {
    private:
        GLuint m_handler = 0;
        std::vector<int> m_data;

    public:
        ~GLIndexBuffer() noexcept override;

        std::shared_ptr<IIndexBuffer> create() noexcept override;

        void destroy() noexcept override;

        std::shared_ptr<IIndexBuffer> putData(std::vector<int> data) noexcept override;
        std::shared_ptr<IIndexBuffer> subData(std::vector<int> data, const int& offset) noexcept override;
        std::shared_ptr<IIndexBuffer> bind() noexcept override;

        std::shared_ptr<IIndexBuffer> setUsage(SGGBufferUsage) noexcept override;
    };
}

#endif //SUNGEARENGINE_GLINDEXBUFFER_H
