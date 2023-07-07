//
// Created by stuka on 07.07.2023.
//

#include "VkVertexBufferLayout.h"

std::shared_ptr<Core::Graphics::IVertexBufferLayout> Core::Graphics::VkVertexBufferLayout::prepare() noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IVertexBufferLayout> Core::Graphics::VkVertexBufferLayout::reset() noexcept
{
    return shared_from_this();
}

std::uint16_t Core::Graphics::VkVertexBufferLayout::getVertexAttributeSizeInLayout(const SGGDataType&) noexcept
{
    return 0;
}

Core::Graphics::VkVertexAttribute* Core::Graphics::VkVertexBufferLayout::createVertexAttribute
(std::uint16_t ID, std::string name, SGGDataType dataType) noexcept
{
    return nullptr;
}

Core::Graphics::VkVertexAttribute* Core::Graphics::VkVertexBufferLayout::createVertexAttribute
(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) noexcept
{
    return nullptr;
}

std::shared_ptr<Core::Graphics::IVertexBufferLayout> Core::Graphics::VkVertexBufferLayout::enableAttribute
(const std::shared_ptr<IVertexAttribute>&) noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IVertexBufferLayout> Core::Graphics::VkVertexBufferLayout::enableAttributes() noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IVertexBufferLayout> Core::Graphics::VkVertexBufferLayout::addAttribute
(std::shared_ptr<IVertexAttribute> attribute) noexcept
{
    return shared_from_this();
}
