//
// Created by stuka on 04.07.2023.
//

#include "PBRRenderPipeline.h"
#include "SGCore/Main/CoreMain.h"

#include "PBRRPGeometryPass.h"
#include "SGCore/Render/PostProcess/PostProcessPass.h"
#include "PBRRPDirectionalLightsPass.h"
#include "SGCore/Render/TextRenderPass.h"

SGCore::PBRRenderPipeline::PBRRenderPipeline()
{
    m_shadersPaths["StandardMeshShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/pbr/default_shader.glsl";
    m_shadersPaths["LayeredPostProcessingShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/postprocessing/default_shader.glsl";
    m_shadersPaths["ShadowsGenerationShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/shadows_generator/default_shader.glsl";
    m_shadersPaths["SkyboxShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/skybox/default_shader.glsl";
    m_shadersPaths["Gizmos"]["ComplexGizmosShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/gizmos/complex_gizmos_shader.glsl";
    m_shadersPaths["Gizmos"]["LinesGizmosShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/gizmos/lines_shader.glsl";
    m_shadersPaths["LinesDebugDrawShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/debug/lines_debug_shader.glsl";
    m_shadersPaths["StandardTextShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/ui/text_shader.glsl";
    m_shadersPaths["InstancingShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/pbr/instancing.glsl";
    m_shadersPaths["BatchingShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/pbr/batching.glsl";
    
    // configure render passes --------

    /*{
        auto directionalLightsPass = MakeRef<PBRRPDirectionalLightsPass>();

        m_renderPasses.push_back(directionalLightsPass);
    }*/

    {
        auto geometryPass = MakeRef<PBRRPGeometryPass>();

        // geometryPass->m_shader->m_uniformBuffer = Scope<IUniformBuffer>(CoreMain::getRenderer().createUniformBuffer());

        m_renderPasses.push_back(geometryPass);
    }

    {
        auto directionalLightsPass = MakeRef<PBRRPDirectionalLightsPass>();

        m_renderPasses.push_back(directionalLightsPass);
    }
    
    {
        auto textRenderPass = MakeRef<TextRenderPass>();
        
        m_renderPasses.push_back(textRenderPass);
    }

    // WARNING! PostProcessFXPass ALWAYS AFTER OTHER PASSES
    {
        auto postProcessFXPass = MakeRef<PostProcessPass>();

        m_renderPasses.push_back(postProcessFXPass);
    }
}

