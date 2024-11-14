//
// Created by stuka on 19.11.2023.
//

#ifndef SUNGEARENGINE_SHADERSUNIVERSALPATHS_H
#define SUNGEARENGINE_SHADERSUNIVERSALPATHS_H

#include <SGCore/pch.h>

#include "Singleton.h"
#include "SGCore/Main/CoreGlobals.h"
#include "Utils.h"

namespace SGCore
{
    struct ShaderUniversalPath
    {
        friend struct ShadersUniversalPaths;

        [[nodiscard]] const std::filesystem::path& getCurrentRealization() const noexcept;

    private:
        std::string m_virtualPath;

        std::filesystem::path m_GLSL4RealizationPath;
        std::filesystem::path m_GLSL46RealizationPath;

        std::filesystem::path m_HLSLRealizationPath;
    };

    struct SGCORE_EXPORT ShadersUniversalPaths
    {
        static inline const std::filesystem::path undefined_path = "undefined";

        static void init() noexcept;

        std::optional<ShaderUniversalPath> operator[](const std::string& virtualPath) noexcept;

        ShaderUniversalPath addShader(const std::string& virtualPath,
                                      const std::filesystem::path& glsl4RealizationPath,
                                      const std::filesystem::path& glsl46RealizationPath,
                                      const std::filesystem::path& hlslRealizationPath) noexcept;

        [[nodiscard]] SG_NOINLINE static auto& getDefaultPaths() noexcept
        {
            return *m_defaultPaths;
        }

    private:
        std::unordered_map<std::string, ShaderUniversalPath> m_paths;

        static inline Ref<ShadersUniversalPaths> m_defaultPaths { };
    };
}

#endif // SUNGEARENGINE_SHADERSUNIVERSALPATHS_H
