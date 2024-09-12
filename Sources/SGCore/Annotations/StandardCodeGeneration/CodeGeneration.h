//
// Created by stuka on 10.09.2024.
//

#ifndef SUNGEARENGINE_CODEGENERATION_H
#define SUNGEARENGINE_CODEGENERATION_H

#include <SGCore/Annotations/AnnotationsProcessor.h>

namespace SGCore::CodeGen
{
    namespace Lang
    {
        enum class Tokens
        {
            K_FILESTART,
            K_EOF,
            K_STARTEXPR,
            K_ENDEXPR,
            K_FOR,
            K_ENDFOR,
            K_IN,
            K_IF,
            K_ELSE,
            K_ENDIF,
            K_VARIABLE_PLACE_START,
            K_VARIABLE_PLACE_END,
            K_VAR,

            // char-tokens
            K_LPAREN,
            K_RPAREN,
            K_LBLOCK,
            K_RBLOCK,
            K_DOT,

            K_UNKNOWN
        };

        struct ASTToken
        {
            Tokens m_type = Tokens::K_EOF;
            // optional
            std::string m_name;
            std::shared_ptr<Lang::ASTToken> m_parent;
            std::vector<std::shared_ptr<Lang::ASTToken>> m_children;
        };
    }

    struct Generator
    {
        void generate(AnnotationsProcessor& annotationsProcessor,
                      const std::filesystem::path& templateFile,
                      const std::filesystem::path& outputFile) noexcept;

    private:
        std::unordered_map<std::string, Lang::Tokens> m_tokensLookup {
                { "##", Lang::Tokens::K_STARTEXPR },
                { "for", Lang::Tokens::K_FOR },
                { "endfor", Lang::Tokens::K_ENDFOR },
                { "in", Lang::Tokens::K_IN },
                { "if", Lang::Tokens::K_IF },
                { "else", Lang::Tokens::K_ELSE },
                { "endif", Lang::Tokens::K_ENDIF },
                { "(", Lang::Tokens::K_LPAREN },
                { ")", Lang::Tokens::K_RPAREN },
                { "{", Lang::Tokens::K_LBLOCK },
                { "}", Lang::Tokens::K_RBLOCK },
                { ".", Lang::Tokens::K_DOT }
        };

        std::shared_ptr<Lang::ASTToken> m_AST = std::make_shared<Lang::ASTToken>(Lang::Tokens::K_FILESTART);

        void analyzeCurrentWordAndCharForTokens(std::shared_ptr<Lang::ASTToken>& currentToken,
                                                std::string& word, char curChar) noexcept;

        std::shared_ptr<Lang::ASTToken> addToken(const std::shared_ptr<Lang::ASTToken>& toToken,
                                                 Lang::Tokens tokenType) noexcept;

        Lang::Tokens getTokenByName(const std::string& tokenName) const noexcept;

        void gotoParent(std::shared_ptr<Lang::ASTToken>& token) const noexcept;

        [[nodiscard]] bool isSpace(char c) noexcept;

        // tmp variables
        bool m_isExprStarted = false;
    };
}

#endif //SUNGEARENGINE_CODEGENERATION_H
