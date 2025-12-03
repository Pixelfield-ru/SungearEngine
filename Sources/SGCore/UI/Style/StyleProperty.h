#pragma once

#include <variant>
#include "Style.h"
#include "SGCore/UI/Utils.h"

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

    template<auto... Members>
    struct RefStyleProperty {
        using MemberType = std::remove_reference_t<std::invoke_result_t<decltype(Utils::FirstGenericOf<Members...>), Style>>;
    };
}
