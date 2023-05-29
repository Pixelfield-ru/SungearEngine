//
// Created by stuka on 23.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLVERTEXBUFFER_H
#define SUNGEARENGINE_GLVERTEXBUFFER_H

#include "../IVertexBuffer.h"

#include <glad/glad.h>

namespace Core::Graphics::API::GL
{
    class GLVertexBuffer : public IVertexBuffer
    {
    private:
        GLuint m_handler = 0;
        std::vector<float> m_data;

    public:
        ~GLVertexBuffer() noexcept override;

        GLVertexBuffer* create() noexcept override;

        void destroy() noexcept override;

        GLVertexBuffer* putData(std::vector<float> data) noexcept override;
        GLVertexBuffer* subData(std::vector<float> data, const int& offset) noexcept override;
        GLVertexBuffer* bind() noexcept override;

        GLVertexBuffer* setUsage(SGGBufferUsage) noexcept override;
    };
}

#endif //SUNGEARENGINE_GLVERTEXBUFFER_H
