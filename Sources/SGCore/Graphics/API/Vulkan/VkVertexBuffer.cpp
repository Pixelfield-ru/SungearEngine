//
// Created by stuka on 07.07.2023.
//

#include "VkVertexBuffer.h"

SGCore::VkVertexBuffer::~VkVertexBuffer() noexcept
{
    destroy();
}

std::shared_ptr<SGCore::IVertexBuffer> SGCore::VkVertexBuffer::create() noexcept
{
    return shared_from_this();
}

void SGCore::VkVertexBuffer::destroy() noexcept
{

}

std::shared_ptr<SGCore::IVertexBuffer> SGCore::VkVertexBuffer::putData(const std::vector<float>& data) noexcept
{
    return shared_from_this();
}

void
SGCore::VkVertexBuffer::subData(const std::vector<float>& data, const size_t& offset) noexcept
{

}

void SGCore::VkVertexBuffer::subData(float* data, const size_t& elementsCount, const size_t& offset) noexcept
{

}

std::shared_ptr<SGCore::IVertexBuffer> SGCore::VkVertexBuffer::bind() noexcept
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IVertexBuffer> SGCore::VkVertexBuffer::setUsage(SGGUsage) noexcept
{
    return shared_from_this();
}
