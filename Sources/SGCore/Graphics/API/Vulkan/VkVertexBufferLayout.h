//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKVERTEXBUFFERLAYOUT_H
#define SUNGEARENGINE_VKVERTEXBUFFERLAYOUT_H

#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "VkVertexAttribute.h"

namespace SGCore
{
    // TODO: impl VkVertexBufferLayout
    class VkVertexBufferLayout : public IVertexBufferLayout
    {
    public:
        std::shared_ptr<IVertexBufferLayout> prepare() noexcept override;

        std::shared_ptr<IVertexBufferLayout> reset() noexcept final;

        std::uint16_t getVertexAttributeSizeInLayout(const SGGDataType&) noexcept;

        VkVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType) noexcept override;
        VkVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) noexcept override;

        std::shared_ptr<IVertexBufferLayout> enableAttribute(const std::shared_ptr<IVertexAttribute>&) noexcept override;
        std::shared_ptr<IVertexBufferLayout> enableAttributes() noexcept override;

        std::shared_ptr<IVertexBufferLayout> addAttribute(std::shared_ptr<IVertexAttribute> attribute) noexcept override;
    };
}

#endif //SUNGEARENGINE_VKVERTEXBUFFERLAYOUT_H
