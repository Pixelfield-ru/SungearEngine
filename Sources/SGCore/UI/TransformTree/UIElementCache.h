//
// Created by stuka on 08.02.2025.
//

#ifndef UIEELEMENTCACHE_H
#define UIEELEMENTCACHE_H

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <array>
#include <vector>

#include "SGCore/Utils/Macroses.h"
#include "SGCore/Utils/Unique/UniqueColor.h"

namespace SGCore::UI
{
    struct CSSStyle;

    /**
     * Used to store calculated properties of UI element.
     */
    struct UIElementCache
    {
        /// basic size of element
        glm::vec2 m_basicSize { };
        /**
         * Final size of element. this size can differ from basic size because element can be auto-resized.
         */
        glm::vec2 m_finalSize { };
        glm::vec4 m_backgroundColor { 1.0 };
        glm::vec4 m_color { 1.0 };

        glm::vec2 m_totalBorderSize { };

        /// column and row gaps
        glm::vec2 m_gap { };

        float m_topPadding = 0.0f;
        float m_rightPadding = 0.0f;
        float m_bottomPadding = 0.0f;
        float m_leftPadding = 0.0f;

        /// bottom-left, top-left, top-right, bottom-right
        std::array<glm::vec2, 4> m_borderRadiusCache { };

        float m_fontSize = 16.0f;

        // ===================================== tmp values! DO NOT CHANGE EXTERNALLY!

        // non copyable in context of UIElementCache
        UniqueColor m_uniqueColor;

        std::vector<CSSStyle*> m_currentFrameStyles;

        glm::vec2 m_contentSize { };

        // used to place children elements in current element considering wrapping, gaps and more
        glm::vec3 m_curLocalPositionForElements { };

        glm::vec2 m_lastRowSize { };

        /// used if nowrap to fit parent (this) element
        glm::vec2 m_childrenElementsScale { 1.0, 1.0 };

        /// used to calculate Z position
        std::int32_t m_layer = 0;

        UIElementCache() noexcept = default;

        copy_constructor(UIElementCache);
        move_constructor(UIElementCache);

        copy_operator(UIElementCache);
        move_operator(UIElementCache);
    };
}

#endif // UIEELEMENTCACHE_H
