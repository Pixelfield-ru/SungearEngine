#include <string>

#ifndef SUNGEARENGINE_GRAPHICSFILESRESOURCESMANAGER_H
#define SUNGEARENGINE_GRAPHICSFILESRESOURCESMANAGER_H

// shaders ----------------------------------------------------------

#define SG_GLSL4_PBR_SHADER_PATH "../SGResources/shaders/glsl4/pbr/default_shader.glsl"
#define SG_GLSL46_PBR_SHADER_PATH "../SGResources/shaders/glsl46/pbr/default_shader.glsl"

#define SG_GLSL4_SHADOWS_GENERATOR_SHADER_PATH "../SGResources/shaders/glsl4/shadows_generator/default_shader.glsl"

#define SG_GLSL4_SKYBOX_SHADER_PATH "../SGResources/shaders/glsl4/skybox/default_shader.glsl"

#define SG_GLSL4_SKYBOX_SHADER_PATH "../SGResources/shaders/glsl4/skybox/default_shader.glsl"

#define SG_GLSL4_LINES_SHADER_PATH "../SGResources/shaders/glsl4/primitives/lines_shader.glsl"
#define SG_GLSL4_COMPLEX_PRIMITIVES_SHADER_PATH "../SGResources/shaders/glsl4/primitives/complex_primitives_shader.glsl"

// -----------------------------------------------------------------

#define SG_SHADERS_SHADOWS_CASTERS_NUM_NAME "SHADOWS_CASTERS_NUM"
#define SG_SHADERS_DIRECTIONAL_LIGHTS_NUM_NAME "DIRECTIONAL_LIGHTS_NUM"

#define SG_FRAMEBUFFER_DEPTH_ATTACHMENT_NAME "depthAttachment"

#define SG_TRANSLATE_WITH_OBJECT "SG_TRANSLATE_WITH_OBJECT"
#define SG_ROTATE_WITH_OBJECT "SG_ROTATE_WITH_OBJECT"
#define SG_SCALE_WITH_OBJECT "SG_SCALE_WITH_OBJECT"

namespace Core::Main
{
    class CoreMain;
}

namespace Core::Graphics
{
    enum StandardShaderType
    {
        SG_PBR_SHADER,
        SG_SHADOWS_GENERATOR_SHADER,
        SG_SKYBOX_SHADER,
        SG_LINES_SHADER,
        SG_COMPLEX_PRIMITIVES_SHADER
    };

    /**
     *
     * @param shaderType - Type of standard shader.
     * @return Path to the desired standard shader based on the currently selected renderer.
     */
    std::string getShaderPath(const StandardShaderType& shaderType);
}

#endif //SUNGEARENGINE_GRAPHICSFILESRESOURCESMANAGER_H
