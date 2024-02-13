//
// Created by ilya on 07.02.24.
//

#ifndef SGSLETRANSLATOR_SGSLETRANSLATOR_H
#define SGSLETRANSLATOR_SGSLETRANSLATOR_H

#include <string>
#include <vector>
#include <set>
#include <regex>
#include "SGSLETranslatorConfiguration.h"
#include "ShaderAnalyzedFile.h"

namespace SGCore
{
    class SGSLETranslator
    {
    public:
        std::shared_ptr<ShaderAnalyzedFile> processCode(const std::string& path, const std::string& code, SGSLETranslator& translator) noexcept;
        std::shared_ptr<ShaderAnalyzedFile> processCode(const std::string& path, const std::string& code) noexcept;

        SGSLETranslatorConfiguration m_config;

    private:
        std::shared_ptr<ShaderAnalyzedFile> processCode(const std::string& path, const std::string& code, SGSLETranslator& translator, bool isRootShader) noexcept;

        std::string sgsleCodeCorrector(const std::string& code);
        std::shared_ptr<ShaderAnalyzedFile> sgslePreprocessor(const std::string& path, const std::string& code) noexcept;
        std::shared_ptr<ShaderAnalyzedFile> sgslePreProcessor(const std::string& path, const std::string& code, SGSLETranslator& translator) noexcept;
        std::shared_ptr<ShaderAnalyzedFile> sgsleMainProcessor(const std::shared_ptr<ShaderAnalyzedFile>& code, SGSLETranslator& translator) noexcept;
        
        void sgsleMakeSubShaderCodePretty(SGSLESubShader& subShader) const noexcept;
        
        static inline const std::regex s_rSideOfAssignExprRegex = std::regex(R"(\s*(\w+)\s*\(([^)]*)\);)");
        static inline const std::regex m_sgslFuncWithArgsCallRegex = std::regex(R"(\s*(\w+)\s*\(([^)]*)\))");
        static inline const std::regex m_funcArgsDividerRegex = std::regex(R"([a-zA-Z0-9_.-]+)");
        static inline const std::regex m_arrayVariableDeclRegex = std::regex(R"((\w+)\[\s*(\d*)\s*\])");
        static inline const std::regex m_sgslVariableFuncCallRegex = std::regex(R"(\b([a-zA-Z_]\w*)\s*\.\s*([a-zA-Z_]\w*\(\)))");
        // regex for assigns like samplers[0..5] = ...
        static inline const std::regex m_lSideOfAssignExprRegex0 = std::regex(R"((\w+)\[\s*(\d+)\s*(?:\.\.\s*(\d+)\s*)?\])");
        static inline const std::regex m_rSideSGFuncCallRegex = std::regex(R"(\s*(\w+)\s*\(([^)]*)\);)");
        static inline const std::sregex_iterator m_regexIterEnd;
        
        std::vector<SGSLESubPass*> m_currentSubPasses;
        std::vector<std::pair<SGSLESubPass*, SGSLESubShader*>> m_currentSubShaders;
        
        std::set<std::string> m_includedFiles;
    };
}

#endif //SGSLETRANSLATOR_SGSLETRANSLATOR_H
