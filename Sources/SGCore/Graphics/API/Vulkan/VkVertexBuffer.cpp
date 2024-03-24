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

std::shared_ptr<SGCore::IVertexBuffer> SGCore::VkVertexBuffer::create(const size_t& byteSize) noexcept
{
    return shared_from_this();
}

void SGCore::VkVertexBuffer::destroy() noexcept
{

}

void SGCore::VkVertexBuffer::subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset,
                                               bool isPutData) noexcept
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

