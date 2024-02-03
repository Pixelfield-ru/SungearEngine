//
// Created by stuka on 04.07.2023.
//

#include "PBRRenderPipeline.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/ECS/Rendering/Skybox.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/LineGizmo.h"
#include "SGCore/ECS/Rendering/Gizmos/IComplexGizmo.h"
#include "SGCore/ECS/Rendering/Pipelines/PostProcessFXPass.h"
#include "SGCore/ECS/Rendering/CamerasCollector.h"
#include "PBRRPDirectionalLightsPass.h"
#include "PBRRPGeometryPass.h"

SGCore::PBRRenderPipeline::PBRRenderPipeline()
{
    m_shadersPaths["StandardMeshShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/pbr/default_shader.glsl";
    m_shadersPaths["PostProcessingShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/postprocessing/default_shader.glsl";
    m_shadersPaths["ShadowsGenerationShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/shadows_generator/default_shader.glsl";
    m_shadersPaths["SkyboxShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/skybox/default_shader.glsl";
    m_shadersPaths["Gizmos"]["ComplexGizmosShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/gizmos/complex_gizmos_shader.glsl";
    m_shadersPaths["Gizmos"]["LinesGizmosShader"].m_GLSL4RealizationPath = "../SGResources/shaders/glsl4/gizmos/lines_shader.glsl";

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

    // WARNING! PostProcessFXPass ALWAYS AFTER OTHER PASSES
    /*{
        auto postProcessFXPass = MakeRef<PostProcessFXPass>();

        m_renderPasses.push_back(postProcessFXPass);
    }*/
}

