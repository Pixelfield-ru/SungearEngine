//
// Created by stuka on 10.09.2024.
//

#ifndef SUNGEARENGINE_CODEGENERATOR_H
#define SUNGEARENGINE_CODEGENERATOR_H

#include <any>
#include "SGCore/MetaInfo/MetaInfo.h"
#include "Scope.h"

namespace SGCore::CodeGen
{
    struct Generator;

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
        K_START_PLACEMENT,
        K_END_PLACEMENT,
        K_VAR,
        K_CPP_CODE_LINE,
        K_CHAR_SEQ,

        // char-tokens
        K_LPAREN,
        K_RPAREN,
        K_LBLOCK,
        K_RBLOCK,
        K_DOT,
        K_COLON,
        K_QUOTE, // "
        K_NOT, // !

        K_UNKNOWN
    };

    

    struct Generator
    {
        Generator();
        Generator(const Generator&) = default;
        Generator(Generator&&)  noexcept = default;

        [[nodiscard]] std::string generate(const std::filesystem::path& templateFile) noexcept;

    private:
        Lang::Scope m_baseScope;

        enum class CommentType
        {
            NO_COMMENT,
            ONE_LINE,
            MULTILINE
        };

        struct VariableFindResult
        {
            std::shared_ptr<Lang::ASTToken> m_token;
            std::size_t m_tokenIndex = 0;
            // can be not provided
            std::shared_ptr<Lang::Table> m_variable;
            // can be not provided
            std::optional<Lang::Function> m_function;
        };

        std::unordered_map<std::string, Tokens> m_tokensLookup {
                { "##", Tokens::K_STARTEXPR },
                { "{{", Tokens::K_START_PLACEMENT },
                { "}}", Tokens::K_END_PLACEMENT },
                { "for", Tokens::K_FOR },
                { "endfor", Tokens::K_ENDFOR },
                { "in", Tokens::K_IN },
                { "if", Tokens::K_IF },
                { "else", Tokens::K_ELSE },
                { "endif", Tokens::K_ENDIF },
                { "(", Tokens::K_LPAREN },
                { ")", Tokens::K_RPAREN },
                { "{", Tokens::K_LBLOCK },
                { "}", Tokens::K_RBLOCK },
                { ".", Tokens::K_DOT },
                { ":", Tokens::K_COLON },
                { "\"", Tokens::K_QUOTE }
        };

        std::shared_ptr<Lang::ASTToken> m_AST = std::make_shared<Lang::ASTToken>(Tokens::K_FILESTART);
        std::shared_ptr<Lang::ASTToken> m_currentCPPCodeToken = std::make_shared<Lang::ASTToken>(Tokens::K_CPP_CODE_LINE);
        std::shared_ptr<Lang::ASTToken> m_currentCharSeqToken = std::make_shared<Lang::ASTToken>(Tokens::K_CHAR_SEQ);

        void buildAST(const std::string& templateFileText) noexcept;
        void generateCodeUsingAST(const std::shared_ptr<Lang::ASTToken>& token,
                                  std::string& outputString) noexcept;

        void analyzeCurrentWordAndCharForTokens(std::shared_ptr<Lang::ASTToken>& currentToken,
                                                std::string& word,
                                                const std::string& text,
                                                std::size_t& curCharIdx,
                                                bool forceAddAsWord = false) noexcept;

        std::shared_ptr<Lang::ASTToken> addToken(const std::shared_ptr<Lang::ASTToken>& toToken,
                                                 Tokens tokenType) noexcept;

        Lang::Tokens getTokenTypeByName(const std::string& tokenName) const noexcept;

        void gotoParent(std::shared_ptr<Lang::ASTToken>& token) const noexcept;

        [[nodiscard]] static bool isSpace(char c) noexcept;

        [[nodiscard]] static VariableFindResult getLastVariable(const std::shared_ptr<Lang::ASTToken>& from, const size_t& begin) noexcept;

        [[nodiscard]] std::any processFunctionTokensAndCallFunc(const std::shared_ptr<Lang::ASTToken>& inToken,
                                                                const size_t& tokenOffset,
                                                                Lang::Function& callableFunction,
                                                                std::string& outputString) const noexcept;

        void addBuiltinVariables() noexcept;
        std::shared_ptr<Lang::Table> metaToObject(SGCore::MetaInfo::Meta& meta);

        // tmp variables ===================
        bool m_isExprStarted = false;
        bool m_isPlacementStarted = false;
        CommentType m_currentCommentType = CommentType::NO_COMMENT;

        // skipping this lang exprs to copy only c++ code
        bool m_skipCodeCopy = false;

        // can we write char sequence or not (example: "hello world")
        bool m_writeCharSeq = false;

        Lang::Table* m_currentUsedVariable = nullptr;

        // =================================
    };
}

#endif //SUNGEARENGINE_CODEGENERATOR_H
