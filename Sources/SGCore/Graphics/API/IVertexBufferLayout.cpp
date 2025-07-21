//
// Created by stuka on 21.05.2023.
//

#include "IVertexBufferLayout.h"

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::IVertexBufferLayout::addAttribute(Ref<IVertexAttribute> attribute) noexcept
{
    m_attributes.push_back(std::move(attribute));

    return shared_from_this();
}

void SGCore::IVertexBufferLayout::removeAttribute(const Ref<IVertexAttribute>& attribute) noexcept
{
    m_attributes.remove(attribute);
}

std::uint16_t SGCore::IVertexBufferLayout::getMaxAttributeID() const noexcept
{
    std::uint16_t maxAttributeID = 0;
    for(const auto& attribute : m_attributes)
    {
        if(attribute->m_ID > maxAttributeID) maxAttributeID = attribute->m_ID;
    }

    return maxAttributeID;
}
