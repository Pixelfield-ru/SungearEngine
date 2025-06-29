//
// Created by stuka on 20.06.2025.
//

#ifndef SUNGEARENGINE_ATLASRECT_H
#define SUNGEARENGINE_ATLASRECT_H

#include <glm/vec2.hpp>

namespace SGCore
{
    struct AtlasRect
    {
        // left bottom corner
        glm::ivec2 m_position { };
        glm::ivec2 m_size { };

        [[nodiscard]] std::int32_t area() const noexcept
        {
            return m_size.x * m_size.y;
        }
    };
}

#endif // SUNGEARENGINE_ATLASRECT_H
