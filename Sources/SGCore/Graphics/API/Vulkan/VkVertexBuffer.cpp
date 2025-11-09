//
// Created by stuka on 07.07.2023.
//

#include "VkVertexBuffer.h"

SGCore::VkVertexBuffer::~VkVertexBuffer() noexcept
{
    destroy();
}

void SGCore::VkVertexBuffer::create() noexcept
{

}

void SGCore::VkVertexBuffer::create(const size_t& byteSize) noexcept
{

}

void SGCore::VkVertexBuffer::destroy() noexcept
{

}

void SGCore::VkVertexBuffer::subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset,
                                               bool isPutData) noexcept
{

}

void SGCore::VkVertexBuffer::bind() noexcept
{

}

void SGCore::VkVertexBuffer::setUsage(SGGUsage) noexcept
{

}

void SGCore::VkVertexBuffer::addAttribute(std::uint32_t location, std::int32_t scalarsCount, SGGDataType dataType,
                                          bool isNormalized, std::int32_t stride, std::uint64_t offsetInStruct,
                                          std::int32_t divisor) noexcept
{
}

void SGCore::VkVertexBuffer::useAttributes() const noexcept
{
}

