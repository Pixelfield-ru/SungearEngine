//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_GRAPHICSFILESRESOURCESMANAGER_H
#define SUNGEARENGINE_GRAPHICSFILESRESOURCESMANAGER_H

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/APIType.h"

// shaders ----------------------------------------------------------

#define SG_GLSL4_PBR_SHADER_PATH "../SGResources/shaders/glsl4/pbr/default_shader.glsl"
#define SG_GLSL46_PBR_SHADER_PATH "../SGResources/shaders/glsl46/pbr/default_shader.glsl"

#define SG_GLSL4_SHADOWS_GENERATOR_SHADER_PATH "../SGResources/shaders/glsl4/shadows_generator/default_shader.glsl"

#define SG_GLSL4_SKYBOX_SHADER_PATH "../SGResources/shaders/glsl4/skybox/default_shader.glsl"

#define SG_GLSL4_SKYBOX_SHADER_PATH "../SGResources/shaders/glsl4/skybox/default_shader.glsl"

#define SG_GLSL4_PRIMITIVES_SHADER_PATH "../SGResources/shaders/glsl4/primitives/default_shader.glsl"

// -----------------------------------------------------------------

#define SG_SHADERS_SHADOWS_CASTERS_NUM_NAME "SHADOWS_CASTERS_NUM"
#define SG_SHADERS_DIRECTIONAL_LIGHTS_NUM_NAME "DIRECTIONAL_LIGHTS_NUM"

#define SG_FRAMEBUFFER_DEPTH_ATTACHMENT_NAME "depthAttachment"

namespace Core::Graphics
{
    enum StandardShaderType
    {
        SG_PBR_SHADER,
        SG_SHADOWS_GENERATOR_SHADER,
        SG_SKYBOX_SHADER,
        SG_PRIMITIVES_SHADER
    };

    /**
     *
     * @param shaderType - Type of standard shader.
     * @return Path to the desired standard shader based on the currently selected renderer.
     */
    static std::string getShaderPath(const StandardShaderType& shaderType)
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

            case SG_PRIMITIVES_SHADER:
                switch(Main::CoreMain::getRenderer().getAPIType())
                {
                    case SG_API_TYPE_GL4:
                        shaderPath = SG_GLSL4_PRIMITIVES_SHADER_PATH;
                        break;
                }
                break;
        }

        return shaderPath;
    }
}

#endif //SUNGEARENGINE_GRAPHICSFILESRESOURCESMANAGER_H
