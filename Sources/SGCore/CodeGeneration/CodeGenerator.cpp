//
// Created by stuka on 10.09.2024.
//

#include <SGCore/Utils/FileUtils.h>
#include "CodeGenerator.h"

#include "SGCore/Logger/Logger.h"



std::string SGCore::CodeGen::Generator::generate(const std::filesystem::path& templateFile) noexcept
{
    // std::string templateFileText = Utils::reduce(FileUtils::readFile(templateFile));
    std::string templateFileText = FileUtils::readFile(templateFile);
    // instead of EOF
    // templateFileText += '\n';

    std::string outputString;

    // buildAST(templateFileText);
    // generateCodeUsingAST(m_AST, outputString);

    // std::printf("end\n");

    /*m_AST = std::make_shared<Lang::ASTToken>(Tokens::K_FILESTART);
    m_currentCPPCodeToken = std::make_shared<Lang::ASTToken>(Tokens::K_CPP_CODE_LINE);
    m_currentCharSeqToken = std::make_shared<Lang::ASTToken>(Tokens::K_CHAR_SEQ);*/

    //addBuiltinVariables();

    return outputString;
}

char SGCore::CodeGen::Generator::SourceCodeReader::lnext(std::string excpected)
{
    if (!hasNext())
        throw ParseException(excpected + " excpected.");

    return source.source[cursor];
}

char SGCore::CodeGen::Generator::SourceCodeReader::next(std::string excpected)
{
    if (!hasNext())
        throw ParseException(excpected + " excpected.");
    
    return source.source[cursor++];
}

std::string SGCore::CodeGen::Generator::SourceCodeReader::next(int count, std::string excpected)
{
    if (!hasNext(count))
        throw ParseException(excpected + " excpected.");

    return source.source.substr(cursor, cursor += count);
}

std::string SGCore::CodeGen::Generator::SourceCodeReader::lnext(int count, std::string excpected)
{
    if (!hasNext(count))
        throw ParseException(excpected + " excpected.");

    return source.source.substr(cursor, cursor + count);
}

bool SGCore::CodeGen::Generator::SourceCodeReader::hasNext()
{
    return cursor < source.source.length();
}

bool SGCore::CodeGen::Generator::SourceCodeReader::hasNext(int count)
{
    return cursor + (count - 1) < source.source.length();
}


std::pair<std::string, std::function<std::string()>> SGCore::CodeGen::Generator::parseAny() {
    auto lineGetters = std::vector<std::function<std::string()>>();
    while (m_reader.hasNext()) {
        lineGetters.push_back(parseLine());
    }

    // FIXME: add cancel reason
    return std::pair("", [lineGetters]() {
        std::string outputString = "";
        for (auto& getter : lineGetters) {
            outputString += getter();
        }
        return outputString;
    });
}

std::function<std::string()> SGCore::CodeGen::Generator::parseLine() {
    std::string skippedSpaces = skipSpaces();
    if (m_reader.hasNext(2) && m_reader.lnext(2) == "##") {
        m_reader.next(2);
        return parseCode();
    }

    auto lineRemains = parseUntil('\n'); // TODO: replace with parseRawCode
    m_reader.next();

    return [skippedSpaces, lineRemains]() { return skippedSpaces + lineRemains + "\n"; };
}

std::function<std::string()> SGCore::CodeGen::Generator::parseCode() {
    skipSpaces();
    auto forLoop = tryParseForLoop();
    if (forLoop.has_value())
        return forLoop.value();

    throw ParseException("Expression required");
}

