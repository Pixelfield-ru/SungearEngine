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
    //struct ASTToken
    //{
    //    Tokens m_type = Tokens::K_EOF;
    //    // optional
    //    std::string m_name;
    //    // optional
    //    std::string m_cppCode;
    //    bool m_isExprToken = false;
    //    std::weak_ptr<Lang::ASTToken> m_parent;
    //    std::vector<std::shared_ptr<Lang::ASTToken>> m_children;

    //    [[nodiscard]] std::shared_ptr<Table> getScopeVariable(const std::string& variableName) const noexcept;
    //};
    struct Generator
    {
        Generator();
        Generator(const Generator&) = default;
        Generator(Generator&&)  noexcept = default;

        [[nodiscard]] std::string generate(const std::filesystem::path& templateFile) noexcept;

    private:
        

        Lang::Scope m_currentScope;

        struct SourceCodeHandler {
            std::string source;
        };

        /// <summary>
        /// Reads sources. Also contains a stack, that stores cursor position
        /// </summary>
        struct SourceCodeReader {
            SourceCodeReader(SourceCodeHandler& handler) : source(handler) {};
            SourceCodeHandler& source;

            int cursor;

            std::stack<int> stack;

            void push();
            void pop();
            /**
             * @brief Removes top stack value without changing cursor
             */
            void apply();

            // char current() noexcept;

            /**
             * @brief returns next character without moving a cursor
             * @return next character
             */
            char lnext() { return lnext("not EOF"); };
            /**
             * @brief returns next character without moving a cursor
             * @param excpected - excpected value (for exception)
             * @return next character
             */
            char lnext(std::string excpected);
            /**
             * @brief returns next character and moves a cursor by 1
             * @return next character
             */
            char next() { return next("not EOF"); };
            /**
             * @brief returns next character and moves a cursor by 1
             * @param excpected - excpected value (for exception)
             * @return next character
             */
            char next(std::string excpected);

            /**
             * @brief return next @p count symbols and moves cursor by a same amount
             * @param count - substr lenght
             * @return @p count symbols
             */
            std::string next(int count) { return next(count, "not EOF"); };
            /**
             * @brief return return next @p count symbols and moves cursor by a same amount
             * @param count - substr lenght
             * @param excpected - excpected value (for exception)
             * @return 
             */
            std::string next(int count, std::string excpected);


            std::string lnext(int count) { return lnext(count, "not EOF"); };
            std::string lnext(int count, std::string excpected);

            bool hasNext();
            bool hasNext(int count);
        };

        struct ParseException : std::exception {
            ParseException(std::string message) : m_message(message) {};
            std::string m_message;
        };
        struct RuntimeException : std::exception {
            RuntimeException(std::string message) : m_message(message) {};
            std::string m_message;
        };

        
        SourceCodeReader m_reader;

        Lang::Scope m_baseScope;

        // Parse stop reason, code generator
        std::pair<std::string, std::function<std::string()>> parseAny(); // <parseLine>*
        std::function<std::string()> parseLine(); // <skipSpaces>(<##><parseCode>|<parseRawString>)

        std::function<std::string()> parseCode(); // <tryParseForLoop>

        std::optional<std::function<std::string()>> tryParseForLoop(); // <for> <parseNewVar> <in> <tryParseExpression> <:> !
        std::optional<std::function<std::string()>> tryParseIf(); // <if> <tryParseExpression> <:> !

        std::optional<std::function<Lang::Any()>> tryParseVarWithPath(); // <tryParseInlinedExpression>(.<parseVarToken>)* 
        std::optional<std::function<Lang::Any()>> tryParseInlinedExpression(); // <(><tryParseExpression><)> 
        std::optional<std::function<Lang::Any()>> tryParseExpression(); // <tryParseVarWithPath>|<tryParseInlinedExpression> 

        std::function<void (std::shared_ptr<Lang::Any>)> parseNewVar(); // returns var setter
        
        template<typename T>
        T parseInBrackets(std::function<T()> parseFunction);
        template<typename T>
        std::optional<T> tryParseInBrackets(std::function<T()> parseFunction);

        std::string parseUntil(char token);
        std::string parseUntil(std::string token);
        std::string parseUntilSpace();
        void skipToEOL();
        bool parseExcactToken(std::string token);
        void forceExcactToken(std::string token);
        bool parseExcactSpacedToken(std::string token);
        std::string parseVarToken();

        std::string skipSpaces();
        std::string skipSpacesRequired();

        

        /*enum class CommentType
        {
            NO_COMMENT,
            ONE_LINE,
            MULTILINE
        };*/

        //struct VariableFindResult
        //{
        //    std::shared_ptr<Lang::ASTToken> m_token;
        //    std::size_t m_tokenIndex = 0;
        //    // can be not provided
        //    std::shared_ptr<Lang::Table> m_variable;
        //    // can be not provided
        //    std::optional<Lang::Function> m_function;
        //};

        /*std::unordered_map<std::string, Tokens> m_tokensLookup {
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
        };*/

        /*std::shared_ptr<Lang::ASTToken> m_AST = std::make_shared<Lang::ASTToken>(Tokens::K_FILESTART);
        std::shared_ptr<Lang::ASTToken> m_currentCPPCodeToken = std::make_shared<Lang::ASTToken>(Tokens::K_CPP_CODE_LINE);
        std::shared_ptr<Lang::ASTToken> m_currentCharSeqToken = std::make_shared<Lang::ASTToken>(Tokens::K_CHAR_SEQ);*/

        // void buildAST(const std::string& templateFileText) noexcept;
        
        // void generateCodeUsingAST(const std::shared_ptr<Lang::ASTToken>& token,
        //                          std::string& outputString) noexcept;

        //void analyzeCurrentWordAndCharForTokens(std::shared_ptr<Lang::ASTToken>& currentToken,
        //                                        std::string& word,
        //                                        const std::string& text,
        //                                        std::size_t& curCharIdx,
        //                                        bool forceAddAsWord = false) noexcept;

        //std::shared_ptr<Lang::ASTToken> addToken(const std::shared_ptr<Lang::ASTToken>& toToken,
        //                                         Tokens tokenType) noexcept;

        //Lang::Tokens getTokenTypeByName(const std::string& tokenName) const noexcept;

        //void gotoParent(std::shared_ptr<Lang::ASTToken>& token) const noexcept;

        //[[nodiscard]] static bool isSpace(char c) noexcept;

        //[[nodiscard]] static VariableFindResult getLastVariable(const std::shared_ptr<Lang::ASTToken>& from, const size_t& begin) noexcept;

        //[[nodiscard]] std::any processFunctionTokensAndCallFunc(const std::shared_ptr<Lang::ASTToken>& inToken,
        //                                                        const size_t& tokenOffset,
        //                                                        Lang::Function& callableFunction,
        //                                                        std::string& outputString) const noexcept;

        //void addBuiltinVariables() noexcept;
        //std::shared_ptr<Lang::Table> metaToObject(SGCore::MetaInfo::Meta& meta);

        //// tmp variables ===================
        //bool m_isExprStarted = false;
        //bool m_isPlacementStarted = false;
        //CommentType m_currentCommentType = CommentType::NO_COMMENT;

        //// skipping this lang exprs to copy only c++ code
        //bool m_skipCodeCopy = false;

        //// can we write char sequence or not (example: "hello world")
        //bool m_writeCharSeq = false;

        //Lang::Table* m_currentUsedVariable = nullptr;

        //// =================================
    };
}

#endif //SUNGEARENGINE_CODEGENERATOR_H
