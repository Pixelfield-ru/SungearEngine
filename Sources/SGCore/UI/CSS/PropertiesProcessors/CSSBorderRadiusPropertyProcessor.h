//
// Created by stuka on 22.02.2025.
//

#ifndef CSSROUNDINGPROPERTYPROCESSOR_H
#define CSSROUNDINGPROPERTYPROCESSOR_H

#include "CSSPropertyProcessorCommon.h"
#include "SGCore/UI/CSS/CSSPropertyType.h"

#include "SGCore/Utils/TypeTraits.h"
#include "SGCore/UI/CSS/ANTLRCSSListener.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_BORDER_RADIUS>
    {
    private:
        using term_value_t = std::variant<UniversalKeyword, Ref<CSSMathNode>>;

    public:

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

                if(!knownTerm)
                {
                    antlrcssListener->printBadTermInPropertyError(propertyName, 0, declExpr->term(0)->getText(),
                                                                  property_default_value_str);
                    return;
                }

                auto termValue = CSSPropertyProcessorCommon::processKnownTerm<UniversalKeyword>(antlrcssListener, currentSelector, knownTerm, 0, propertyName);

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
                else
                {
                    // todo: keyword support
                }
            }
            else if(declExpr->term().size() >= 2 && declExpr->operator_().size() == 0)
            {
                std::vector<term_value_t> termsValues;

                for(size_t i = 0; i < declExpr->term().size(); ++i)
                {
                    auto* knownTerm = dynamic_cast<css3Parser::KnownTermContext*>(declExpr->term(i));

                    if(!knownTerm)
                    {
                        antlrcssListener->printBadTermInPropertyError(propertyName, i, declExpr->term(i)->getText(),
                                                                      property_default_value_str);
                        return;
                    }

                    auto termValue = CSSPropertyProcessorCommon::processKnownTerm<UniversalKeyword>(antlrcssListener, currentSelector, knownTerm, i, propertyName);

                    if(std::holds_alternative<UniversalKeyword>(termValue))
                    {
                        antlrcssListener->printBadTermInPropertyError(propertyName, i, knownTerm->getText(),
                                                                      property_default_value_str);
                        return;
                    }

                    termsValues.push_back(termValue);
                }

                expandBordersRadiusVector(termsValues);

                const auto finalValue0 = BorderRadiusAlternativeValue {
                    .m_radiusX = std::get<1>(termsValues[0]),
                    .m_radiusY = std::get<1>(termsValues[0])
                };

                const auto finalValue1 = BorderRadiusAlternativeValue {
                    .m_radiusX = std::get<1>(termsValues[1]),
                    .m_radiusY = std::get<1>(termsValues[1])
                };

                const auto finalValue2 = BorderRadiusAlternativeValue {
                    .m_radiusX = std::get<1>(termsValues[2]),
                    .m_radiusY = std::get<1>(termsValues[2])
                };

                const auto finalValue3 = BorderRadiusAlternativeValue {
                    .m_radiusX = std::get<1>(termsValues[3]),
                    .m_radiusY = std::get<1>(termsValues[3])
                };

                currentSelector->m_topLeftBorderRadius.m_value = finalValue0;
                currentSelector->m_topRightBorderRadius.m_value = finalValue1;
                currentSelector->m_bottomRightBorderRadius.m_value = finalValue2;
                currentSelector->m_bottomLeftBorderRadius.m_value = finalValue3;
            }
            else if(declExpr->term().size() >= 2) // if we have operator '/'
            {
                if(declExpr->operator_().size() > 1)
                {
                    antlrcssListener->printInvalidCountOfOperatorsInPropertyError(
                        propertyName, property_default_value_str, declExpr->operator_().size(), 1);
                    return;
                }

                if(!declExpr->operator_(0)->getText().contains("/"))
                {
                    antlrcssListener->printBadOperatorInPropertyError(propertyName, 0, declExpr->operator_(0)->getText(), property_default_value_str);
                    return;
                }

                // terms before '/'
                std::vector<term_value_t> leftTermsValues;
                // terms after term_value_t
                std::vector<term_value_t> rightTermsValues;

                const size_t operatorTokenIndex = declExpr->operator_(0)->getStart()->getTokenIndex();

                // sorting terms to left and right
                for(size_t i = 0; i < declExpr->term().size(); ++i)
                {
                    auto* knownTerm = dynamic_cast<css3Parser::KnownTermContext*>(declExpr->term(i));

                    if(!knownTerm)
                    {
                        antlrcssListener->printBadTermInPropertyError(propertyName, i, declExpr->term(i)->getText(), property_default_value_str);
                        return;
                    }

                    const auto termValue = CSSPropertyProcessorCommon::processKnownTerm<UniversalKeyword>(antlrcssListener, currentSelector, knownTerm, i, propertyName);

                    // do not allow some keywords in this context
                    if(std::holds_alternative<UniversalKeyword>(termValue))
                    {
                        antlrcssListener->printBadTermInPropertyError(propertyName, i, knownTerm->getText(),
                                                                      property_default_value_str);
                        return;
                    }

                    if(knownTerm->getStart()->getTokenIndex() < operatorTokenIndex)
                    {
                        leftTermsValues.push_back(termValue);
                    }
                    else
                    {
                        rightTermsValues.push_back(termValue);
                    }
                }

                // if count of terms is invalid
                if(leftTermsValues.size() == 0 || leftTermsValues.size() > 4)
                {
                    LOG_E(SGCORE_TAG,
                          "ANTLRCSSListener can not process property '{}' correctly: property has invalid count of terms in section 'value'. "
                          "Property has been set to the default value. "
                          "Number of terms to the left of the operator '/': {}. "
                          "Valid count of terms on left: 4. "
                          "Set keyword (default): '{}'.\n"
                          "In CSS file: {}",
                          propertyName,
                          leftTermsValues.size(),
                          property_default_value_str,
                          Utils::toUTF8(antlrcssListener->m_toCSSFile->getPath().resolved().u16string()));

                    return;
                }

                // if count of terms is invalid
                if(rightTermsValues.size() == 0 || rightTermsValues.size() > 4)
                {
                    LOG_E(SGCORE_TAG,
                          "ANTLRCSSListener can not process property '{}' correctly: property has invalid count of terms in section 'value'. "
                          "Property has been set to the default value. "
                          "Number of terms to the right of the operator '/': {}. "
                          "Valid count of terms on right: 4. "
                          "Set keyword (default): '{}'.\n"
                          "In CSS file: {}",
                          propertyName,
                          rightTermsValues.size(),
                          property_default_value_str,
                          Utils::toUTF8(antlrcssListener->m_toCSSFile->getPath().resolved().u16string()));

                    return;
                }

                // here all terms values contains math nodes =================================

                expandBordersRadiusVector(leftTermsValues);
                expandBordersRadiusVector(rightTermsValues);

                const auto finalValue0 = BorderRadiusAlternativeValue {
                    .m_radiusX = std::get<1>(leftTermsValues[0]),
                    .m_radiusY = std::get<1>(rightTermsValues[0])
                };

                const auto finalValue1 = BorderRadiusAlternativeValue {
                    .m_radiusX = std::get<1>(leftTermsValues[1]),
                    .m_radiusY = std::get<1>(rightTermsValues[1])
                };

                const auto finalValue2 = BorderRadiusAlternativeValue {
                    .m_radiusX = std::get<1>(leftTermsValues[2]),
                    .m_radiusY = std::get<1>(rightTermsValues[2])
                };

                const auto finalValue3 = BorderRadiusAlternativeValue {
                    .m_radiusX = std::get<1>(leftTermsValues[3]),
                    .m_radiusY = std::get<1>(rightTermsValues[3])
                };

                currentSelector->m_topLeftBorderRadius.m_value = finalValue0;
                currentSelector->m_topRightBorderRadius.m_value = finalValue1;
                currentSelector->m_bottomRightBorderRadius.m_value = finalValue2;
                currentSelector->m_bottomLeftBorderRadius.m_value = finalValue3;
            }
        }

    private:
        static void expandBordersRadiusVector(std::vector<term_value_t>& outputVec) noexcept
        {
            switch(outputVec.size())
            {
                case 1:
                {
                    outputVec.push_back(outputVec[0]);
                    outputVec.push_back(outputVec[0]);
                    outputVec.push_back(outputVec[0]);
                    return;
                }
                case 2:
                {
                    outputVec.push_back(outputVec[0]);
                    outputVec.push_back(outputVec[1]);
                    return;
                }
                case 3:
                {
                    outputVec.push_back(outputVec[1]);
                    return;
                }
                default: return;
            }
        }
    };
}

#endif //CSSROUNDINGPROPERTYPROCESSOR_H
