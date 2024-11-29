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
    m_shadersPaths.addShader(
            "StandardMeshShader",
            "${enginePath}/Resources/sg_shaders/glsl4/pbr/default_shader.glsl",
            ShadersUniversalPaths::undefined_path,
            ShadersUniversalPaths::undefined_path
    );

    m_shadersPaths.addShader(
            "LayeredPostProcessingShader",
            "${enginePath}/Resources/sg_shaders/glsl4/postprocessing/default_shader.glsl",
            ShadersUniversalPaths::undefined_path,
            ShadersUniversalPaths::undefined_path
    );

    m_shadersPaths.addShader(
            "ShadowsGenerationShader",
            "${enginePath}/Resources/sg_shaders/glsl4/shadows_generator/default_shader.glsl",
            ShadersUniversalPaths::undefined_path,
            ShadersUniversalPaths::undefined_path
    );

    m_shadersPaths.addShader(
            "SkyboxShader",
            "${enginePath}/Resources/sg_shaders/glsl4/skybox/default_shader.glsl",
            ShadersUniversalPaths::undefined_path,
            ShadersUniversalPaths::undefined_path
    );

    m_shadersPaths.addShader(
            "Gizmos/ComplexGizmosShader",
            "${enginePath}/Resources/sg_shaders/glsl4/gizmos/complex_gizmos_shader.glsl",
            ShadersUniversalPaths::undefined_path,
            ShadersUniversalPaths::undefined_path
    );

    m_shadersPaths.addShader(
            "Gizmos/LinesGizmosShader",
            "${enginePath}/Resources/sg_shaders/glsl4/gizmos/lines_shader.glsl",
            ShadersUniversalPaths::undefined_path,
            ShadersUniversalPaths::undefined_path
    );

    m_shadersPaths.addShader(
            "LinesDebugDrawShader",
            "${enginePath}/Resources/sg_shaders/glsl4/debug/lines_debug_shader.glsl",
            ShadersUniversalPaths::undefined_path,
            ShadersUniversalPaths::undefined_path
    );

    m_shadersPaths.addShader(
            "StandardTextShader",
            "${enginePath}/Resources/sg_shaders/glsl4/ui/text_shader.glsl",
            ShadersUniversalPaths::undefined_path,
            ShadersUniversalPaths::undefined_path
    );

    m_shadersPaths.addShader(
            "InstancingShader",
            "${enginePath}/Resources/sg_shaders/glsl4/pbr/instancing.glsl",
            ShadersUniversalPaths::undefined_path,
            ShadersUniversalPaths::undefined_path
    );

    m_shadersPaths.addShader(
            "BatchingShader",
            "${enginePath}/Resources/sg_shaders/glsl4/pbr/batching.glsl",
            ShadersUniversalPaths::undefined_path,
            ShadersUniversalPaths::undefined_path
    );
    
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

