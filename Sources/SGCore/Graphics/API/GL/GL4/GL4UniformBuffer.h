#pragma once

#ifndef SUNGEARENGINE_GL4UNIFORMBUFFER_H
#define SUNGEARENGINE_GL4UNIFORMBUFFER_H

#include "SGCore/Graphics/API/IUniformBuffer.h"

namespace SGCore
{
    class GL4UniformBuffer : public IUniformBuffer
    {
    public:
        std::uint32_t m_handler = 0;

        void subDataOnGAPISide(const std::int64_t& offset, const int& size) noexcept final;
    public:
        ~GL4UniformBuffer() override;

        std::shared_ptr<IUniformBuffer> bind() noexcept final;

        std::shared_ptr<IUniformBuffer> prepare() noexcept final;

        void setLayoutLocation(const std::uint16_t& location) noexcept final;

        void destroy() noexcept final;
    };
}

#endif //SUNGEARENGINE_GL4UNIFORMBUFFER_H
