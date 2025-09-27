//
// Created by stuka on 25.09.2025.
//

#pragma once

#include <glm/vec4.hpp>

#include "UniqueID.h"

namespace SGCore
{
    struct UniqueColor
    {
        UniqueColor() noexcept;
        UniqueColor(const UniqueColor& color) noexcept;
        UniqueColor(UniqueColor&& color) noexcept;

        const UniqueID& id() const noexcept;
        glm::vec4 color() const noexcept;

        UniqueColor& operator=(const UniqueColor& color) noexcept;
        UniqueColor& operator=(UniqueColor&& color) noexcept;

    private:
        UniqueID m_id { };
        glm::vec4 m_color { };

        void generate() noexcept;
    };
}