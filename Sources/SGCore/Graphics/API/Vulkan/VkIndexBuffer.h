//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKINDEXBUFFER_H
#define SUNGEARENGINE_VKINDEXBUFFER_H

#include "SGCore/Graphics/API/IIndexBuffer.h"

namespace SGCore
{
    // TODO: impl VkIndexBuffer
    class VkIndexBuffer : public IIndexBuffer
    {
    public:
        ~VkIndexBuffer() noexcept override;

        std::shared_ptr<IIndexBuffer> create() noexcept final;
        std::shared_ptr<IIndexBuffer> create(const size_t& byteSize) noexcept final;

        void destroy() noexcept final;

        std::shared_ptr<IIndexBuffer> putData(const std::vector<std::uint32_t>& data) noexcept final;
        void subData(const std::vector<std::uint32_t>& data, const int& offset) noexcept final;
        void subData(std::uint32_t* data, const size_t& elementsCount, const int& offset) noexcept final;
        std::shared_ptr<IIndexBuffer> bind() noexcept final;

        std::shared_ptr<IIndexBuffer> setUsage(SGGUsage) noexcept final;

    };
}

#endif //SUNGEARENGINE_VKINDEXBUFFER_H
