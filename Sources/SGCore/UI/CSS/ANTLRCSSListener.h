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
        friend struct CSSPropertyProcessorCommon;

        CSSFile* m_toCSSFile { };

        void enterSelector(css3Parser::SelectorContext* ctx) override;

        void enterKnownDeclaration(css3Parser::KnownDeclarationContext* ctx) override;

        void resolvePseudos() noexcept;

    private:
        /**
         * Iterates through all nodes of any function that can calculate and creates tree of math nodes.
         * @param currentANTLRNode Current node in CSS tree.
         * @param currentPropertyName Current property.
         * @param currentParentMathNode Current node in math tree.
         * @param supportedQualifiers Set of supported qualifiers in calc() expression. If contains DQ_ANY, it is assumed that calc() expr supports all qualifiers in current context.
         */
        void processCalculation(antlr4::tree::ParseTree* currentANTLRNode,
                                const std::string& currentPropertyName,
                                const Ref<CSSMathNode>& currentParentMathNode,
                                const std::unordered_set<CSSDimensionQualifier>& supportedQualifiers) noexcept;

        void printInvalidCountOfTermsInPropertyError(const std::string& propertyName,
                                                     const std::string& defaultSetKeyword,
                                                     const size_t& currentTermsCount,
                                                     const std::int64_t& validTermsMinCount,
                                                     const std::int64_t& validTermsMaxCount = -1) const noexcept;

        void printInvalidCountOfOperatorsInPropertyError(const std::string& propertyName,
                                                         const std::string& defaultSetKeyword,
                                                         const size_t& currentOperatorsCount,
                                                         const std::int64_t& validOperatorsMinCount,
                                                         const std::int64_t& validOperatorsMaxCount = -1) const noexcept;

        void printBadTermInPropertyError(const std::string& propertyName,
                                         const std::int64_t& termIndex,
                                         const std::string& termValue,
                                         const std::string& defaultSetKeyword) const noexcept;

        void printBadOperatorInPropertyError(const std::string& propertyName,
                                             const std::int64_t& operatorIndex,
                                             const std::string& operatorValue,
                                             const std::string& defaultSetKeyword) const noexcept;

        void printUnknownKeywordUsedError(const std::string& propertyName,
                                          const std::string& currentKeyword,
                                          const std::string& defaultSetKeyword) const noexcept;

        void printUnsupportedQualifierInCurrentContextError(const std::string& propertyName,
                                                            const std::string& currentDimensionString,
                                                            const std::unordered_set<CSSDimensionQualifier>& supportedQualifiers) const noexcept;

        void printInvalidHexError(const std::string& propertyName,
                                  const std::string& currentHex,
                                  const std::string& defaultSetKeyword) const noexcept;

        CSSStyle* m_currentStyle { };
        CSSStyle* m_currentPseudoClassStyle { };
        std::vector<std::pair<CSSStyle*, css3Parser::KnownDeclarationContext*>> m_pseudosToResolve;
    };
}

#endif //SUNGEARENGINE_ANTLRCSSLISTENER_H
