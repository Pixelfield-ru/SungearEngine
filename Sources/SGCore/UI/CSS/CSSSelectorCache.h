//
// Created by stuka on 08.02.2025.
//

#ifndef CSSSELECTORCACHE_H
#define CSSSELECTORCACHE_H

#include <glm/vec4.hpp>

namespace SGCore::UI
{
    /**
     * Used to store calculated properties of CSS selector.
     */
    struct CSSSelectorCache
    {
        float m_width { };
        float m_height { };
        glm::vec4 m_backgroundColor { };

        float m_totalBorderWidth = 0.0f;
    };
}

#endif //CSSSELECTORCACHE_H
