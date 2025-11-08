//
// Created by stuka on 09.11.2025.
//

#include "IVertexArray.h"

#include "IIndexBuffer.h"
#include "IVertexBuffer.h"

SGCore::IVertexArray::~IVertexArray()
{
    for(auto* vertexBuffer : m_vertexBuffers)
    {
        vertexBuffer->m_parentVertexArray = nullptr;
    }

    if(m_indexBuffer)
    {
        m_indexBuffer->m_parentVertexArray = nullptr;
    }
}

void SGCore::IVertexArray::addVertexBuffer(IVertexBuffer* vertexBuffer) noexcept
{
    m_vertexBuffers.insert(vertexBuffer);
}

void SGCore::IVertexArray::setIndexBuffer(IIndexBuffer* indexBuffer) noexcept
{
    m_indexBuffer = indexBuffer;
}

const std::unordered_set<SGCore::IVertexBuffer*>& SGCore::IVertexArray::getVertexBuffers() noexcept
{
    return m_vertexBuffers;
}

SGCore::IIndexBuffer* SGCore::IVertexArray::getIndexBuffer() noexcept
{
    return m_indexBuffer;
}
