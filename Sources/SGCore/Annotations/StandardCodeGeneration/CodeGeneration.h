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
            K_START_PLACEMENT,
            K_END_PLACEMENT,
            K_VAR,
            K_CPP_CODE_LINE,

            // char-tokens
            K_LPAREN,
            K_RPAREN,
            K_LBLOCK,
            K_RBLOCK,
            K_DOT,

            K_UNKNOWN
        };

        struct Type;
        struct Function;

        struct Type
        {
            friend struct Generator;

            std::string m_name;
            std::vector<Type> m_extends;

            [[nodiscard]] bool instanceof(const Type& other) const noexcept;
            [[nodiscard]] bool instanceof(const std::string& typeName) const noexcept;

            [[nodiscard]] Type* tryGetMember(const std::string& name) noexcept;
            [[nodiscard]] Function* tryGetFunction(const std::string& name) noexcept;

        private:
            // first - name, second - type
            std::unordered_map<std::string, Type> m_members;
            // first - name, second - function
            std::unordered_map<std::string, Function> m_functions;
        };

        struct FunctionArgument
        {
            std::string m_name;
            bool m_isNecessary = true;
            Type m_acceptableType;
        };

        struct Function
        {
            std::string m_name;
            Type m_returnType;
            std::vector<FunctionArgument> m_arguments;

            std::function<void()> m_functor;
        };

        struct ASTToken
        {
            Tokens m_type = Tokens::K_EOF;
            // optional
            std::string m_name;
            // optional
            std::string m_cppCode;
            bool m_isExprToken = false;
            std::weak_ptr<Lang::ASTToken> m_parent;
            std::vector<std::shared_ptr<Lang::ASTToken>> m_children;
        };
    }

    struct Generator
    {
        [[nodiscard]] std::string generate(AnnotationsProcessor& annotationsProcessor,
                                           const std::filesystem::path& templateFile) noexcept;

    private:
        /*std::vector<Lang::Type> m_currentTypes {
                {
                    .m_name = "vector",
                    .m_extends = {
                            {
                                    .m_name = "iterable"
                            }
                    }
                },
                {
                    .m_name = "bool"
                },
                {
                    .m_name = "uint64"
                }
        };*/

        std::unordered_map<std::string, Lang::Tokens> m_tokensLookup {
                { "##", Lang::Tokens::K_STARTEXPR },
                { "{{", Lang::Tokens::K_START_PLACEMENT },
                { "}}", Lang::Tokens::K_END_PLACEMENT },
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
        std::shared_ptr<Lang::ASTToken> m_currentCPPCodeToken = std::make_shared<Lang::ASTToken>(Lang::Tokens::K_CPP_CODE_LINE);

        void buildAST(const std::string& templateFileText) noexcept;
        void generateCodeUsingAST(const std::shared_ptr<Lang::ASTToken>& token,
                                  std::string& outputString) noexcept;

        void analyzeCurrentWordAndCharForTokens(std::shared_ptr<Lang::ASTToken>& currentToken,
                                                std::string& word,
                                                const std::string& text,
                                                std::size_t& curCharIdx,
                                                bool forceAddAsWord = false) noexcept;

        std::shared_ptr<Lang::ASTToken> addToken(const std::shared_ptr<Lang::ASTToken>& toToken,
                                                 Lang::Tokens tokenType) noexcept;

        Lang::Tokens getTokenTypeByName(const std::string& tokenName) const noexcept;

        void gotoParent(std::shared_ptr<Lang::ASTToken>& token) const noexcept;

        [[nodiscard]] bool isSpace(char c) noexcept;

        // tmp variables
        bool m_isExprStarted = false;
        bool m_isPlacementStarted = false;

        // skipping this lang exprs to copy only c++ code
        bool m_skipCodeCopy = false;
    };
}

#endif //SUNGEARENGINE_CODEGENERATION_H
