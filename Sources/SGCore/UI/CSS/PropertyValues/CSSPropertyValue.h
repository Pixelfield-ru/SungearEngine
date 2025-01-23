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
    struct CSSPropertyValue
    {
        static constexpr size_t values_count = sizeof...(ValuesT);
        using keywords_enum_t = decltype(DefaultKeyword);
        static constexpr keywords_enum_t default_keyword = DefaultKeyword;

        std::variant<keywords_enum_t, std::tuple<ValuesT...>> m_value = DefaultKeyword;
    };
}

#endif //SUNGEARENGINE_CSSPROPERTYVALUE_H
