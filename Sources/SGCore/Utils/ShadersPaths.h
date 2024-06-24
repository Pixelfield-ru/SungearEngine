//
// Created by stuka on 19.11.2023.
//

#ifndef SUNGEARENGINE_SHADERSPATHS_H
#define SUNGEARENGINE_SHADERSPATHS_H

#include <SGCore/pch.h>

#include "SGUtils/Singleton.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct ShadersPaths
    {
        std::string m_virtualPath;

        std::string m_GLSL4RealizationPath;
        std::string m_GLSL46RealizationPath;

        std::string m_HLSLRealizationPath;

        ShadersPaths& operator[](const std::string& path) noexcept;

        ShadersPaths& getByVirtualPath(const std::string& virtualPath) noexcept;

        // ---------------

        [[nodiscard]] std::string getCurrentRealization() const noexcept;

        // ---------------

        auto& getPaths() noexcept
        { return m_paths; }

        // ---------------

        bool isSubPathExists(const std::string& subPath) const noexcept;

    private:
        // todo: make divider by / or \\ or both
        static inline const std::regex s_pathsDividerRegex = std::regex(R"([a-zA-Z]+)");
        static inline const std::sregex_iterator end;

        std::unordered_map<std::string, Ref<ShadersPaths>> m_paths;
    };
}

#endif // SUNGEARENGINE_SHADERSPATHS_H
