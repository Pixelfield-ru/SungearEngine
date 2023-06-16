#pragma once

#ifndef SUNGEARENGINE_GL46UNIFORMBUFFER_H
#define SUNGEARENGINE_GL46UNIFORMBUFFER_H

#include "SGCore/Graphics/API/IUniformBuffer.h"

namespace Core::Graphics::API::GL::GL46
{
    class GL46UniformBuffer : public IUniformBuffer
    {
    private:
        std::uint32_t m_handler = 0;

    public:
        std::shared_ptr<IUniformBuffer> bind() noexcept final;

        std::shared_ptr<IUniformBuffer> prepare() noexcept final;

        std::shared_ptr<IUniformBuffer> updateLocations(const IShader& fromShader) noexcept final;

        void destroy() noexcept final;
    };
}

#endif //SUNGEARENGINE_GL46UNIFORMBUFFER_H
