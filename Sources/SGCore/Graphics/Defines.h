#include <string>

#ifndef SUNGEARENGINE_DEFINES_H
#define SUNGEARENGINE_DEFINES_H

// glsl4 shaders ---------------------------------------------------

#define SG_GLSL4_PBR_SHADER_PATH                            "../SGResources/shaders/glsl4/pbr/default_shader.glsl"

#define SG_GLSL4_SHADOWS_GENERATOR_SHADER_PATH              "../SGResources/shaders/glsl4/shadows_generator/default_shader.glsl"

#define SG_GLSL4_SKYBOX_SHADER_PATH                         "../SGResources/shaders/glsl4/skybox/default_shader.glsl"

#define SG_GLSL4_LINES_SHADER_PATH                          "../SGResources/shaders/glsl4/primitives/lines_shader.glsl"
#define SG_GLSL4_COMPLEX_PRIMITIVES_SHADER_PATH             "../SGResources/shaders/glsl4/primitives/complex_primitives_shader.glsl"

#define SG_GLSL4_PP_LAYER_DEPTH_TEST_SHADER_PATH            "../SGResources/shaders/glsl4/postprocessing/pp_layer_depth_test_shader.glsl"
#define SG_GLSL4_DEFAULT_PP_LAYER_FX_APPLY_SHADER_PATH     "../SGResources/shaders/glsl4/postprocessing/default_pp_layer_fx_apply_shader.glsl"
#define SG_GLSL4_FINAL_PP_OVERLAY_SHADER_PATH               "../SGResources/shaders/glsl4/postprocessing/final_pp_overlay_shader.glsl"

// -----------------------------------------------------------------

// glsl46 shaders --------------------------------------------------

#define SG_GLSL46_PBR_SHADER_PATH                   "../SGResources/shaders/glsl46/pbr/default_shader.glsl"

// -----------------------------------------------------------------

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
        SG_COMPLEX_PRIMITIVES_SHADER,
        SG_PP_LAYER_DEPTH_TEST_SHADER,
        SG_PP_LAYER_APPLY_FX_SHADER,
        SG_PP_FINAL_OVERLAY_SHADER
    };

    /**
     *
     * @param shaderType - Type of standard shader.
     * @return Path to the desired standard shader based on the currently selected renderer.
     */
    std::string getShaderPath(const StandardShaderType& shaderType);
}

#endif // SUNGEARENGINE_DEFINES_H
