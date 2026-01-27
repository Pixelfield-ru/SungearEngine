#pragma once

#include <variant>
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

        copy_constructor(DValueCowNode) = default;
        move_constructor(DValueCowNode) = default;

        explicit(false) DValueCowNode(T& initialValue) noexcept : m_value(initialValue) {}
        explicit(false) DValueCowNode(T&& initialValue) noexcept : m_value(std::move(initialValue)) {}

        ~DValueCowNode() noexcept = default;

        T& getValue() const noexcept {
            if (auto val = std::get_if<T>(const_cast<decltype(m_value)*>(&this->m_value))) {
                return *val;
            }
            return std::get_if<DValueCacheNode<T>>(const_cast<decltype(m_value)*>(&m_value))->m_value;
        }

        template<typename TV> requires std::convertible_to<TV, T>
        void setValue(TV&& value) noexcept {
            m_value.template emplace<T>(std::forward<TV>(value));
        }

        T& operator*() const noexcept { return getValue(); }
        T* operator->() const noexcept { return &getValue(); }
    };
}
