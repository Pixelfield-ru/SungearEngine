//
// Created by stuka on 07.07.2023.
//

#include "VkVertexArray.h"

SGCore::VkVertexArray::~VkVertexArray() noexcept
{
    destroy();
}

std::shared_ptr<SGCore::IVertexArray> SGCore::VkVertexArray::create() noexcept
{
    return shared_from_this();
}

void SGCore::VkVertexArray::destroy() noexcept
{

}

void SGCore::VkVertexArray::bind() noexcept
{
}
