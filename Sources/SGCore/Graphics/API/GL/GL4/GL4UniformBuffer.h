#pragma once

#ifndef SUNGEARENGINE_GL4UNIFORMBUFFER_H
#define SUNGEARENGINE_GL4UNIFORMBUFFER_H

#include "SGCore/Graphics/API/IUniformBuffer.h"

namespace SGCore
{
    class GL4UniformBuffer : public IUniformBuffer
    {
        friend struct GL4Renderer;

    public:
        std::uint32_t m_handler = 0;

        void subDataOnGAPISide(const std::int64_t& offset, const int& size) noexcept final;

        ~GL4UniformBuffer() override;

        void bind() noexcept final;

        void prepare() noexcept final;

        void setLayoutLocation(const std::uint16_t& location) noexcept final;

        void destroy() noexcept final;

    protected:
        GL4UniformBuffer() noexcept = default;
    };
}

#endif //SUNGEARENGINE_GL4UNIFORMBUFFER_H
