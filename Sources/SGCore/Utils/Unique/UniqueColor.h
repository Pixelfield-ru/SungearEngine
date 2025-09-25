//
// Created by stuka on 25.09.2025.
//

#pragma once

#include <glm/vec4.hpp>

#include "UniqueID.h"

namespace SGCore
{
    /**
     * Usage:\n
     * SGCore::UniqueColor uniqueColor;\n
     * uniqueColor.generate();\n
     */
    struct UniqueColor
    {
        UniqueID m_id { };

        void generate() noexcept;

        glm::vec4 color() const noexcept;

    private:
        glm::vec4 m_color { };
    };
}