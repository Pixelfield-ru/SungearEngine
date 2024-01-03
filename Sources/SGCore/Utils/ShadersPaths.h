//
// Created by stuka on 19.11.2023.
//

#ifndef SUNGEARENGINE_SHADERSPATHS_H
#define SUNGEARENGINE_SHADERSPATHS_H

#include <unordered_map>
#include <string>

#include "SGCore/Patterns/Singleton.h"
#include "SGCore/Main/CoreGlobals.h"

struct ShadersPaths
{
    std::string m_virtualPath;

    std::string m_GLSL4RealizationPath;
    std::string m_GLSL46RealizationPath;

    std::string m_HLSLRealizationPath;

    ShadersPaths& operator[](const std::string& path) noexcept;

    // ---------------

    void createDefaultPaths() noexcept;

    [[nodiscard]] std::string getCurrentRealization() const noexcept;

    // ---------------

    auto& getPaths() noexcept { return m_paths; }

    // ---------------

private:
    std::unordered_map<std::string, SGCore::Ref<ShadersPaths>> m_paths;
};

#endif // SUNGEARENGINE_SHADERSPATHS_H
