//
// Created by stuka on 02.03.2025.
//

#ifndef CSSPROPERTYPROCESSORCOMMON_H
#define CSSPROPERTYPROCESSORCOMMON_H

#include "SGCore/UI/CSS/ANTLRCSSListener.h"

namespace SGCore::UI
{
    struct CSSPropertyProcessorCommon
    {
        template<typename KeywordT>
        static std::variant<KeywordT, Ref<CSSMathNode>> processKnownTerm(
            ANTLRCSSListener* antlrcssListener,
            CSSSelector* currentSelector,
            css3Parser::KnownTermContext* knownTerm,
            const size_t& termIndex,
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

            return getKeywordFromStringValue<KeywordT>(knownTerm->getText());
        }
    };
}

#endif //CSSPROPERTYPROCESSORCOMMON_H
