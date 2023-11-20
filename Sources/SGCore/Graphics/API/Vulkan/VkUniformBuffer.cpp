//
// Created by stuka on 07.07.2023.
//

#include "VkUniformBuffer.h"

SGCore::VkUniformBuffer::~VkUniformBuffer()
{
    destroy();
}

void SGCore::VkUniformBuffer::subDataOnGAPISide(const int64_t& offset, const int& size) noexcept
{

}

std::shared_ptr<SGCore::IUniformBuffer> SGCore::VkUniformBuffer::bind() noexcept
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IUniformBuffer> SGCore::VkUniformBuffer::prepare() noexcept
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IUniformBuffer> SGCore::VkUniformBuffer::updateLocations
(const SGCore::IShader& fromShader) noexcept
{
    return shared_from_this();
}

void SGCore::VkUniformBuffer::setLayoutLocation(const uint16_t& location) noexcept
{

}

void SGCore::VkUniformBuffer::destroy() noexcept
{

}