std::optional<std::function<SGCore::CodeGen::Lang::Any()>> SGCore::CodeGen::Generator::tryParseVarWithPath() {
    m_reader.push();

    auto firstExpr = tryParseInlinedExpression();
    if (firstExpr.has_value()) {
        auto path = std::vector<std::string>();

        m_reader.apply();
        skipSpaces();
        while (parseExcactToken(".")) {
            skipSpaces();
            path.push_back(parseVarToken());
            skipSpaces();
        }
         
        return [firstExpr, path]() {
            auto first = firstExpr.value()();
            auto current = dynamic_cast<Lang::Table*>(&first);
            if (current == nullptr)
                throw new RuntimeException("Table excpected");

            for (auto i = 0; i < path.size(); i++) {
                auto next = (std::shared_ptr<Lang::Any>)((*current)[path[i]]);
                current = &*std::dynamic_pointer_cast<Lang::Table>(next);
            }

            return *current;
        };
    }

    m_reader.pop();
    return std::nullopt;
}

std::optional<std::function<SGCore::CodeGen::Lang::Any()>> SGCore::CodeGen::Generator::tryParseInlinedExpression()
{
    m_reader.push();

    skipSpaces();

    // Try parse inlined exprs:

    // Try parse subexpr:

    if (parseExcactToken("(")) {
        m_reader.apply();
        skipSpaces();

        auto parsed = tryParseExpression();
        if (!parsed.has_value()) {
            throw ParseException("Expression excpected");
        }

        forceExcactToken(")"); skipSpaces();

        return parsed;
    }

    m_reader.pop();
    return std::nullopt;
}

std::optional<std::function<SGCore::CodeGen::Lang::Any()>> SGCore::CodeGen::Generator::tryParseExpression() {
    m_reader.push();

    // Trying parse as self

    auto asPath = tryParseVarWithPath();
    if (asPath.has_value()) {
        m_reader.apply();
        return asPath.value();
    }

    // Trying parse as inline expr:

    auto inlined = tryParseInlinedExpression();
    if (inlined.has_value()) {
        m_reader.apply();
        return inlined.value();
    }
    m_reader.pop();
    return std::nullopt;
}

std::optional<std::function<std::string()>> SGCore::CodeGen::Generator::tryParseIf()
{
    m_reader.push();
    if (!parseExcactToken("if")) {
        m_reader.pop();
        return std::nullopt;
    }
    m_reader.apply();

    skipSpacesRequired();

    auto expr = tryParseExpression(); skipSpaces();
    if (!expr.has_value())
        throw ParseException("Expression required");

    forceExcactToken(":"); skipToEOL();

    auto ifContent = parseAny();

    std::function<std::string()> elseContent = []() {return ""; };
    if (ifContent.first == "else") {
        elseContent = ifContent.second;
    }

    return [expr, ifContent, elseContent] {
        auto exprSolved = expr.value()();
        // FIXME: add bool type
        auto exprAsBool = dynamic_cast<Lang::Number*>(&exprSolved);

        if (exprAsBool->m_value != 0) {
            return ifContent.second();
        }
        return elseContent();
    };
}

std::optional<std::function<std::string()>> SGCore::CodeGen::Generator::tryParseForLoop()
{
    m_reader.push();
    if (!parseExcactToken("for")) {                             // for
        m_reader.pop();
        return std::nullopt;
    }
    m_reader.apply(); // Now way back! On parse faield - error

    skipSpacesRequired();
    auto varSetter = parseNewVar(); skipSpacesRequired();       // var
    forceExcactToken("in"); skipSpacesRequired();               // in
    auto sourceVarGetter = tryParseExpression(); skipSpaces();
    forceExcactToken(":"); skipToEOL();   // iterable

    if (!sourceVarGetter.has_value())
        throw "Expression for iterable excpected";

    auto forContent = parseAny();

    return [varSetter, sourceVarGetter, forContent, this]() {
        m_currentScope = m_currentScope.createSubScope();

        auto sourceVar = dynamic_cast<Lang::Table*>(&(sourceVarGetter.value()()));
        if (sourceVar == nullptr)
            throw RuntimeException("variable is null!");

        std::string outputStr = "";

        auto keyObj = std::make_shared<Lang::String>(Lang::String());
        for (auto& key : sourceVar->getMembers()) {
            keyObj->m_value = key.first;
            varSetter(keyObj);
            outputStr += forContent.second();
        }

        m_currentScope = m_currentScope.destroySubScope();
        return outputStr;
    };
}