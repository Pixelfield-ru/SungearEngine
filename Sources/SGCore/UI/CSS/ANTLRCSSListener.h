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

        void enterDimension(css3Parser::DimensionContext* ctx) override;
        void enterCalc(css3Parser::CalcContext* ctx) override;

    private:
        /**
         * Iterates through all nodes of any function that can calculate and creates tree of math nodes.
         * @param currentANTLRNode - Current node in CSS tree.
         * @param currentParentMathNode - Current node in math tree.
         */
        void processCalculation(antlr4::tree::ParseTree* currentANTLRNode,
                                const Ref<CSSMathNode>& currentParentMathNode) noexcept;

        CSSSelector* m_currentSelector { };
    };
}

#endif //SUNGEARENGINE_ANTLRCSSLISTENER_H
