//
// Created by stuka on 28.11.2025.
//

#pragma once

#include <variant>

#include "BindingsStorage.h"

namespace SGCore::UI
{
    template<typename T>
    struct AttributeValue
    {
        AttributeValue() = default;
        AttributeValue(const AttributeValue&) noexcept = default;
        AttributeValue(AttributeValue&&) noexcept = default;

        template<typename U>
        requires(std::is_convertible_v<U, T>)
        AttributeValue(U&& value) noexcept
        {
            m_value = std::forward<U>(value);
        }

        AttributeValue(Binding binding) noexcept
        {
            m_value = std::move(binding);
        }

        template<typename F0, typename F1>
        void valueOrBinding(F0&& valueFunc, F1&& bindingFunc) noexcept
        {
            if(std::holds_alternative<T>(m_value))
            {
                valueFunc(std::get<T>(m_value));
            }
            else
            {
                bindingFunc(std::get<Binding>(m_value));
            }
        }

        T* get() noexcept
        {
            if(std::holds_alternative<T>(m_value))
            {
                return &std::get<T>(m_value);
            }

            return std::get<Binding>(m_value).template getAs<T>();
        }

        const T* get() const noexcept
        {
            if(std::holds_alternative<T>(m_value))
            {
                return &std::get<T>(m_value);
            }

            return std::get<Binding>(m_value).template getAs<T>();
        }

        AttributeValue& operator=(const AttributeValue& other) noexcept = default;
        AttributeValue& operator=(AttributeValue&& other) noexcept = default;

        AttributeValue& operator=(Binding other) noexcept
        {
            if(this == std::addressof(other)) return *this;

            m_value = std::move(other);

            return *this;
        }

        template<typename U>
        requires(std::is_convertible_v<U, T>)
        AttributeValue& operator=(U&& value) noexcept
        {
            m_value = std::forward<U>(value);

            return *this;
        }

    private:
        std::variant<T, Binding> m_value = T{};
    };
}
