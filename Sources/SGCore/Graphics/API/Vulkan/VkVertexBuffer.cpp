//
// Created by stuka on 07.07.2023.
//

#include "VkVertexBuffer.h"

Core::Graphics::VkVertexBuffer::~VkVertexBuffer() noexcept
{
    destroy();
}

std::shared_ptr<Core::Graphics::IVertexBuffer> Core::Graphics::VkVertexBuffer::create() noexcept
{
    return shared_from_this();
}

void Core::Graphics::VkVertexBuffer::destroy() noexcept
{

}

std::shared_ptr<Core::Graphics::IVertexBuffer> Core::Graphics::VkVertexBuffer::putData(std::vector<float> data) noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IVertexBuffer>
Core::Graphics::VkVertexBuffer::subData(std::vector<float> data, const int& offset) noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IVertexBuffer> Core::Graphics::VkVertexBuffer::bind() noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IVertexBuffer> Core::Graphics::VkVertexBuffer::setUsage(SGGUsage) noexcept
{
    return shared_from_this();
}
