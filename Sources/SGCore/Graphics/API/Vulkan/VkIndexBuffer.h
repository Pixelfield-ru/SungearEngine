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

        void create() noexcept final;
        void create(const size_t& byteSize) noexcept final;

        void destroy() noexcept final;

        void putData(const std::vector<std::uint32_t>& data) noexcept final;
        void subData(const std::vector<std::uint32_t>& data, const int& offset) noexcept final;
        void subData(std::uint32_t* data, const size_t& elementsCount, const int& offset) noexcept final;
        void bind() noexcept final;

        void setUsage(SGGUsage) noexcept final;

    };
}

#endif //SUNGEARENGINE_VKINDEXBUFFER_H
