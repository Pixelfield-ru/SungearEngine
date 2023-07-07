//
// Created by stuka on 07.07.2023.
//

#include "VkUniformBuffer.h"

Core::Graphics::VkUniformBuffer::~VkUniformBuffer()
{
    destroy();
}

void Core::Graphics::VkUniformBuffer::subDataOnGAPISide(const int64_t& offset, const int& size) noexcept
{

}

std::shared_ptr<Core::Graphics::IUniformBuffer> Core::Graphics::VkUniformBuffer::bind() noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IUniformBuffer> Core::Graphics::VkUniformBuffer::prepare() noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IUniformBuffer> Core::Graphics::VkUniformBuffer::updateLocations
(const Core::Graphics::IShader& fromShader) noexcept
{
    return shared_from_this();
}

void Core::Graphics::VkUniformBuffer::setLayoutLocation(const uint16_t& location) noexcept
{

}

void Core::Graphics::VkUniformBuffer::destroy() noexcept
{

}
