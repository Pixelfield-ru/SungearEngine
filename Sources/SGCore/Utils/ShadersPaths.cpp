//
// Created by stuka on 19.11.2023.
//
#include "ShadersPaths.h"
#include "SGCore/Main/CoreMain.h"

ShadersPaths& ShadersPaths::operator[](const std::string& path) noexcept
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
        auto newShadersPaths = std::shared_ptr<ShadersPaths>(new ShadersPaths);

        m_paths[path] = newShadersPaths;

        newShadersPaths->m_virtualPath += m_virtualPath + "/" + path;

        return *newShadersPaths;
    }

    foundShadersPaths->second->m_virtualPath += m_virtualPath + "/" + path;

    return *foundShadersPaths->second;
}

void ShadersPaths::createDefaultPaths() noexcept
{
    (*this)["PBR"]["DefaultShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/pbr/default_shader.glsl";

    // -------------------------------------

    (*this)["PostProcessing"]["DepthPassShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/postprocessing/layer_depth_pass.glsl";

    (*this)["PostProcessing"]["FinalOverlayShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/postprocessing/final_pp_fx_shader.glsl";

    (*this)["PostProcessing"]["DefaultLayerShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/postprocessing/pp_layer_fx_shader.glsl";

    (*this)["PostProcessing"]["AttachmentsCombiningShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/postprocessing/attachments_combining.glsl";

    // -------------------------------------

    (*this)["Gizmos"]["ComplexGizmosShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/gizmos/complex_gizmos_shader.glsl";

    (*this)["Gizmos"]["LinesGizmosShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/gizmos/lines_shader.glsl";

    // -------------------------------------

    (*this)["ShadowsGeneration"]["DefaultShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/shadows_generator/default_shader.glsl";

    // -------------------------------------

    (*this)["Skybox"]["DefaultShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/skybox/default_shader.glsl";

    // -------------------------------------
}

std::string ShadersPaths::getCurrentRealization() const noexcept
{
    auto gapiType = SGCore::CoreMain::getRenderer().getGAPIType();

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
