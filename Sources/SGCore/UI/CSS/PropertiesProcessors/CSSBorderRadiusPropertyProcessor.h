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

                if(knownTerm->calc()) // width: calc(...)
                {
                    auto mathNode = MakeRef<CSSMathNode>();

                    antlrcssListener->processCalculation(knownTerm->calc(),
                                                         propertyName,
                                                         mathNode,
                                                         { CSSDimensionQualifier::DQ_ANY });
                    mathNode->resolvePriorities();

                    const auto alternativeValue = BorderRadiusAlternativeValue {
                        .m_radiusX = mathNode,
                        .m_radiusY = mathNode
                    };

                    currentSelector->m_topLeftBorderRadius.m_value = alternativeValue;
                    currentSelector->m_topRightBorderRadius.m_value = alternativeValue;
                    currentSelector->m_bottomRightBorderRadius.m_value = alternativeValue;
                    currentSelector->m_bottomLeftBorderRadius.m_value = alternativeValue;
                }
                else if(knownTerm->number())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();
                    mathNode->m_value = std::stof(knownTerm->number()->getText());

                    const auto alternativeValue = BorderRadiusAlternativeValue {
                        .m_radiusX = mathNode,
                        .m_radiusY = mathNode
                    };

                    currentSelector->m_topLeftBorderRadius.m_value = alternativeValue;
                    currentSelector->m_topRightBorderRadius.m_value = alternativeValue;
                    currentSelector->m_bottomRightBorderRadius.m_value = alternativeValue;
                    currentSelector->m_bottomLeftBorderRadius.m_value = alternativeValue;
                }
                else if(knownTerm->dimension())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();

                    const std::string dimension = knownTerm->dimension()->Dimension()->getText();

                    mathNode->m_dimensionQualifier = getDimensionQualifierFromString(dimension);
                    mathNode->m_value = std::stof(dimension);

                    const auto alternativeValue = BorderRadiusAlternativeValue {
                        .m_radiusX = mathNode,
                        .m_radiusY = mathNode
                    };

                    currentSelector->m_topLeftBorderRadius.m_value = alternativeValue;
                    currentSelector->m_topRightBorderRadius.m_value = alternativeValue;
                    currentSelector->m_bottomRightBorderRadius.m_value = alternativeValue;
                    currentSelector->m_bottomLeftBorderRadius.m_value = alternativeValue;
                }
                else if(knownTerm->percentage())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();

                    const std::string percentage = knownTerm->percentage()->Percentage()->getText();

                    mathNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PERCENT;
                    mathNode->m_value = std::stof(percentage);

                    const auto alternativeValue = BorderRadiusAlternativeValue {
                        .m_radiusX = mathNode,
                        .m_radiusY = mathNode
                    };

                    currentSelector->m_topLeftBorderRadius.m_value = alternativeValue;
                    currentSelector->m_topRightBorderRadius.m_value = alternativeValue;
                    currentSelector->m_bottomRightBorderRadius.m_value = alternativeValue;
                    currentSelector->m_bottomLeftBorderRadius.m_value = alternativeValue;
                }
            }
        }

    private:

    };
}

#endif //CSSROUNDINGPROPERTYPROCESSOR_H
