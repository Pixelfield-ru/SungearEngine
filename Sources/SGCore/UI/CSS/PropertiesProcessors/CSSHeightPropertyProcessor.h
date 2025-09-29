//
// Created by stuka on 24.01.2025.
//

#ifndef SUNGEARENGINE_CSSHEIGHTPROPERTYPROCESSOR_H
#define SUNGEARENGINE_CSSHEIGHTPROPERTYPROCESSOR_H

#include "CSSPropertyProcessor.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_HEIGHT>
    {
        static constexpr PositionAndSizeKeyword property_default_value = PositionAndSizeKeyword::KW_AUTO;
        static inline const std::string property_default_value_str = "auto";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {
            auto* currentSelector = antlrcssListener->m_currentStyle;

            auto* declExpr = propertyDeclContext->expr();

            currentSelector->m_height.m_value = property_default_value;

            if(declExpr->term().size() > 1) // invalid count of terms. ignoring property...
            {
                antlrcssListener->printInvalidCountOfTermsInPropertyError(propertyName,
                                                                          property_default_value_str,
                                                                          declExpr->term().size(),
                                                                          1);

                return;
            }

            auto* term = declExpr->term(0);
            auto* knownTerm = dynamic_cast<css3Parser::KnownTermContext*>(term);

            if(knownTerm)
            {
                if(knownTerm->calc()) // width: calc(...)
                {
                    auto mathNode = MakeRef<CSSMathNode>();

                    antlrcssListener->processCalculation(knownTerm->calc(),
                                                         propertyName,
                                                         mathNode,
                                                         { CSSDimensionQualifier::DQ_ANY });
                    mathNode->resolvePriorities();

                    currentSelector->m_height.m_value = mathNode;
                }
                else if(knownTerm->number())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();
                    mathNode->m_value = std::stof(knownTerm->number()->getText());

                    currentSelector->m_height.m_value = mathNode;
                }
                else if(knownTerm->dimension())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();

                    const std::string dimension = knownTerm->dimension()->Dimension()->getText();

                    mathNode->m_dimensionQualifier = getDimensionQualifierFromString(dimension);
                    mathNode->m_value = std::stof(dimension);

                    currentSelector->m_height.m_value = mathNode;
                }
                else if(knownTerm->percentage())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();

                    const std::string percentage = knownTerm->percentage()->Percentage()->getText();

                    mathNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PERCENT;
                    mathNode->m_value = std::stof(percentage);

                    currentSelector->m_height.m_value = mathNode;
                }
                else // we have keyword
                {
                    const auto keyword = getKeywordFromStringValue<PositionAndSizeKeyword>(propertyCurrentValue);

                    if(keyword == PositionAndSizeKeyword::KW_UNKNOWN)
                    {
                        antlrcssListener->printUnknownKeywordUsedError(propertyName, propertyCurrentValue, property_default_value_str);

                        return;
                    }

                    currentSelector->m_height.m_value = keyword;
                }
            }
            else // we have bad term. ignoring property...
            {
                antlrcssListener->printBadTermInPropertyError(propertyName, 0, term->getText(), property_default_value_str);
            }
        }
    };
}

#endif //SUNGEARENGINE_CSSHEIGHTPROPERTYPROCESSOR_H
