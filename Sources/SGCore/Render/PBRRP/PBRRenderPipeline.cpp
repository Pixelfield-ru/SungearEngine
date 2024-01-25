//
// Created by stuka on 04.07.2023.
//

#include "PBRRenderPipeline.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/ICamera.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Rendering/Skybox.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "SGCore/ECS/Transformations/TransformationsUpdater.h"
#include "SGCore/ECS/Rendering/Gizmos/LineGizmo.h"
#include "SGCore/ECS/Rendering/Gizmos/IComplexGizmo.h"
#include "SGCore/ECS/Rendering/Pipelines/PostProcessFXPass.h"
#include "SGCore/ECS/Scene.h"
#include "SGCore/ECS/Rendering/CamerasCollector.h"
#include "PBRFRPDirectionalLightsCollector.h"
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

void SGCore::PBRRenderPipeline::update(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    if(m_prepareFunc)
    {
        m_prepareFunc();
    }

    auto thisShared = shared_from_this();

    for(auto& renderPass : m_renderPasses)
    {
        if(!renderPass->m_active) continue;

        renderPass->render(scene, thisShared);
    }
}

void SGCore::PBRRenderPipeline::useScene(const SGCore::Ref<SGCore::Scene>& scene)
{
    auto& cameras = scene->getSystem<CamerasCollector>()->m_componentsCollector.m_cachedEntities;

    auto& meshes = scene->getSystem<MeshesCollector>()->m_componentsCollector.m_cachedEntities;

    auto& dirLights = scene->getSystem<PBRFRPDirectionalLightsCollector>()->m_componentsCollector.m_cachedEntities;

    m_prepareFunc = [&, cameras]()
    {
        SG_BEGIN_ITERATE_CACHED_ENTITIES(
                *cameras,
                camerasLayer,
                cameraEntity)

                auto camera = cameraEntity.getComponent<ICamera>();

                if(camera)
                {
                    camera->clearPostProcessFrameBuffers();
                }
        SG_END_ITERATE_CACHED_ENTITIES
    };

    /*auto directionalLightsPass = getRenderPass<PBRRPDirectionalLightsPass>();
    directionalLightsPass->m_componentsToRenderIn = dirLights;
    directionalLightsPass->m_componentsToRender = meshes;*/

    auto geometryPass = getRenderPass<PBRRPGeometryPass>();
    geometryPass->m_componentsToRenderIn = cameras;
    geometryPass->m_componentsToRender = meshes;

    /*auto ppFXPass = getRenderPass<PostProcessFXPass>();
    ppFXPass->m_componentsToRenderIn = cameras;*/
}
