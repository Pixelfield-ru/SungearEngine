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
    struct GLVertexAttribute
    {
        GLuint location { };
        GLint size { };
        GLenum dataType { };
        GLboolean isNormalized { };
        GLsizei stride { };
        GLuint64 offset { };
        GLuint divisor { };
    };

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

        void addAttribute(std::uint32_t location,
                          std::int32_t scalarsCount,
                          SGGDataType dataType,
                          bool isNormalized,
                          std::int32_t stride,
                          std::uint64_t offsetInStruct,
                          std::int32_t divisor) noexcept final;

        void useAttributes() const noexcept final;

    protected:
        void subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset, bool isPutData) noexcept override;

        std::vector<GLVertexAttribute> m_attributes;

    private:
        GLVertexBuffer() noexcept = default;

        GLuint m_handler = 0;
    };
}

#endif //SUNGEARENGINE_GLVERTEXBUFFER_H
