//
// Created by stuka on 18.01.2025.
//

#ifndef SUNGEARENGINE_ANTLRCSSLISTENER_H
#define SUNGEARENGINE_ANTLRCSSLISTENER_H

#include <antlr4-runtime.h>
#include "SGCore/UI/ANTLR4CSS3Generated/css3Lexer.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3Parser.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3ParserBaseListener.h"

#include "CSSFile.h"
#include "SGCore/Memory/AssetRef.h"

#include "SGCore/UI/CSS/Math/CSSMathNode.h"
#include "SGCore/UI/CSS/Math/CSSMathNumericNode.h"

namespace SGCore::UI
{
    struct CSSMathNode;

    struct ANTLRCSSListener : public css3ParserBaseListener
    {
        template<CSSPropertyType>
        friend struct CSSPropertyProcessor;

        CSSFile* m_toCSSFile { };

        void enterSelector(css3Parser::SelectorContext* ctx) override;

        void enterKnownDeclaration(css3Parser::KnownDeclarationContext* ctx) override;

    private:
        /**
         * Iterates through all nodes of any function that can calculate and creates tree of math nodes.
         * @param currentANTLRNode - Current node in CSS tree.
         * @param currentParentMathNode - Current node in math tree.
         */
        void processCalculation(antlr4::tree::ParseTree* currentANTLRNode,
                                const Ref<CSSMathNode>& currentParentMathNode) noexcept;

        void printInvalidCountOfTermsInPropertyError(const std::string& propertyName,
                                                     const std::string& defaultSetKeyword,
                                                     const size_t& currentTermsCount,
                                                     const std::int64_t& validTermsMinCount,
                                                     const std::int64_t& validTermsMaxCount = -1) const noexcept;

        void printBadTermInPropertyError(const std::string& propertyName,
                                         const std::int64_t& termIndex,
                                         const std::string& termValue,
                                         const std::string& defaultSetKeyword) const noexcept;

        void printUnknownKeywordUsedError(const std::string& propertyName,
                                          const std::string& currentKeyword,
                                          const std::string& defaultSetKeyword) const noexcept;

        static CSSDimensionQualifier getDimensionFromString(const std::string& dimensionStr) noexcept;

        template<auto DefaultKeyword>
        requires(std::is_enum_v<decltype(DefaultKeyword)>)
        std::variant<decltype(DefaultKeyword), Ref<CSSMathNode>>
        getCalculablePropertyTermValue(const std::string& propertyName,
                                       const std::string& propertyValueStr,
                                       const std::string& propertyDefaultValueStr,
                                       css3Parser::TermContext* term) noexcept
        {
            using keyword_enum_t = decltype(DefaultKeyword);

            std::variant<keyword_enum_t, Ref<CSSMathNode>> output;

            output = DefaultKeyword;

            auto* knownTerm = dynamic_cast<css3Parser::KnownTermContext*>(term);

            if(knownTerm)
            {
                if(knownTerm->calc()) // width: calc(...)
                {
                    auto mathNode = MakeRef<CSSMathNode>();

                    processCalculation(knownTerm->calc(), mathNode);
                    mathNode->resolvePriorities();

                    std::cout << "calc: " << mathNode->calculate() << std::endl;

                    output = mathNode;
                }
                else if(knownTerm->number())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();
                    mathNode->m_value = std::stof(knownTerm->number()->getText());

                    std::cout << "calc: " << mathNode->calculate() << std::endl;

                    output = mathNode;
                }
                else if(knownTerm->dimension())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();

                    const std::string dimension = knownTerm->dimension()->Dimension()->getText();

                    mathNode->m_dimensionQualifier = getDimensionFromString(dimension);
                    mathNode->m_value = std::stof(dimension);

                    std::cout << "calc: " << mathNode->calculate() << std::endl;

                    output = mathNode;
                }
                else if(knownTerm->percentage())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();

                    const std::string percentage = knownTerm->percentage()->Percentage()->getText();

                    mathNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PERCENT;
                    mathNode->m_value = std::stof(percentage);

                    std::cout << "calc: " << mathNode->calculate() << std::endl;

                    output = mathNode;
                }
                else // we have keyword
                {
                    const keyword_enum_t keyword = getKeywordFromStringValue<keyword_enum_t>(propertyValueStr);

                    if(keyword == keyword_enum_t::KW_UNKNOWN)
                    {
                        printUnknownKeywordUsedError(propertyName, propertyValueStr, propertyDefaultValueStr);

                        return output;
                    }

                    output = keyword;
                }
            }
            else // we have bad term. ignoring property...
            {
                printBadTermInPropertyError(propertyName, 0, term->getText(), propertyDefaultValueStr);
            }

            return output;
        }

        CSSSelector* m_currentSelector { };
    };
}

#endif //SUNGEARENGINE_ANTLRCSSLISTENER_H
