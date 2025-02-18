//
// Created by stuka on 08.02.2025.
//

#ifndef CSSSELECTORCACHE_H
#define CSSSELECTORCACHE_H

#include <glm/vec4.hpp>

namespace SGCore::UI
{
    /**
     * Used to store calculated properties of UI element.
     */
    struct UIElementCache
    {
        glm::vec2 m_size { };
        glm::vec4 m_backgroundColor { };

        float m_totalBorderWidth = 0.0f;

        // column and row gaps
        glm::vec2 m_gap { };

        float m_padding = 0.0f;

        // used to place children elements in current element considering wrapping, gaps and more
        glm::vec3 m_curPositionForElements { };
        // free space inside the current element, which is left for children elements
        glm::vec2 m_freeSpaceSize { };
    };
}

#endif //CSSSELECTORCACHE_H
