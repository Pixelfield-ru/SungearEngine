//
// Created by stuka on 08.02.2025.
//

#ifndef UIEELEMENTCACHE_H
#define UIEELEMENTCACHE_H

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

        glm::vec2 m_totalBorderSize { };

        /// column and row gaps
        glm::vec2 m_gap { };

        float m_topPadding = 0.0f;
        float m_rightPadding = 0.0f;
        float m_bottomPadding = 0.0f;
        float m_leftPadding = 0.0f;

        /// bottom-left, top-left, top-right, bottom-right
        std::array<glm::vec2, 4> m_borderRadiusCache;

        // ===================================== tmp values! DO NOT CHANGE EXTERNALLY!

        // used to place children elements in current element considering wrapping, gaps and more
        glm::vec3 m_curLocalPositionForElements { };
        // free space inside the current element, which is left for children elements
        glm::vec2 m_freeSpaceSize { };

        glm::vec2 m_lastRowSize { };
    };
}

#endif // UIEELEMENTCACHE_H
