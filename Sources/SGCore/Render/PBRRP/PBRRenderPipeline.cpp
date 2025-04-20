//
// Created by stuka on 04.07.2023.
//

#include "PBRRenderPipeline.h"
#include "PBRRPGeometryPass.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Render/PostProcess/PostProcessPass.h"
#include "SGCore/Render/BaseRenderPasses/OutlinePass.h"
#include "PBRRPDirectionalLightsPass.h"
#include "SGCore/Render/DebugDraw.h"
#include "SGCore/Render/TextRenderPass.h"
#include "SGCore/Render/BaseRenderPasses/UIRenderPass.h"
#include "SGCore/Render/PostProcess/PostProcessBuffersClearPass.h"

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

    m_shadersPaths.addPath(
            "StandardTerrainShader",
            "${enginePath}/Resources/sg_shaders/features/terrain.sgshader"
    );
    
    // configure render passes --------

    /*{
        auto directionalLightsPass = MakeRef<PBRRPDirectionalLightsPass>();

        m_renderPasses.push_back(directionalLightsPass);
    }*/

    {
        auto postProcessBuffersClearPass = MakeRef<PostProcessBuffersClearPass>();

        // geometryPass->m_shader->m_uniformBuffer = Scope<IUniformBuffer>(CoreMain::getRenderer().createUniformBuffer());

        m_renderPasses.push_back(postProcessBuffersClearPass);
    }

    {
        auto geometryPass = MakeRef<PBRRPGeometryPass>();

        // geometryPass->m_shader->m_uniformBuffer = Scope<IUniformBuffer>(CoreMain::getRenderer().createUniformBuffer());

        m_renderPasses.push_back(geometryPass);
    }

    {
        auto debugDrawPass = MakeRef<DebugDraw>();

        // geometryPass->m_shader->m_uniformBuffer = Scope<IUniformBuffer>(CoreMain::getRenderer().createUniformBuffer());

        m_renderPasses.push_back(debugDrawPass);
    }

    {
        auto directionalLightsPass = MakeRef<PBRRPDirectionalLightsPass>();

        m_renderPasses.push_back(directionalLightsPass);
    }
    
    {
        auto textRenderPass = MakeRef<TextRenderPass>();
        
        m_renderPasses.push_back(textRenderPass);
    }

    // WARNING! UIRenderPass ALWAYS AFTER OTHER GEOMETRY PASS
    {
        auto uiPass = MakeRef<UIRenderPass>();

        m_renderPasses.push_back(uiPass);
    }

    // WARNING! PostProcessFXPass ALWAYS AFTER OTHER PASSES
    {
        auto postProcessFXPass = MakeRef<PostProcessPass>();

        m_renderPasses.push_back(postProcessFXPass);
    }

    // WARNING! THIS PASS IS ALWAYS AFTER POST PROCESS PASS
    {
        auto outlinePass = MakeRef<OutlinePass>();

        m_renderPasses.push_back(outlinePass);
    }
}

