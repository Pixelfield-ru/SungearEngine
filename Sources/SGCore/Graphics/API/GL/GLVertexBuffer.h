//
// Created by stuka on 23.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLVERTEXBUFFER_H
#define SUNGEARENGINE_GLVERTEXBUFFER_H

#include <glad/glad.h>

#include "SGCore/Graphics/API/IVertexBuffer.h"

namespace SGCore
{
    class GLVertexBuffer : public IVertexBuffer
    {
    private:
        GLuint m_handler = 0;

    public:
        ~GLVertexBuffer() noexcept override;

        std::shared_ptr<IVertexBuffer> create() noexcept override;
        std::shared_ptr<IVertexBuffer> create(const size_t& byteSize) noexcept override;
        void destroy() noexcept override;
        
        std::shared_ptr<IVertexBuffer> bind() noexcept override;

        std::shared_ptr<IVertexBuffer> setUsage(SGGUsage) noexcept override;
        
    protected:
        void subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset, bool isPutData) noexcept override;
    };
}

#endif //SUNGEARENGINE_GLVERTEXBUFFER_H
