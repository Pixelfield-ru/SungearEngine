//
// Created by stuka on 24.12.2023.
//

#ifndef SGSLINTERPRETER_GLSLSHADERSPREPROCESSOR_H
#define SGSLINTERPRETER_GLSLSHADERSPREPROCESSOR_H

#include <string>
#include <sstream>
#include <regex>
#include <filesystem>
#include <iostream>
#include <set>
#include <algorithm>
#include <utility>
#include <unordered_map>

#include "SGUtils/Utils.h"
#include "ShaderAnalyzedFile.h"
#include "SGSLSubPass.h"
#include "SGSLSubShader.h"
#include "SGSLVariable.h"
#include "SGSLSubShaderType.h"

// TODO: ВООБЩЕ НЕ ГОДИТСЯ ПОД МНОГОПТОК
namespace SGCore
{
    class GLSLShadersPreprocessor
    {
    public:
        static ShaderAnalyzedFile processCode(const std::string& shaderFilePath, const std::string_view& rawCode, GLSLShadersPreprocessor& preprocessor) noexcept;

    private:
        static void checkAndCompileSGSLExpr(std::string& line, const size_t& currentLineIdx, ShaderAnalyzedFile& shaderAnalyzedFile, GLSLShadersPreprocessor& preprocessor);

        static void appendLine(const std::string& line, ShaderAnalyzedFile& shaderAnalyzedFile);

        static inline const std::regex m_assignExprsRegex = std::regex(R"((\w+)\s+(\w+(\[\d*\])?)\s*=\s*(\w+)\s*\(([^)]*)\);)");
        static inline const std::regex m_stringArgsDividerRegex = std::regex("\"([^\"]+)\"");
        static inline const std::regex m_arraySizeRegex = std::regex(R"(\d+)");
        static inline const std::regex m_variableNameRegex = std::regex(R"([a-zA-Z]+)");
        static inline const std::regex m_sgslTypeFuncCallRegex = std::regex(R"(\b([a-zA-Z_]\w*)\s*\.\s*([a-zA-Z_]\w*\(\)))");
        static inline const std::regex m_sgslSubPassesDeclRegex = std::regex(R"(\s*(\w+)\s*\(([^)]*)\)\{)");
        static inline const std::regex m_nonStringArgsDividerRegex = std::regex(R"([a-zA-Z]+)");

        static inline const std::sregex_iterator end;

        bool m_sgslUniformsDeclaration = false;

        bool m_ignoreNextChars = false;

        std::set<std::string> m_pushedDefines;
        bool m_lastIfDefDirectiveExists = true;

        std::set<SGSLVariable> m_declaredVariables;
        static inline std::set<std::string> m_arraySizeNotRequiredFunctions { "SGGetTextures" };
        static inline std::set<std::string> m_arraySizeRequiredFunctions { "SGGetTexturesFromMaterial", "SGGetFrameBufferTextures", "SGGetAllFrameBuffersAttachmentsWithType" };
    };
}

#endif //SGSLINTERPRETER_GLSLSHADERSPREPROCESSOR_H
