//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKVERTEXBUFFER_H
#define SUNGEARENGINE_VKVERTEXBUFFER_H

#include "SGCore/Graphics/API/IVertexBuffer.h"

namespace Core::Graphics
{
    // TODO: impl VkVertexBuffer
    class VkVertexBuffer : public IVertexBuffer
    {
    public:
        ~VkVertexBuffer() noexcept override;

        std::shared_ptr<IVertexBuffer> create() noexcept final;

        void destroy() noexcept final;

        std::shared_ptr<IVertexBuffer> putData(std::vector<float> data) noexcept final;
        std::shared_ptr<IVertexBuffer> subData(std::vector<float> data, const int& offset) noexcept final;
        std::shared_ptr<IVertexBuffer> bind() noexcept final;

        std::shared_ptr<IVertexBuffer> setUsage(SGGUsage) noexcept final;
    };
}

#endif //SUNGEARENGINE_VKVERTEXBUFFER_H
