#pragma once

#include <variant>
#include "../Style/Style.h"
#include "SGCore/UI/Utils.h"
#include "SGCore/UI/DValue/DValueDestinationCacheNode.h"

namespace SGCore::UI::DValue
{
    /**
     * @brief Represents simple style property, like `width`, `gap` and so on.
     */
    template<typename T>
    struct DValueCowNode
    {
        std::variant<T, DValueCacheNode<T>> m_value;

        copy_constructor(DValueCowNode);
        move_constructor(DValueCowNode);

        explicit(false) DValueCowNode(T& initialValue) noexcept : m_value(initialValue) {}
        explicit(false) DValueCowNode(T&& initialValue) noexcept : m_value(std::move(initialValue)) {}

        ~DValueCowNode() noexcept = default;

        T& getValue() noexcept;
    };
}
