//
// Created by stuka on 08.11.2025.
//

#include "IVertexBuffer.h"

#include "IVertexArray.h"

SGCore::IVertexBuffer::~IVertexBuffer()
{
    if(m_parentVertexArray)
    {
        m_parentVertexArray->m_vertexBuffers.erase(this);
    }
}

const std::vector<std::uint8_t>& SGCore::IVertexBuffer::getData() const noexcept
{
    return m_data;
}
