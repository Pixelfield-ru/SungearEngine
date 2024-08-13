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

        std::shared_ptr<IVertexBuffer> create() noexcept final;
        std::shared_ptr<IVertexBuffer> create(const size_t& byteSize) noexcept final;
        void destroy() noexcept final;
        
        std::shared_ptr<IVertexBuffer> bind() noexcept final;

        std::shared_ptr<IVertexBuffer> setUsage(SGGUsage) noexcept final;
    
    protected:
        void subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset, bool isPutData) noexcept override;
    };
}

#endif //SUNGEARENGINE_VKVERTEXBUFFER_H
