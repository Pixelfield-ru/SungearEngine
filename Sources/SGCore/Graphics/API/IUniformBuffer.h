#pragma once

#ifndef SUNGEARENGINE_IUNIFORMBUFFER_H
#define SUNGEARENGINE_IUNIFORMBUFFER_H

#include <memory>

namespace Core::Graphics::API
{
    class IUniformBuffer : std::enable_shared_from_this<IUniformBuffer>
    {
    private:
    public:
        virtual ~IUniformBuffer() = default;

        virtual std::shared_ptr<IUniformBuffer> create() = 0;

        virtual std::shared_ptr<IUniformBuffer> bind() = 0;

        virtual void destroy() = 0;
    };
}

#endif //SUNGEARENGINE_IUNIFORMBUFFER_H
