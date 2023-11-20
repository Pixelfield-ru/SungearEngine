//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKUNIFORMBUFFER_H
#define SUNGEARENGINE_VKUNIFORMBUFFER_H

#include "SGCore/Graphics/API/IUniformBuffer.h"

namespace SGCore
{
    // TODO: impl VkUniformBuffer
    class VkUniformBuffer : public IUniformBuffer
    {
    private:
        void subDataOnGAPISide(const std::int64_t& offset, const int& size) noexcept final;
    public:
        ~VkUniformBuffer() override;

        std::shared_ptr<IUniformBuffer> bind() noexcept final;

        std::shared_ptr<IUniformBuffer> prepare() noexcept final;

        std::shared_ptr<IUniformBuffer> updateLocations(const IShader& fromShader) noexcept final;

        void setLayoutLocation(const std::uint16_t& location) noexcept final;

        void destroy() noexcept final;
    };
}

#endif //SUNGEARENGINE_VKUNIFORMBUFFER_H
