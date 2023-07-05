//
// Created by stuka on 21.05.2023.
//

#include "IVertexBufferLayout.h"

std::shared_ptr<Core::Graphics::IVertexBufferLayout> Core::Graphics::IVertexBufferLayout::addAttribute(std::shared_ptr<IVertexAttribute> attribute) noexcept
{
    m_attributes.push_back(std::move(attribute));

    return shared_from_this();
}

void Core::Graphics::IVertexBufferLayout::removeAttribute(const std::shared_ptr<IVertexAttribute>& attribute) noexcept
{
    m_attributes.remove(attribute);
}
