#pragma once

#include <variant>
#include "Style.h"

namespace SGCore::UI
{
    /**
     * @brief Represents simple style property, like `width`, `gap` and so on.
     */
    template<typename Underlying>
    struct StyleProperty
    {
        std::variant<Underlying> m_value;
    };

    template<auto... Member>
    struct RefStyleProperty {
        // using MemberType = std::remove_reference_t<std::invoke_result_t<decltype(Member), Style>>;
    };
}