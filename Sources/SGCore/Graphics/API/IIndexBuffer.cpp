//
// Created by stuka on 04.07.2023.
//

#include "IIndexBuffer.h"

const std::vector<std::uint32_t> SGCore::IIndexBuffer::getData() const noexcept
{
    return m_data;
}
