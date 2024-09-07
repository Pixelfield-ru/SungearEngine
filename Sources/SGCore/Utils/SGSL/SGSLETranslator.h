//
// Created by ilya on 07.02.24.
//

#ifndef SGSLETRANSLATOR_SGSLETRANSLATOR_H
#define SGSLETRANSLATOR_SGSLETRANSLATOR_H

#include <SGCore/pch.h>

#include "SGSLETranslatorConfiguration.h"
#include "ShaderAnalyzedFile.h"

namespace SGCore
{
    class SGSLETranslator
    {
    public:
        void processCode(const std::filesystem::path& path, const std::string& code, SGSLETranslator& translator, const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept;
        void processCode(const std::filesystem::path& path, const std::string& code, const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept;

        SGSLETranslatorConfiguration m_config;

    private:
        void processCode(const std::filesystem::path& path,
                         const std::string& code,
                         SGSLETranslator& translator,
                         bool isRootShader,
                         const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept;

        std::string sgsleCodeCorrector(const std::string& code);
        void sgslePreprocessor(const std::string& path, const std::string& code, const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept;
        void sgslePreProcessor(const std::string& path, const std::string& code, SGSLETranslator& translator, const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept;
        void sgsleMainProcessor(const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile, SGSLETranslator& translator) noexcept;
        
        void sgsleMakeSubShaderCodePretty(const Ref<SGCore::SGSLESubShader>& subShader) const noexcept;
        
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
