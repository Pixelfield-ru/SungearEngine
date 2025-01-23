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

namespace SGCore::UI
{
    struct CSSMathNode;

    struct ANTLRCSSListener : public css3ParserBaseListener
    {
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

        CSSSelector* m_currentSelector { };
    };
}

#endif //SUNGEARENGINE_ANTLRCSSLISTENER_H
