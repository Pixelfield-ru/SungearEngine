//
// Created by stuka on 07.07.2023.
//

#include "VkVertexArray.h"

Core::Graphics::VkVertexArray::~VkVertexArray() noexcept
{
    destroy();
}

std::shared_ptr<Core::Graphics::IVertexArray> Core::Graphics::VkVertexArray::create() noexcept
{
    return shared_from_this();
}

void Core::Graphics::VkVertexArray::destroy() noexcept
{

}

std::shared_ptr<Core::Graphics::IVertexArray> Core::Graphics::VkVertexArray::bind() noexcept
{
    return shared_from_this();
}
