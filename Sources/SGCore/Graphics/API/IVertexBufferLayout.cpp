//
// Created by stuka on 21.05.2023.
//

#include "IVertexBufferLayout.h"

Core::Graphics::API::IVertexBufferLayout* Core::Graphics::API::IVertexBufferLayout::addAttribute(std::shared_ptr<IVertexAttribute> attribute) noexcept
{
    m_attributes.push_back(std::move(attribute));

    return this;
}

void Core::Graphics::API::IVertexBufferLayout::removeAttribute(const std::shared_ptr<IVertexAttribute>& attribute) noexcept
{
    m_attributes.remove(attribute);
}
