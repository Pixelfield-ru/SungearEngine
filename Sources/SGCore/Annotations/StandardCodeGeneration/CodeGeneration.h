//
// Created by stuka on 10.09.2024.
//

#ifndef SUNGEARENGINE_CODEGENERATION_H
#define SUNGEARENGINE_CODEGENERATION_H

namespace SGCore::CodeGen
{
    struct Generator;

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
            K_CHAR_SEQ,

            // char-tokens
            K_LPAREN,
            K_RPAREN,
            K_LBLOCK,
            K_RBLOCK,
            K_DOT,
            K_COLON,

            K_UNKNOWN
        };

        struct Variable;
        struct Function;

        struct Type
        {
            std::string m_name;
            std::vector<Type> m_extends;
            // first - name, second - type
            std::unordered_map<std::string, Type> m_members;
            // first - name, second - function
            std::unordered_map<std::string, Function> m_functions;

            [[nodiscard]] bool instanceof(const Type& other) const noexcept;
            [[nodiscard]] bool instanceof(const std::string& typeName) const noexcept;
        };

        struct Variable
        {
            // friend struct Generator;

            Variable() = default;
            Variable(const Variable&) = default;
            Variable(Variable&&) noexcept = default;

            explicit Variable(const Type& withType) noexcept;

            std::string m_name;
            // value that will be inserted in result document
            std::string m_insertedValue;

            [[nodiscard]] std::shared_ptr<Variable> getMember(const std::string& name) noexcept;
            [[nodiscard]] Function* tryGetFunction(const std::string& name) noexcept;

            [[nodiscard]] const Type& getTypeInfo() const noexcept;

            [[nodiscard]] bool hasMember(const std::string& memberName) const noexcept;

            [[nodiscard]] size_t membersCount() const noexcept;

            [[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<Variable>>& getMembers() const noexcept;

            Variable& operator=(const Variable&) noexcept = default;
            Variable& operator=(Variable&&) noexcept = default;

            Variable& operator[](const std::string& memberName) noexcept;

            void setMemberPtr(const std::string& memberName,
                              const std::shared_ptr<Variable>& memberValue) noexcept;

        private:
            Type m_typeInfo;
            std::unordered_map<std::string, std::shared_ptr<Variable>> m_inheritanceMap;

            // first - name, second - type
            std::unordered_map<std::string, std::shared_ptr<Variable>> m_members;
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
            std::unordered_map<std::string, std::shared_ptr<Variable>> m_scope;
            bool m_isExprToken = false;
            std::weak_ptr<Lang::ASTToken> m_parent;
            std::vector<std::shared_ptr<Lang::ASTToken>> m_children;

            [[nodiscard]] std::shared_ptr<Variable> getScopeVariable(const std::string& variableName) const noexcept;
        };
    }

    struct Generator
    {
        Generator();
        Generator(const Generator&) = default;
        Generator(Generator&&)  noexcept = default;

        [[nodiscard]] std::string generate(const std::filesystem::path& templateFile) noexcept;

    private:
        enum class CommentType
        {
            NO_COMMENT,
            ONE_LINE,
            MULTILINE
        };

        using token_and_var = std::pair<std::shared_ptr<Lang::ASTToken>, std::shared_ptr<Lang::Variable>>;

        std::vector<Lang::Type> m_currentTypes;

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
                { ".", Lang::Tokens::K_DOT },
                { ":", Lang::Tokens::K_COLON }
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

        [[nodiscard]] std::optional<Lang::Type> getTypeByName(const std::string& typeName) const noexcept;

        void gotoParent(std::shared_ptr<Lang::ASTToken>& token) const noexcept;

        [[nodiscard]] static bool isSpace(char c) noexcept;

        [[nodiscard]] static token_and_var getLastVariable(const std::shared_ptr<Lang::ASTToken>& from, const size_t& begin) noexcept;

        // tmp variables ===================
        bool m_isExprStarted = false;
        bool m_isPlacementStarted = false;
        CommentType m_currentCommentType = CommentType::NO_COMMENT;

        // skipping this lang exprs to copy only c++ code
        bool m_skipCodeCopy = false;

        Lang::Variable* m_currentUsedVariable = nullptr;

        // =================================
    };
}

#endif //SUNGEARENGINE_CODEGENERATION_H
