//
// Created by stuka on 23.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLVERTEXBUFFER_H
#define SUNGEARENGINE_GLVERTEXBUFFER_H

#include "../IVertexBuffer.h"

#include <glad/glad.h>

namespace Core::Graphics
{
    class GLVertexBuffer : public IVertexBuffer
    {
    private:
        GLuint m_handler = 0;
        std::vector<float> m_data;

    public:
        ~GLVertexBuffer() noexcept override;

        std::shared_ptr<IVertexBuffer> create() noexcept override;

        void destroy() noexcept override;

        std::shared_ptr<IVertexBuffer> putData(std::vector<float> data) noexcept override;
        std::shared_ptr<IVertexBuffer> subData(std::vector<float> data, const int& offset) noexcept override;
        std::shared_ptr<IVertexBuffer> bind() noexcept override;

        std::shared_ptr<IVertexBuffer> setUsage(SGGUsage) noexcept override;
    };
}

#endif //SUNGEARENGINE_GLVERTEXBUFFER_H
