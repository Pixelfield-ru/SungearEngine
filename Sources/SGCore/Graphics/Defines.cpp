#include "Defines.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/APIType.h"

std::string Core::Graphics::getShaderPath(const StandardShaderType& shaderType)
{
    std::string shaderPath;

    switch(shaderType)
    {
        case SG_PBR_SHADER:
            switch(Main::CoreMain::getRenderer().getAPIType())
            {
                case SG_API_TYPE_GL4:
                    shaderPath = SG_GLSL4_PBR_SHADER_PATH;
                    break;

                case SG_API_TYPE_GL46:
                    shaderPath = SG_GLSL46_PBR_SHADER_PATH;
                    break;
            }
            break;

        case SG_SHADOWS_GENERATOR_SHADER:
            switch(Main::CoreMain::getRenderer().getAPIType())
            {
                case SG_API_TYPE_GL4:
                    shaderPath = SG_GLSL4_SHADOWS_GENERATOR_SHADER_PATH;
                    break;
            }
            break;

        case SG_SKYBOX_SHADER:
            switch(Main::CoreMain::getRenderer().getAPIType())
            {
                case SG_API_TYPE_GL4:
                    shaderPath = SG_GLSL4_SKYBOX_SHADER_PATH;
                    break;
            }
            break;

        case SG_LINES_SHADER:
            switch(Main::CoreMain::getRenderer().getAPIType())
            {
                case SG_API_TYPE_GL4:
                    shaderPath = SG_GLSL4_LINES_SHADER_PATH;
                    break;
            }
            break;

        case SG_COMPLEX_PRIMITIVES_SHADER:
            switch(Main::CoreMain::getRenderer().getAPIType())
            {
                case SG_API_TYPE_GL4:
                    shaderPath = SG_GLSL4_COMPLEX_PRIMITIVES_SHADER_PATH;
                    break;
            }
            break;

        case SG_PP_LAYER_SHADER:
            switch(Main::CoreMain::getRenderer().getAPIType())
            {
                case SG_API_TYPE_GL4:
                    shaderPath = SG_GLSL4_PP_LAYER_SHADER_PATH;
                    break;
            }
            break;

        case SG_PP_FINAL_OVERLAY_SHADER:
            switch(Main::CoreMain::getRenderer().getAPIType())
            {
                case SG_API_TYPE_GL4:
                    shaderPath = SG_GLSL4_FINAL_PP_OVERLAY_SHADER_PATH;
                    break;
            }
            break;
    }

    return shaderPath;
}
