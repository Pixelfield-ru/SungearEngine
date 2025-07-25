//
// Created by stuka on 13.12.2024.
//

#ifndef SUNGEARENGINE_SGSLETRANSLATOR_H
#define SUNGEARENGINE_SGSLETRANSLATOR_H

#include <unordered_set>
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/SGSL/SGSLETranslatorConfiguration.h"

namespace SGCore
{
    struct ShaderAnalyzedFile;

    struct SGSLETranslator
    {
        /**
         * SHADERS LANGUAGE LANGUAGE EXTENSION.\n
         * Key words and usage.\n
         * \p #subpass [name of subpass]\n
         * \p #fragment (or \p #vertex or \p #geometry) - current shader annotation\n
         * \p #attribute [name of attribute] [value]\n
         *
         * PREPROCESSOR DIRECTIVES:\n
         * \p #include "path to file" - NOTE THAT ALL FILES HAS "PRAGMA ONCE" BY DEFAULT.\n
         * \n
         * TYPE SPACE AFTER ALL KEYWORDS!
         */

        SGSLETranslatorConfiguration m_config;

        void processCode(const std::filesystem::path& path, const std::string& code, const Ref<ShaderAnalyzedFile>& toAnalyzedFile) noexcept;

        void clearOutputDirectory() const noexcept;

        static void includeDirectory(const std::filesystem::path& dirPath) noexcept;

    private:
        /**
         * Includes processing preprocessor directives (like #include) and comments.
         * @param path
         * @param code
         * @return
         */
        [[nodiscard]] std::string preprocessorPass(const std::filesystem::path& path, const std::string& code) noexcept;

        void translateCode(const std::filesystem::path& path, const std::string& code, const Ref<ShaderAnalyzedFile>& toAnalyzedFile) noexcept;

        std::unordered_set<std::filesystem::path> m_includedPaths;

        static inline std::vector<std::filesystem::path> s_includedDirectories;

        /**
         * Reads line ignoring comments.
         * @param startIdx
         * @param code
         * @return First - code, second - count of read characters.
         */
        [[nodiscard]] static std::pair<std::string, size_t> readExpr(const size_t& startIdx, const std::string& code,
                                                                     const std::string& exprFenceBegin,
                                                                     const std::string& exprFenceEnd) noexcept;

        /**
         * Reads word ignoring comments.
         * @param startIdx
         * @param code
         * @return First - code, second - count of read characters.
         */
        [[nodiscard]] static std::pair<std::string, size_t> readWord(const size_t& startIdx, const std::string& code) noexcept;

        static bool isValidCommentToken(const std::string& commentToken) noexcept;

        [[nodiscard]] static std::filesystem::path findRealIncludePath(const std::filesystem::path& originalIncludePath) noexcept;
    };
}

#endif //SUNGEARENGINE_SGSLETRANSLATOR_H
