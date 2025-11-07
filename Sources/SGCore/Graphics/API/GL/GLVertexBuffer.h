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
        friend struct GL4Renderer;

    public:
        ~GLVertexBuffer() noexcept override;

        void create() noexcept override;
        void create(const size_t& byteSize) noexcept override;
        void destroy() noexcept override;
        
        void bind() noexcept override;

        void setUsage(SGGUsage) noexcept override;
        
    protected:
        void subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset, bool isPutData) noexcept override;

    private:
        GLVertexBuffer() noexcept = default;

        GLuint m_handler = 0;
    };
}

#endif //SUNGEARENGINE_GLVERTEXBUFFER_H
