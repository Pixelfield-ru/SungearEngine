//
// Created by stuka on 08.11.2025.
//

#include "IVertexBuffer.h"

const std::vector<std::uint8_t>& SGCore::IVertexBuffer::getData() const noexcept
{
    return m_data;
}
