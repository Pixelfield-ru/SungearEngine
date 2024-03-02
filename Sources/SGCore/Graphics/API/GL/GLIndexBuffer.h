#pragma once

#ifndef SUNGEARENGINE_GLINDEXBUFFER_H
#define SUNGEARENGINE_GLINDEXBUFFER_H

#include "../IIndexBuffer.h"
#include "memory"

#include <glad/glad.h>

namespace SGCore
{
    class GLIndexBuffer : public IIndexBuffer
    {
    private:
        GLuint m_handler = 0;

    public:
        ~GLIndexBuffer() noexcept override;

        std::shared_ptr<IIndexBuffer> create() noexcept override;

        void destroy() noexcept final;

        std::shared_ptr<IIndexBuffer> putData(const std::vector<std::uint32_t>& data) noexcept override;
        void subData(const std::vector<std::uint32_t>& data, const int& offset) noexcept override;
        void subData(std::uint32_t* data, const size_t& elementsCount, const int& offset) noexcept override;
        std::shared_ptr<IIndexBuffer> bind() noexcept override;

        std::shared_ptr<IIndexBuffer> setUsage(SGGUsage) noexcept override;
    };
}

#endif //SUNGEARENGINE_GLINDEXBUFFER_H
