//
// Created by stuka on 25.09.2025.
//

#include "UniqueColor.h"

#include "UniqueIDGenerator.h"

void SGCore::UniqueColor::generate() noexcept
{
    if(!m_id.valid())
    {
        UniqueIDGenerator::generate(m_id);
    }

    m_color.x = (float) (((m_id.id() + 1) & 0xFF0000) >> 16) / 255.0f;
    m_color.y = (float) (((m_id.id() + 1) & 0x00FF00) >> 8) / 255.0f;
    m_color.z = (float) ((m_id.id() + 1) & 0x0000FF) / 255.0f;

    std::cout << fmt::format("UNIQUE COLOR x = {}, y = {}, z = {}",
                     m_color.x, m_color.y, m_color.z) << std::endl;
}

glm::vec4 SGCore::UniqueColor::color() const noexcept
{
    return m_color;
}
