//
// Created by stuka on 12.05.2025.
//

#ifndef SUNGEARENGINE_CSSFONTSIZEPROPERTYPROCESSOR_H
#define SUNGEARENGINE_CSSFONTSIZEPROPERTYPROCESSOR_H

#include "CSSPropertyProcessorCommon.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3Parser.h"
#include "SGCore/UI/CSS/ANTLRCSSListener.h"
#include "SGCore/UI/CSS/PropertyValues/CSSPropertyValueKeywords.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_FONT_SIZE>
    {
        static constexpr FontSizeKeyword property_default_value = FontSizeKeyword::KW_MEDIUM;
        static inline const std::string property_default_value_str = "medium";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {
            CSSSelector* currentSelector = antlrcssListener->m_currentSelector;

            auto* expr = propertyDeclContext->expr();

            if(expr->term().size() >= 1)
            {
                auto* knownTerm = dynamic_cast<css3Parser::KnownTermContext*>(expr->term(0));

                auto termValue = CSSPropertyProcessorCommon::processKnownTerm<UniversalKeyword>(antlrcssListener, currentSelector, knownTerm, 0, propertyName);

                if(std::holds_alternative<Ref<CSSMathNode>>(termValue))
                {
                    auto mathNode = std::get<1>(termValue);
                    currentSelector->m_fontSize.m_value = mathNode;
                }
            }
        }
    };
}

#endif // SUNGEARENGINE_CSSFONTSIZEPROPERTYPROCESSOR_H
