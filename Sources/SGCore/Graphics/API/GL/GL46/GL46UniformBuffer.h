#pragma once

#ifndef SUNGEARENGINE_GL46UNIFORMBUFFER_H
#define SUNGEARENGINE_GL46UNIFORMBUFFER_H

#include "SGCore/Graphics/API/IUniformBuffer.h"

namespace SGCore
{
    class GL46UniformBuffer : public IUniformBuffer
    {
    private:
        std::uint32_t m_handler = 0;

        void subDataOnGAPISide(const std::int64_t& offset, const int& size) noexcept final;
    public:
        ~GL46UniformBuffer() override;

        void bind() noexcept final;

        void prepare() noexcept final;

        void setLayoutLocation(const std::uint16_t& location) noexcept final;

        void destroy() noexcept final;
    };
}

#endif //SUNGEARENGINE_GL46UNIFORMBUFFER_H
