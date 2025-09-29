//
// Created by stuka on 24.01.2025.
//

#ifndef SUNGEARENGINE_CSSDISPLAYPROPERTYPROCESSOR_H
#define SUNGEARENGINE_CSSDISPLAYPROPERTYPROCESSOR_H

#include "CSSPropertyProcessor.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_DISPLAY>
    {
        static constexpr DisplayKeyword property_default_value = DisplayKeyword::KW_FLEX;
        static inline const std::string property_default_value_str = "flex";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {
            auto* currentSelector = antlrcssListener->m_currentStyle;

            currentSelector->m_display = getDisplayKeywordFromStringValue(propertyCurrentValue);

            if(currentSelector->m_display == DisplayKeyword::KW_UNKNOWN)
            {
                currentSelector->m_display = property_default_value;
                antlrcssListener->printUnknownKeywordUsedError(propertyName, propertyCurrentValue, property_default_value_str);
            }
        }
    };
}

#endif //SUNGEARENGINE_CSSDISPLAYPROPERTYPROCESSOR_H
