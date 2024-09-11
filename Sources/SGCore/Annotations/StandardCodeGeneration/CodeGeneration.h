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
            K_LINEEND,
            K_FOR,
            K_ENDFOR,
            K_IN,
            K_IF,
            K_ENDIF,

            // char-tokens
            K_LPAREN,
            K_RPAREN,
            K_LBLOCK,
            K_RBLOCK,

            K_UNKNOWN
        };

        struct ASTToken
        {
            Tokens m_type = Tokens::K_EOF;
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
                { "endif", Lang::Tokens::K_ENDIF },
                { "(", Lang::Tokens::K_LPAREN },
                { ")", Lang::Tokens::K_RPAREN },
                { "{", Lang::Tokens::K_LBLOCK },
                { "}", Lang::Tokens::K_RBLOCK }
        };

        std::shared_ptr<Lang::ASTToken> m_AST = std::make_shared<Lang::ASTToken>(Lang::Tokens::K_FILESTART);

        std::shared_ptr<Lang::ASTToken> addToken(const std::shared_ptr<Lang::ASTToken>& toToken,
                                                 Lang::Tokens tokenType) noexcept;

        Lang::Tokens getTokenByName(const std::string& tokenName) const noexcept;
    };
}

#endif //SUNGEARENGINE_CODEGENERATION_H
