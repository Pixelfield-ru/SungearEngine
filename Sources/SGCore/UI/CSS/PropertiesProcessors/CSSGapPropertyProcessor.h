//
// Created by stuka on 02.03.2025.
//

#ifndef CSSGAPPROPERTYPROCESSOR_H
#define CSSGAPPROPERTYPROCESSOR_H

#include "CSSPropertyProcessorCommon.h"
#include "SGCore/UI/CSS/CSSPropertyType.h"
#include "SGCore/UI/CSS/ANTLRCSSListener.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_GAP>
    {
        static constexpr PositionAndSizeKeyword property_default_value = PositionAndSizeKeyword::KW_UNSET;
        static inline const std::string property_default_value_str = "unset";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {
            auto* currentSelector = antlrcssListener->m_currentStyle;

            auto* declExpr = propertyDeclContext->expr();

            currentSelector->m_gap.m_value = property_default_value;

            if(declExpr->term().size() == 1)
            {
                auto* knownTerm = dynamic_cast<css3Parser::KnownTermContext*>(declExpr->term(0));

                if(!knownTerm)
                {
                    antlrcssListener->printBadTermInPropertyError(propertyName, 0, declExpr->term(0)->getText(),
                                                                  property_default_value_str);
                    return;
                }

                auto termValue = CSSPropertyProcessorCommon::processKnownTerm<PositionAndSizeKeyword>(
                    antlrcssListener, currentSelector, knownTerm, 0, propertyName);

                if(std::holds_alternative<Ref<CSSMathNode>>(termValue))
                {
                    auto mathNode = std::get<1>(termValue);

                    currentSelector->m_gap.setWithAlternative({});

                    currentSelector->m_gap.getFromAlternativeValue<0>() = mathNode;
                    currentSelector->m_gap.getFromAlternativeValue<1>() = mathNode;
                }
                else
                {
                    // todo: keyword support
                }
            }
            else if(declExpr->term().size() == 2)
            {
                auto* knownTerm0 = dynamic_cast<css3Parser::KnownTermContext*>(declExpr->term(0));

                if(!knownTerm0)
                {
                    antlrcssListener->printBadTermInPropertyError(propertyName, 0, declExpr->term(0)->getText(),
                                                                  property_default_value_str);
                    return;
                }

                auto* knownTerm1 = dynamic_cast<css3Parser::KnownTermContext*>(declExpr->term(1));

                if(!knownTerm1)
                {
                    antlrcssListener->printBadTermInPropertyError(propertyName, 1, declExpr->term(1)->getText(),
                                                                  property_default_value_str);
                    return;
                }

                auto termValue0 = CSSPropertyProcessorCommon::processKnownTerm<PositionAndSizeKeyword>(
                    antlrcssListener, currentSelector, knownTerm0, 0, propertyName);

                auto termValue1 = CSSPropertyProcessorCommon::processKnownTerm<PositionAndSizeKeyword>(
                    antlrcssListener, currentSelector, knownTerm1, 1, propertyName);

                if(std::holds_alternative<PositionAndSizeKeyword>(termValue0))
                {
                    antlrcssListener->printBadTermInPropertyError(propertyName, 0, knownTerm0->getText(),
                                                                       property_default_value_str);
                    return;
                }

                if(std::holds_alternative<PositionAndSizeKeyword>(termValue1))
                {
                    antlrcssListener->printBadTermInPropertyError(propertyName, 1, knownTerm1->getText(),
                                                                       property_default_value_str);
                    return;
                }

                currentSelector->m_gap.setWithAlternative({});

                currentSelector->m_gap.getFromAlternativeValue<0>() = std::get<1>(termValue0);
                currentSelector->m_gap.getFromAlternativeValue<1>() = std::get<1>(termValue1);
            }
            else
            {
                antlrcssListener->printInvalidCountOfTermsInPropertyError(propertyName, property_default_value_str, declExpr->term().size(), 1, 2);
            }
        }
    };
}

#endif //CSSGAPPROPERTYPROCESSOR_H
