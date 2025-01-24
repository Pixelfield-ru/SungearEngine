//
// Created by stuka on 24.01.2025.
//

#ifndef SUNGEARENGINE_CSSWIDTHPROPERTYPROCESSOR_H
#define SUNGEARENGINE_CSSWIDTHPROPERTYPROCESSOR_H

#include "CSSPropertyProcessor.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_WIDTH>
    {
        static constexpr PositionAndSizeKeyword property_default_value = PositionAndSizeKeyword::KW_AUTO;
        static inline const std::string property_default_value_str = "auto";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {
            auto* currentSelector = antlrcssListener->m_currentSelector;

            auto* declExpr = propertyDeclContext->expr();

            currentSelector->m_width.m_value = property_default_value;

            if(declExpr->term().size() > 1) // invalid count of terms. ignoring property...
            {
                antlrcssListener->printInvalidCountOfTermsInPropertyError(propertyName,
                                                                          property_default_value_str,
                                                                          declExpr->term().size(),
                                                                          1);

                return;
            }

            auto outputValue =
                    antlrcssListener->getCalculablePropertyTermValue<PositionAndSizeKeyword::KW_AUTO>(propertyName,
                                                                                                      propertyCurrentValue,
                                                                                                      property_default_value_str,
                                                                                                      declExpr->term(0));

            if(outputValue.index() == 0)
            {
                currentSelector->m_width.m_value = std::get<0>(outputValue);
            }
            else if(outputValue.index() == 1)
            {
                currentSelector->m_width.m_value = std::get<1>(outputValue);
            }
        }
    };
}

#endif //SUNGEARENGINE_CSSWIDTHPROPERTYPROCESSOR_H
