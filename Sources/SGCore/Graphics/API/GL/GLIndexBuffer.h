#pragma once

#ifndef SUNGEARENGINE_GLINDEXBUFFER_H
#define SUNGEARENGINE_GLINDEXBUFFER_H

#include "../IIndexBuffer.h"
#include "memory"

#include <glad/glad.h>

namespace Core::Graphics
{
    class GLIndexBuffer : public IIndexBuffer
    {
    private:
        GLuint m_handler = 0;
        std::vector<std::uint32_t> m_data = std::vector<std::uint32_t>();

    public:
        ~GLIndexBuffer() noexcept override;

        std::shared_ptr<IIndexBuffer> create() noexcept override;

        void destroy() noexcept final;

        std::shared_ptr<IIndexBuffer> putData(std::vector<std::uint32_t> data) noexcept override;
        std::shared_ptr<IIndexBuffer> subData(std::vector<std::uint32_t> data, const int& offset) noexcept override;
        std::shared_ptr<IIndexBuffer> bind() noexcept override;

        std::shared_ptr<IIndexBuffer> setUsage(SGGUsage) noexcept override;
    };
}

#endif //SUNGEARENGINE_GLINDEXBUFFER_H
