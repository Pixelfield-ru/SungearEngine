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
        Ref<IVertexBufferLayout> prepare() noexcept override;

        Ref<IVertexBufferLayout> reset() noexcept final;

        std::uint16_t getVertexAttributeSizeInLayout(const SGGDataType&) noexcept;

        VkVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType) noexcept override;
        VkVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) noexcept override;
        VkVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized, const size_t& divisor) noexcept override;
        VkVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, const size_t& divisor) noexcept override;
        VkVertexAttribute*
        createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType,
                              const std::uint16_t& scalarsCount, bool normalized, const size_t& stride,
                              const size_t& offset, const size_t& divisor) noexcept override;
        
        Ref<IVertexBufferLayout> enableAttribute(const Ref<IVertexAttribute>&) noexcept override;
        Ref<IVertexBufferLayout> enableAttributes() noexcept override;

        Ref<IVertexBufferLayout> addAttribute(Ref<IVertexAttribute> attribute) noexcept override;
    };
}

#endif //SUNGEARENGINE_VKVERTEXBUFFERLAYOUT_H
