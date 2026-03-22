//
// Created by stuka on 24.01.2025.
//

#pragma once

#include "CSSPropertyProcessor.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_FLEX_DIRECTION>
    {
        static constexpr FlexboxKeyword property_default_value = FlexboxKeyword::KW_ROW;
        static inline const std::string property_default_value_str = "row";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {
            auto* currentSelector = antlrcssListener->m_currentStyle;

            currentSelector->m_flexDirection = getFlexboxKeywordFromStringValue(propertyCurrentValue);

            if(currentSelector->m_flexDirection == FlexboxKeyword::KW_UNKNOWN)
            {
                currentSelector->m_flexDirection = property_default_value;
                antlrcssListener->printUnknownKeywordUsedError(propertyName, propertyCurrentValue, property_default_value_str);
            }
        }
    };
}
