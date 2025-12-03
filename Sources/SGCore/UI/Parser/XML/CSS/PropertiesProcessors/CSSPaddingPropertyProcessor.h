//
// Created by stuka on 03.03.2025.
//

#ifndef CSSPADDINGPROPERTYPROCESSOR_H
#define CSSPADDINGPROPERTYPROCESSOR_H

#include "CSSPropertyProcessorCommon.h"
#include "SGCore/UI/Parser/XML/CSS/ANTLRCSSListener.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_PADDING>
    {
    private:
        using term_value_t = std::variant<UniversalKeyword, Ref<DynValueNode>>;

    public:

        static constexpr UniversalKeyword property_default_value = UniversalKeyword::KW_UNSET;
        static inline const std::string property_default_value_str = "unset";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {
            auto* currentSelector = antlrcssListener->m_currentStyle;

            auto* declExpr = propertyDeclContext->expr();

            currentSelector->m_padding.m_value = property_default_value;

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

                if(std::holds_alternative<Ref<DynValueNode>>(termValue))
                {
                    auto mathNode = std::get<1>(termValue);

                    currentSelector->m_padding.setWithAlternative({});

                    currentSelector->m_padding.getFromAlternativeValue<0>() = mathNode;
                    currentSelector->m_padding.getFromAlternativeValue<1>() = mathNode;
                    currentSelector->m_padding.getFromAlternativeValue<2>() = mathNode;
                    currentSelector->m_padding.getFromAlternativeValue<3>() = mathNode;
                }
                else
                {
                    // todo: keyword support
                }
            }
            else if(declExpr->term().size() >= 2)
            {
                std::vector<term_value_t> termsValues;

                // processing terms values
                for(size_t i = 0; i < declExpr->term().size(); ++i)
                {
                    auto* knownTerm = dynamic_cast<css3Parser::KnownTermContext*>(declExpr->term(i));

                    if(!knownTerm)
                    {
                        antlrcssListener->printBadTermInPropertyError(propertyName, i, declExpr->term(i)->getText(), property_default_value_str);
                        return;
                    }

                    const auto termValue = CSSPropertyProcessorCommon::processKnownTerm<UniversalKeyword>(
                        antlrcssListener, currentSelector, knownTerm, i, propertyName);

                    // do not allow some keywords in this context
                    if(std::holds_alternative<UniversalKeyword>(termValue))
                    {
                        antlrcssListener->printBadTermInPropertyError(propertyName, i, knownTerm->getText(),
                                                                      property_default_value_str);
                        return;
                    }

                    termsValues.push_back(termValue);
                }

                // here all terms values contains math nodes =================================

                expandPaddingsVector(termsValues.size(), termsValues);

                currentSelector->m_padding.setWithAlternative({});

                currentSelector->m_padding.getFromAlternativeValue<0>() = std::get<1>(termsValues[0]);
                currentSelector->m_padding.getFromAlternativeValue<1>() = std::get<1>(termsValues[1]);
                currentSelector->m_padding.getFromAlternativeValue<2>() = std::get<1>(termsValues[2]);
                currentSelector->m_padding.getFromAlternativeValue<3>() = std::get<1>(termsValues[3]);
            }
        }

    private:
        static void expandPaddingsVector(std::int8_t originalSize, std::vector<term_value_t>& outputVec) noexcept
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

#endif //CSSPADDINGPROPERTYPROCESSOR_H
