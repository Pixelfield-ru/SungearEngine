
//
// Created by stuka on 15.02.2025.
//

#ifndef UIVERTEX_H
#define UIVERTEX_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace SGCore::UI
{
    struct UIVertex
    {
        glm::vec3 m_position;
        glm::vec2 m_uv;

        // for 9-slicing
        std::int32_t m_sliceIndex = 0;
    };
}

#endif //UIVERTEX_H
