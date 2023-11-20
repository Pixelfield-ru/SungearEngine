//
// Created by stuka on 19.11.2023.
//
#include "ShadersPaths.h"
#include "SGCore/Main/CoreMain.h"

ShadersPaths& ShadersPaths::operator[](const std::string& path) noexcept
{
    auto& paths = m_paths[path];
    if(!paths)
    {
        paths = std::make_shared<ShadersPaths>();
    }
    paths->m_virtualPath += m_virtualPath + "/" + path;

    /*SGCF_INFO("New ShadersPaths. "
              "Current realization: " + paths.getCurrentRealization() + ". ShadersPaths virtual path: " +
              paths.m_virtualPath, SG_LOG_CURRENT_SESSION_FILE);*/

    return *paths;
}

void ShadersPaths::createDefaultPaths() noexcept
{
    operator[]("PBR")["DefaultShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/pbr/default_shader.glsl";

    // -------------------------------------

    operator[]("PostProcessing")["FinalOverlayShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/postprocessing/final_pp_overlay_shader.glsl";

    operator[]("PostProcessing")["DefaultLayerShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/postprocessing/pp_layer_shader.glsl";

    // -------------------------------------

    operator[]("Gizmos")["ComplexGizmosShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/gizmos/complex_gizmos_shader.glsl";

    operator[]("Gizmos")["LinesShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/gizmos/lines_shader.glsl";

    // -------------------------------------

    operator[]("ShadowsGeneration")["DefaultShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/shadows_generator/default_shader.glsl";

    // -------------------------------------

    operator[]("Skybox")["DefaultShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/skybox/default_shader.glsl";

    // -------------------------------------
}

std::string ShadersPaths::getCurrentRealization() const noexcept
{
    auto gapiType = Core::Main::CoreMain::getRenderer().getGAPIType();

    switch(gapiType)
    {
        case Core::Graphics::SG_API_TYPE_UNKNOWN: return "";
        case Core::Graphics::SG_API_TYPE_GL4: return m_GLSL4RealizationPath;
        case Core::Graphics::SG_API_TYPE_GL46: return m_GLSL46RealizationPath;
        case Core::Graphics::SG_API_TYPE_GLES2: return "";
        case Core::Graphics::SG_API_TYPE_GLES3: return "";
        case Core::Graphics::SG_API_TYPE_VULKAN: return m_GLSL46RealizationPath;

        default: return "";
    }
}
