#pragma once

#ifndef SUNGEARENGINE_GLINDEXBUFFER_H
#define SUNGEARENGINE_GLINDEXBUFFER_H

#include <glad/glad.h>

#include "SGCore/Graphics/API/IIndexBuffer.h"

namespace SGCore
{
    class GLIndexBuffer : public IIndexBuffer
    {
        friend struct GL4Renderer;

    public:
        ~GLIndexBuffer() noexcept override;

        void create() noexcept override;
        void create(const size_t& byteSize) noexcept override;

        void destroy() noexcept final;

        void putData(const std::vector<std::uint32_t>& data) noexcept override;
        void subData(const std::vector<std::uint32_t>& data, const int& offset) noexcept override;
        void subData(std::uint32_t* data, const size_t& elementsCount, const int& offset) noexcept override;
        void bind() noexcept override;

        void setUsage(SGGUsage) noexcept override;

    private:
        GLIndexBuffer() noexcept = default;

        GLuint m_handler = 0;
    };
}

#endif //SUNGEARENGINE_GLINDEXBUFFER_H
