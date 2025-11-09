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

void SGCore::IVertexBuffer::addAttribute(std::uint32_t location,
                                         std::int32_t scalarsCount,
                                         SGGDataType dataType,
                                         bool isNormalized,
                                         std::int32_t stride,
                                         std::uint64_t offsetInStruct) noexcept
{
    addAttribute(location, scalarsCount, dataType, isNormalized, stride, offsetInStruct, 0);
}

const std::vector<std::uint8_t>& SGCore::IVertexBuffer::getData() const noexcept
{
    return m_data;
}
