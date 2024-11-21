//
// Created by stuka on 19.11.2023.
//
#include "ShadersUniversalPaths.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/GAPIType.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/IShader.h"

#include <memory>

const std::filesystem::path& SGCore::ShaderUniversalPath::getCurrentRealization() const noexcept
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

void SGCore::ShadersUniversalPaths::init() noexcept
{
    m_defaultPaths = MakeRef<ShadersUniversalPaths>();

    m_defaultPaths->addShader("Gizmos/ComplexGizmosShader",
                              CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/gizmos/complex_gizmos_shader.glsl",
                              ShadersUniversalPaths::undefined_path,
                              ShadersUniversalPaths::undefined_path
    );

    m_defaultPaths->addShader("Gizmos/LinesGizmosShader",
                              CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/gizmos/lines_shader.glsl",
                              ShadersUniversalPaths::undefined_path,
                              ShadersUniversalPaths::undefined_path
    );

    m_defaultPaths->addShader("LayeredPP/ReceiverShader",
                              CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/postprocessing/layered/receiver_shader.glsl",
                              ShadersUniversalPaths::undefined_path,
                              ShadersUniversalPaths::undefined_path
    );

    m_defaultPaths->addShader("LayeredPP/LayerFXShader",
                              CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/postprocessing/layered/layer_default_fx.glsl",
                              ShadersUniversalPaths::undefined_path,
                              ShadersUniversalPaths::undefined_path
    );

    m_defaultPaths->addShader("LayeredPP/FinalFrameFXShader",
                              CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/postprocessing/layered/final_frame_fx.glsl",
                              ShadersUniversalPaths::undefined_path,
                              ShadersUniversalPaths::undefined_path
    );

    m_defaultPaths->addShader("InfiniteGrid",
                              CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/gizmos/infinite_grid.glsl",
                              ShadersUniversalPaths::undefined_path,
                              ShadersUniversalPaths::undefined_path
    );
}

SGCore::ShaderUniversalPath SGCore::ShadersUniversalPaths::addShader(const std::string& virtualPath,
                                                                     const std::filesystem::path& glsl4RealizationPath,
                                                                     const std::filesystem::path& glsl46RealizationPath,
                                                                     const std::filesystem::path& hlslRealizationPath) noexcept
{
    auto foundIt = m_paths.find(virtualPath);
    if(foundIt != m_paths.end()) return foundIt->second;

    ShaderUniversalPath path;
    path.m_virtualPath = virtualPath;

    path.m_GLSL4RealizationPath = glsl4RealizationPath;
    path.m_GLSL46RealizationPath = glsl46RealizationPath;
    path.m_HLSLRealizationPath = hlslRealizationPath;

    m_paths[virtualPath] = path;

    return path;
}

std::optional<SGCore::ShaderUniversalPath> SGCore::ShadersUniversalPaths::operator[](const std::string& virtualPath) noexcept
{
    auto foundIt = m_paths.find(virtualPath);
    if(foundIt == m_paths.end()) return std::nullopt;

    return foundIt->second;
}
