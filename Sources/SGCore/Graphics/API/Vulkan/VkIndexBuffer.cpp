//
// Created by stuka on 07.07.2023.
//

#include "VkIndexBuffer.h"

Core::Graphics::VkIndexBuffer::~VkIndexBuffer() noexcept
{
    destroy();
}

std::shared_ptr<Core::Graphics::IIndexBuffer> Core::Graphics::VkIndexBuffer::create() noexcept
{
    return shared_from_this();
}

void Core::Graphics::VkIndexBuffer::destroy() noexcept
{

}

std::shared_ptr<Core::Graphics::IIndexBuffer> Core::Graphics::VkIndexBuffer::putData(std::vector<std::uint32_t> data) noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IIndexBuffer> Core::Graphics::VkIndexBuffer::subData
(std::vector<std::uint32_t> data, const int& offset) noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IIndexBuffer> Core::Graphics::VkIndexBuffer::bind() noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IIndexBuffer> Core::Graphics::VkIndexBuffer::setUsage(SGGUsage) noexcept
{
    return shared_from_this();
}
