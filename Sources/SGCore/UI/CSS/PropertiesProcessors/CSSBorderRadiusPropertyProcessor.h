//
// Created by stuka on 22.02.2025.
//

#ifndef CSSROUNDINGPROPERTYPROCESSOR_H
#define CSSROUNDINGPROPERTYPROCESSOR_H

#include "SGCore/UI/CSS/CSSPropertyType.h"

#include "SGCore/Utils/TypeTraits.h"
#include "SGCore/UI/CSS/ANTLRCSSListener.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_BORDER_RADIUS>
    {
        static constexpr UniversalKeyword property_default_value = UniversalKeyword::KW_UNSET;
        static inline const std::string property_default_value_str = "unset";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {
            auto* currentSelector = antlrcssListener->m_currentSelector;

            auto* declExpr = propertyDeclContext->expr();

            currentSelector->m_topLeftBorderRadius.m_value = property_default_value;
            currentSelector->m_topRightBorderRadius.m_value = property_default_value;
            currentSelector->m_bottomRightBorderRadius.m_value = property_default_value;
            currentSelector->m_bottomLeftBorderRadius.m_value = property_default_value;

            if(declExpr->term().size() == 1)
            {
                auto* knownTerm = dynamic_cast<css3Parser::KnownTermContext*>(declExpr->term(0));

                if(knownTerm)
                {
                    auto termValue = processKnownTerm(antlrcssListener, currentSelector, knownTerm, propertyName);

                    if(std::holds_alternative<Ref<CSSMathNode>>(termValue))
                    {
                        const auto finalValue = BorderRadiusAlternativeValue {
                            .m_radiusX = std::get<1>(termValue),
                            .m_radiusY = std::get<1>(termValue)
                        };

                        currentSelector->m_topLeftBorderRadius.m_value = finalValue;
                        currentSelector->m_topRightBorderRadius.m_value = finalValue;
                        currentSelector->m_bottomRightBorderRadius.m_value = finalValue;
                        currentSelector->m_bottomLeftBorderRadius.m_value = finalValue;
                    }
                }
            }
            else if(declExpr->term().size() == 2)
            {
                auto* knownTerm0 = dynamic_cast<css3Parser::KnownTermContext*>(declExpr->term(0));
                auto* knownTerm1 = dynamic_cast<css3Parser::KnownTermContext*>(declExpr->term(1));

                if(knownTerm0 && knownTerm1)
                {
                    auto termValue0 = processKnownTerm(antlrcssListener, currentSelector, knownTerm0, propertyName);
                    auto termValue1 = processKnownTerm(antlrcssListener, currentSelector, knownTerm1, propertyName);

                    if(std::holds_alternative<UniversalKeyword>(termValue0))
                    {
                        antlrcssListener->printBadTermInPropertyError(propertyName, 0, knownTerm0->getText(), property_default_value_str);
                        return;
                    }

                    if(std::holds_alternative<UniversalKeyword>(termValue1))
                    {
                        antlrcssListener->printBadTermInPropertyError(propertyName, 1, knownTerm1->getText(), property_default_value_str);
                        return;
                    }

                    const auto finalValue = BorderRadiusAlternativeValue {
                        .m_radiusX = std::get<1>(termValue0),
                        .m_radiusY = std::get<1>(termValue1)
                    };

                    currentSelector->m_topLeftBorderRadius.m_value = finalValue;
                    currentSelector->m_topRightBorderRadius.m_value = finalValue;
                    currentSelector->m_bottomRightBorderRadius.m_value = finalValue;
                    currentSelector->m_bottomLeftBorderRadius.m_value = finalValue;
                }
            }
        }

    private:
        static std::variant<UniversalKeyword, Ref<CSSMathNode>> processKnownTerm(
            ANTLRCSSListener* antlrcssListener,
            CSSSelector* currentSelector,
            css3Parser::KnownTermContext* knownTerm,
            const std::string& propertyName) noexcept
        {
            if(knownTerm->calc()) // width: calc(...)
            {
                auto mathNode = MakeRef<CSSMathNode>();

                antlrcssListener->processCalculation(knownTerm->calc(),
                                                     propertyName,
                                                     mathNode,
                                                     { CSSDimensionQualifier::DQ_ANY });
                mathNode->resolvePriorities();

                return { mathNode };
            }
            if(knownTerm->number())
            {
                auto mathNode = MakeRef<CSSMathNumericNode>();
                mathNode->m_value = std::stof(knownTerm->number()->getText());

                return { mathNode };
            }
            if(knownTerm->dimension())
            {
                auto mathNode = MakeRef<CSSMathNumericNode>();

                const std::string dimension = knownTerm->dimension()->Dimension()->getText();

                mathNode->m_dimensionQualifier = getDimensionQualifierFromString(dimension);
                mathNode->m_value = std::stof(dimension);

                return { mathNode };
            }
            if(knownTerm->percentage())
            {
                auto mathNode = MakeRef<CSSMathNumericNode>();

                const std::string percentage = knownTerm->percentage()->Percentage()->getText();

                mathNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PERCENT;
                mathNode->m_value = std::stof(percentage);

                return { mathNode };
            }

            return property_default_value;
        }
    };
}

#endif //CSSROUNDINGPROPERTYPROCESSOR_H
