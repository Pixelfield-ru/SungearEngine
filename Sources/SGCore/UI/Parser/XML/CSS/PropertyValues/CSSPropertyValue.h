//
// Created by stuka on 23.01.2025.
//

#ifndef SUNGEARENGINE_CSSPROPERTYVALUE_H
#define SUNGEARENGINE_CSSPROPERTYVALUE_H

#include <variant>
#include "CSSPropertyValueKeywords.h"

namespace SGCore::UI
{
    /**
     * css:
     * .selector {
     *     width: inherit;
     *     height: calc(100% - 5px);
     * }\n
     * decltype(inherit) == decltype(DefaultKeyword) (alternative value).\n
     * decltype(calc(100% - 5px)) == decltype(ValuesT...) (main value)
     * @tparam DefaultKeyword Used to set default value of property.
     *                        Also used to know the type of enum of keywords that is used to set as alternative value.
     * @tparam ValuesT - Used to accept implicit bunch of value (it can be keywords or something else)
     */
    template<auto DefaultKeyword, typename... ValuesT>
    struct PropertyValue
    {
        using keywords_enum_t = decltype(DefaultKeyword);

        static constexpr size_t values_count = sizeof...(ValuesT);
        static constexpr keywords_enum_t default_keyword = DefaultKeyword;

        std::variant<keywords_enum_t, std::tuple<ValuesT...>> m_value = DefaultKeyword;

        void setWithKeyword(keywords_enum_t keyword) noexcept
        {
            m_value = keyword;
        }

        void setWithAlternative(const std::tuple<ValuesT...>& alternativeValues) noexcept
        {
            m_value = alternativeValues;
        }

        auto& getKeyword() noexcept
        {
            return std::get<0>(m_value);
        }

        const auto& getKeyword() const noexcept
        {
            return std::get<0>(m_value);
        }

        auto& getAlternative() noexcept
        {
            return std::get<1>(m_value);
        }

        const auto& getAlternative() const noexcept
        {
            return std::get<1>(m_value);
        }

        template<size_t Index>
        auto& getFromAlternativeValue() noexcept
        {
            return std::get<Index>(std::get<1>(m_value));
        }

        template<size_t Index>
        const auto& getFromAlternativeValue() const noexcept
        {
            return std::get<Index>(std::get<1>(m_value));
        }

        bool containsKeyword() const noexcept
        {
            // return false;
            return m_value.valueless_by_exception() ? false : m_value.index() == 0;
        }

        bool containsAlternative() const noexcept
        {
            // return false;
            return m_value.valueless_by_exception() ? false : m_value.index() == 1;
        }
    };
}

#endif //SUNGEARENGINE_CSSPROPERTYVALUE_H
