//
// Created by stuka on 07.07.2023.
//

#include "VkVertexBufferLayout.h"

std::shared_ptr<SGCore::IVertexBufferLayout> SGCore::VkVertexBufferLayout::prepare() noexcept
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IVertexBufferLayout> SGCore::VkVertexBufferLayout::reset() noexcept
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

std::shared_ptr<SGCore::IVertexBufferLayout> SGCore::VkVertexBufferLayout::enableAttribute
(const std::shared_ptr<IVertexAttribute>&) noexcept
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IVertexBufferLayout> SGCore::VkVertexBufferLayout::enableAttributes() noexcept
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IVertexBufferLayout> SGCore::VkVertexBufferLayout::addAttribute
(std::shared_ptr<IVertexAttribute> attribute) noexcept
{
    return shared_from_this();
}
