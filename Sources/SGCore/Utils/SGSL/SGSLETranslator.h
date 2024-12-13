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
         * \@subpass <name of subpass (without spaces)>\n
         * \@fragment (or \@vertex or \@geometry) - current shader annotation\n
         * \@attribute <name of attribute (without spaces)> <value (spaces can be used)>\n
         * PREPROCESSOR DIRECTIVES:\n
         * #include "path to file" - NOTE THAT ALL FILES HAS "PRAGMA ONCE" BY DEFAULT.
         */

        SGSLETranslatorConfiguration m_config;

        void processCode(const std::filesystem::path& path, const std::string& code, const Ref<ShaderAnalyzedFile>& toAnalyzedFile) noexcept;

        static void includeDirectory(const std::filesystem::path& dirPath) noexcept;

    private:
        [[nodiscard]] std::string preprocessorPass(const std::string& code) noexcept;

        std::unordered_set<std::filesystem::path> m_includedPaths;

        static inline std::vector<std::filesystem::path> s_includedDirectories;

        /**
         * Reads line ignoring comments.
         * @param startIdx
         * @param code
         * @return
         */
        [[nodiscard]] static std::pair<std::string, size_t> readLine(const size_t& startIdx, const std::string& code) noexcept;

        static bool isValidCommentToken(const std::string& commentToken) noexcept;

        [[nodiscard]] static std::filesystem::path findRealIncludePath(const std::filesystem::path& originalIncludePath) noexcept;
    };
}

#endif //SUNGEARENGINE_SGSLETRANSLATOR_H
