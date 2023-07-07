//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKINDEXBUFFER_H
#define SUNGEARENGINE_VKINDEXBUFFER_H

#include "SGCore/Graphics/API/IIndexBuffer.h"

namespace Core::Graphics
{
    // TODO: impl VkIndexBuffer
    class VkIndexBuffer : public IIndexBuffer
    {
    public:
        ~VkIndexBuffer() noexcept override;

        std::shared_ptr<IIndexBuffer> create() noexcept final;

        void destroy() noexcept final;

        std::shared_ptr<IIndexBuffer> putData(std::vector<std::uint32_t> data) noexcept final;
        std::shared_ptr<IIndexBuffer> subData(std::vector<std::uint32_t> data, const int& offset) noexcept final;
        std::shared_ptr<IIndexBuffer> bind() noexcept final;

        std::shared_ptr<IIndexBuffer> setUsage(SGGUsage) noexcept final;

    };
}

#endif //SUNGEARENGINE_VKINDEXBUFFER_H
