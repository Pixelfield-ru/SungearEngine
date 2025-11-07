//
// Created by stuka on 07.07.2023.
//

#include "VkIndexBuffer.h"

SGCore::VkIndexBuffer::~VkIndexBuffer() noexcept
{
    destroy();
}

void SGCore::VkIndexBuffer::create() noexcept
{

}

void SGCore::VkIndexBuffer::create(const size_t& byteSize) noexcept
{

}


void SGCore::VkIndexBuffer::destroy() noexcept
{

}

void SGCore::VkIndexBuffer::putData(const std::vector<std::uint32_t>& data) noexcept
{

}

void SGCore::VkIndexBuffer::subData
(const std::vector<std::uint32_t>& data, const int& offset) noexcept
{

}

void SGCore::VkIndexBuffer::subData(std::uint32_t* data, const size_t& elementsCount, const int& offset) noexcept
{

}

void SGCore::VkIndexBuffer::bind() noexcept
{

}

void SGCore::VkIndexBuffer::setUsage(SGGUsage) noexcept
{

}