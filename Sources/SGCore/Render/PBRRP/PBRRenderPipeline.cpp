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
    m_shadersPaths.addPath(
            "StandardMeshShader",
            "${enginePath}/Resources/sg_shaders/features/pbr/pbr.sgshader"
    );

    m_shadersPaths.addPath(
            "SkyboxShader",
            "${enginePath}/Resources/sg_shaders/features/skybox/skybox.sgshader"
    );

    m_shadersPaths.addPath(
            "Gizmos/ComplexGizmosShader",
            "${enginePath}/Resources/sg_shaders/features/gizmos/complex_gizmos.sgshader"
    );

    m_shadersPaths.addPath(
            "Gizmos/LinesGizmosShader",
            "${enginePath}/Resources/sg_shaders/features/gizmos/lines.sgshader"
    );

    m_shadersPaths.addPath(
            "LinesDebugDrawShader",
            "${enginePath}/Resources/sg_shaders/features/debug/debug.sgshader"
    );

    m_shadersPaths.addPath(
            "StandardTextShader",
            "${enginePath}/Resources/sg_shaders/features/ui/text_shader.sgshader"
    );

    m_shadersPaths.addPath(
            "InstancingShader",
            "${enginePath}/Resources/sg_shaders/features/pbr/instancing.sgshader"
    );

    m_shadersPaths.addPath(
            "BatchingShader",
            "${enginePath}/Resources/sg_shaders/features/pbr/batching.sgshader"
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

