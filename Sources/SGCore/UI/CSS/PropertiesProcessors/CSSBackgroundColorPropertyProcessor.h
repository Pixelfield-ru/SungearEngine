//
// Created by stuka on 24.01.2025.
//

#ifndef SUNGEARENGINE_CSSBACKGROUNDCOLORPROPERTYPROCESSOR_H
#define SUNGEARENGINE_CSSBACKGROUNDCOLORPROPERTYPROCESSOR_H

#include "CSSPropertyProcessor.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_BACKGROUND_COLOR>
    {
        static constexpr ColorKeyword property_default_value = ColorKeyword::KW_TRANSPARENT;
        static inline const std::string property_default_value_str = "transparent";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {
            auto* currentSelector = antlrcssListener->m_currentStyle;

            auto* declExpr = propertyDeclContext->expr();

            currentSelector->m_backgroundColor.m_value = property_default_value;

            if(declExpr->term().size() > 1) // invalid count of terms. ignoring property...
            {
                antlrcssListener->printInvalidCountOfTermsInPropertyError(propertyName,
                                                                          property_default_value_str,
                                                                          declExpr->term().size(),
                                                                          1);

                return;
            }

            auto* knownTerm = dynamic_cast<css3Parser::KnownTermContext*>(declExpr->term(0));

            if(knownTerm)
            {
                // setting vector4 as current value in variant
                currentSelector->m_backgroundColor.setWithAlternative({});

                auto& colorTuple = std::get<1>(currentSelector->m_backgroundColor.m_value);

                std::array<Ref<CSSMathNode>*, 4> colorArr = {
                        &std::get<0>(colorTuple),
                        &std::get<1>(colorTuple),
                        &std::get<2>(colorTuple),
                        &std::get<3>(colorTuple)
                };

                if(knownTerm->color()) // we have rgb() or rgba() color
                {
                    for(size_t i = 0; i < knownTerm->color()->color_component().size(); ++i)
                    {
                        auto* currentComponent = knownTerm->color()->color_component(i);

                        if(currentComponent->calcValue()->number())
                        {
                            auto mathNode = MakeRef<CSSMathNumericNode>();
                            mathNode->m_value = std::stof(currentComponent->calcValue()->number()->getText());

                            *colorArr[i] = mathNode;
                        }
                        else if(currentComponent->calcValue()->calc())
                        {
                            auto mathNode = MakeRef<CSSMathNode>();

                            antlrcssListener->processCalculation(currentComponent->calcValue()->calc(),
                                                                 propertyName,
                                                                 mathNode,
                                                                 {});

                            *colorArr[i] = mathNode;
                        }
                        else if(currentComponent->calcValue()->percentage())
                        {
                            auto mathNode = MakeRef<CSSMathNumericNode>();
                            mathNode->m_value = std::stof(currentComponent->calcValue()->percentage()->getText());
                            mathNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PERCENT;

                            *colorArr[i] = mathNode;
                        }
                        else
                        {
                            antlrcssListener->printUnsupportedQualifierInCurrentContextError(propertyName,
                                                                                             currentComponent->getText(),
                                                                                             {});
                        }
                    }

                    // processing alpha component ==============================================================
                    auto* alphaComponent = knownTerm->color()->color_alpha();

                    if(alphaComponent->calcValue()->number())
                    {
                        auto mathNode = MakeRef<CSSMathNumericNode>();
                        mathNode->m_value = std::stof(alphaComponent->calcValue()->number()->getText());

                        *colorArr[3] = mathNode;
                    }
                    else if(alphaComponent->calcValue()->calc())
                    {
                        auto mathNode = MakeRef<CSSMathNode>();

                        antlrcssListener->processCalculation(alphaComponent->calcValue()->calc(),
                                                             propertyName,
                                                             mathNode,
                                                             {});

                        *colorArr[3] = mathNode;
                    }
                    else if(alphaComponent->calcValue()->percentage())
                    {
                        auto mathNode = MakeRef<CSSMathNumericNode>();
                        mathNode->m_value = std::stof(alphaComponent->calcValue()->percentage()->getText());
                        mathNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PERCENT;

                        *colorArr[3] = mathNode;
                    }
                    else
                    {
                        antlrcssListener->printUnsupportedQualifierInCurrentContextError(propertyName,
                                                                                         alphaComponent->getText(),
                                                                                         {});
                    }
                }
                else if(knownTerm->hexcolor()) // we have hex color
                {
                    const std::string hexColorStr = knownTerm->hexcolor()->getText();

                    if(hexColorStr.empty() ||
                       (hexColorStr.size() != 7 && hexColorStr.size() != 9) || hexColorStr[0] != '#') // invalid hex
                    {
                        antlrcssListener->printInvalidHexError(propertyName, hexColorStr, property_default_value_str);

                        return;
                    }

                    // else parsing our hex ======================

                    // initializing nodes
                    for(std::int8_t i = 0; i < 4; ++i)
                    {
                        auto mathNode = MakeRef<CSSMathNumericNode>();
                        *colorArr[i] = mathNode;

                        // if we are on alpha component and hex does not have alpha component
                        if(i == 3 && hexColorStr.size() != 9)
                        {
                            mathNode->m_value = 255.0f;
                        }
                        else // in other cases just parsing hex
                        {
                            mathNode->m_value = std::stoi(hexColorStr.substr(i * 2 + 1, 2), nullptr, 16); // r, g, b
                        }
                    }
                }
                else // we have keyword
                {
                    const ColorKeyword colorKeyword = getColorKeywordFromStringValue(knownTerm->getText());

                    if(colorKeyword == ColorKeyword::KW_UNKNOWN)
                    {
                        antlrcssListener->printUnknownKeywordUsedError(propertyName,
                                                                       knownTerm->getText(),
                                                                       property_default_value_str);

                        return;
                    }

                    currentSelector->m_backgroundColor.m_value = colorKeyword;
                }
            }
        }
    };
}

#endif //SUNGEARENGINE_CSSBACKGROUNDCOLORPROPERTYPROCESSOR_H
