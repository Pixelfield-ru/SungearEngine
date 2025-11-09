//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKVERTEXBUFFER_H
#define SUNGEARENGINE_VKVERTEXBUFFER_H

#include "SGCore/Graphics/API/IVertexBuffer.h"

namespace SGCore
{
    // TODO: impl VkVertexBuffer
    class VkVertexBuffer : public IVertexBuffer
    {
    public:
        ~VkVertexBuffer() noexcept override;

        void create() noexcept final;
        void create(const size_t& byteSize) noexcept final;
        void destroy() noexcept final;
        
        void bind() noexcept final;

        void setUsage(SGGUsage) noexcept final;

        void addAttribute(std::uint32_t location,
                          std::int32_t scalarsCount,
                          SGGDataType dataType,
                          bool isNormalized,
                          std::int32_t stride,
                          std::uint64_t offsetInStruct,
                          std::int32_t divisor) noexcept final;

        void useAttributes() const noexcept final;
    
    protected:
        void subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset, bool isPutData) noexcept override;
    };
}

#endif //SUNGEARENGINE_VKVERTEXBUFFER_H
