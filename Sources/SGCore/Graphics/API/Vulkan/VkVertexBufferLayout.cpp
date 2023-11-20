//
// Created by stuka on 07.07.2023.
//

#include "VkVertexBufferLayout.h"

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::VkVertexBufferLayout::prepare() noexcept
{
    return shared_from_this();
}

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::VkVertexBufferLayout::reset() noexcept
{
    return shared_from_this();
}

std::uint16_t SGCore::VkVertexBufferLayout::getVertexAttributeSizeInLayout(const SGGDataType&) noexcept
{
    return 0;
}

SGCore::VkVertexAttribute* SGCore::VkVertexBufferLayout::createVertexAttribute
(std::uint16_t ID, std::string name, SGGDataType dataType) noexcept
{
    return nullptr;
}

SGCore::VkVertexAttribute* SGCore::VkVertexBufferLayout::createVertexAttribute
(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) noexcept
{
    return nullptr;
}

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::VkVertexBufferLayout::enableAttribute
(const Ref<IVertexAttribute>&) noexcept
{
    return shared_from_this();
}

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::VkVertexBufferLayout::enableAttributes() noexcept
{
    return shared_from_this();
}

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::VkVertexBufferLayout::addAttribute
(Ref<IVertexAttribute> attribute) noexcept
{
    return shared_from_this();
}
