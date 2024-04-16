//
// Created by stuka on 19.11.2023.
//
#include "ShadersPaths.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/GAPIType.h"
#include "SGCore/Graphics/API/IRenderer.h"

#include <memory>

SGCore::ShadersPaths& SGCore::ShadersPaths::operator[](const std::string& path) noexcept
{
    /*auto& paths = m_paths[path];
    if(!paths)
    {
        paths = std::make_shared<ShadersPaths>();
    }
    paths->m_virtualPath += m_virtualPath + "/" + path;*/

    auto foundShadersPaths = m_paths.find(path);

    if(foundShadersPaths == m_paths.end())
    {
        auto newShadersPaths = SGCore::MakeRef<ShadersPaths>();

        m_paths[path] = newShadersPaths;

        newShadersPaths->m_virtualPath += m_virtualPath + "/" + path;

        return *newShadersPaths;
    }

    foundShadersPaths->second->m_virtualPath += m_virtualPath + "/" + path;

    return *foundShadersPaths->second;
}

SGCore::ShadersPaths& SGCore::ShadersPaths::getByVirtualPath(const std::string& virtualPath) noexcept
{
    std::sregex_iterator iter(virtualPath.begin(), virtualPath.end(), s_pathsDividerRegex);
    ShadersPaths* curShadersPaths = this;
    while(iter != end)
    {
        curShadersPaths = &(*curShadersPaths)[(*iter)[0]];

        ++iter;
    }

    return *curShadersPaths;
}

std::string SGCore::ShadersPaths::getCurrentRealization() const noexcept
{
    auto gapiType = SGCore::CoreMain::getRenderer()->getGAPIType();

    switch(gapiType)
    {
        case SGCore::GAPIType::SG_API_TYPE_UNKNOWN: return "";
        case SGCore::GAPIType::SG_API_TYPE_GL4: return m_GLSL4RealizationPath;
        case SGCore::GAPIType::SG_API_TYPE_GL46: return m_GLSL46RealizationPath;
        case SGCore::GAPIType::SG_API_TYPE_GLES2: return "";
        case SGCore::GAPIType::SG_API_TYPE_GLES3: return "";
        case SGCore::GAPIType::SG_API_TYPE_VULKAN: return m_GLSL46RealizationPath;

        default: return "";
    }
}

bool SGCore::ShadersPaths::isSubPathExists(const std::string& subPath) const noexcept
{
    return m_paths.find(subPath) != m_paths.end();
}
