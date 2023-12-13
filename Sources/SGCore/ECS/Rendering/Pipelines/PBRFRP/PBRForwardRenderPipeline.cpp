//
// Created by stuka on 04.07.2023.
//

#include "PBRForwardRenderPipeline.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Rendering/Skybox.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "SGCore/ECS/Transformations/TransformationsUpdater.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsCollector.h"
#include "SGCore/ECS/Rendering/SkyboxesCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/LinesGizmosCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/ComplexGizmosCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/LineGizmo.h"
#include "SGCore/ECS/Rendering/Gizmos/IComplexGizmo.h"
#include "SGCore/ECS/Rendering/Pipelines/PBRFRP/PBRFRPSkyboxesPass.h"
#include "SGCore/ECS/Rendering/CamerasCollector.h"
#include "PBRFRPGeometryPass.h"
#include "SGCore/ECS/Rendering/Pipelines/PostProcessFXPass.h"
#include "SGCore/ECS/Rendering/Pipelines/ComplexGizmosPass.h"
#include "PBRFRPDirectionalLightsPass.h"
#include "SGCore/ECS/Rendering/Pipelines/LinesGizmosPass.h"
#include "SGCore/ECS/Scene.h"

SGCore::PBRForwardRenderPipeline::PBRForwardRenderPipeline()
{
    auto& shadersPaths = *SGSingleton::getSharedPtrInstance<ShadersPaths>();

    // configure render passes --------
    {
        auto skyboxesPass = MakeRef<PBRFRPSkyboxesPass>();

        skyboxesPass->m_shader = Ref<IShader>(
                CoreMain::getRenderer().createShader(
                        shadersPaths["Skybox"]["DefaultShader"]
                )
        );

        skyboxesPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_SKYBOX,
                sgStandardTextureTypeToString(SGTextureType::SGTP_SKYBOX),
                1
        );

        skyboxesPass->m_shaderMarkup.calculateBlocksOffsets();

        m_renderPasses.push_back(skyboxesPass);
    }

    {
        auto directionalLightsPass = MakeRef<PBRFRPDirectionalLightsPass>();

        directionalLightsPass->m_shader = Ref<IShader>(
                CoreMain::getRenderer().createShader(
                        shadersPaths["ShadowsGeneration"]["DefaultShader"]
                )
        );

        directionalLightsPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_DIFFUSE,
                sgStandardTextureTypeToString(SGTextureType::SGTP_DIFFUSE),
                1
        );

        directionalLightsPass->m_shaderMarkup.calculateBlocksOffsets();

        m_renderPasses.push_back(directionalLightsPass);
    }

    {
        auto geometryPass = MakeRef<PBRFRPGeometryPass>();

        geometryPass->m_shader = Ref<IShader>(
                CoreMain::getRenderer().createShader(
                        shadersPaths["PBR"]["DefaultShader"]
                )
        );

        geometryPass->m_shader->m_uniformBuffer = Scope<IUniformBuffer>(CoreMain::getRenderer().createUniformBuffer());

        geometryPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_DIFFUSE,
                sgStandardTextureTypeToString(SGTextureType::SGTP_DIFFUSE),
                1
        );
        geometryPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_DIFFUSE_ROUGHNESS,
                sgStandardTextureTypeToString(SGTextureType::SGTP_DIFFUSE_ROUGHNESS),
                1
        );
        geometryPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_NORMALS,
                sgStandardTextureTypeToString(SGTextureType::SGTP_NORMALS),
                1
        );
        geometryPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_METALNESS,
                sgStandardTextureTypeToString(SGTextureType::SGTP_METALNESS),
                1
        );
        geometryPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_LIGHTMAP,
                sgStandardTextureTypeToString(SGTextureType::SGTP_LIGHTMAP),
                1
        );
        geometryPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_SHADOW_MAP2D,
                sgStandardTextureTypeToString(SGTextureType::SGTP_SHADOW_MAP2D),
                5,
                false
        );

        geometryPass->m_shaderMarkup.calculateBlocksOffsets();

        m_renderPasses.push_back(geometryPass);
    }

    {
        auto linesGizmosPass = MakeRef<LinesGizmosPass>();

        linesGizmosPass->m_shader = Ref<IShader>(
                CoreMain::getRenderer().createShader(
                        shadersPaths["Gizmos"]["LinesGizmosShader"]
                )
        );

        m_renderPasses.push_back(linesGizmosPass);
    }

    {
        auto complexGizmosPass = MakeRef<ComplexGizmosPass>();

        complexGizmosPass->m_shader = Ref<IShader>(
                CoreMain::getRenderer().createShader(
                        shadersPaths["Gizmos"]["ComplexGizmosShader"]
                )
        );

        m_renderPasses.push_back(complexGizmosPass);
    }

    // WARNING! PostProcessFXPass ALWAYS AFTER OTHER PASSES
    {
        auto postProcessFXPass = MakeRef<PostProcessFXPass>();

        m_renderPasses.push_back(postProcessFXPass);
    }
}

void SGCore::PBRForwardRenderPipeline::useScene(const SGCore::Ref<SGCore::Scene>& scene)
{
    auto& cameras = scene->getSystem<CamerasCollector>()->m_componentsCollector.m_cachedEntities;

    auto& meshes = scene->getSystem<MeshesCollector>()->m_componentsCollector.m_cachedEntities;

    auto& linesGizmos = scene->getSystem<LinesGizmosCollector>()->m_componentsCollector.m_cachedEntities;

    auto& complexGizmos = scene->getSystem<ComplexGizmosCollector>()->m_componentsCollector.m_cachedEntities;

    auto& skyboxes = scene->getSystem<SkyboxesCollector>()->m_componentsCollector.m_cachedEntities;

    auto& dirLights = scene->getSystem<DirectionalLightsCollector>()->m_componentsCollector.m_cachedEntities;

    m_prepareFunc = [&, cameras]()
    {
        SG_BEGIN_ITERATE_CACHED_ENTITIES(
                *cameras,
                camerasLayer,
                cameraEntity)

                auto camera = cameraEntity.getComponent<Camera>();

                if(camera)
                {
                    camera->clearPostProcessFrameBuffers();
                }
        SG_END_ITERATE_CACHED_ENTITIES
    };

    auto directionalLightsPass = getRenderPass<PBRFRPDirectionalLightsPass>();
    directionalLightsPass->m_componentsToRenderIn = dirLights;
    directionalLightsPass->m_componentsToRender = meshes;

    auto skyboxesPass = getRenderPass<PBRFRPSkyboxesPass>();
    skyboxesPass->m_componentsToRenderIn = cameras;
    skyboxesPass->m_componentsToRender = skyboxes;

    auto geometryPass = getRenderPass<PBRFRPGeometryPass>();
    geometryPass->m_componentsToRenderIn = cameras;
    geometryPass->m_componentsToRender = meshes;

    auto linesGizmosPass = getRenderPass<LinesGizmosPass>();
    linesGizmosPass->m_componentsToRenderIn = cameras;
    linesGizmosPass->m_componentsToRender = linesGizmos;

    auto complexGizmosPass = getRenderPass<ComplexGizmosPass>();
    complexGizmosPass->m_componentsToRenderIn = cameras;
    complexGizmosPass->m_componentsToRender = complexGizmos;

    auto ppFXPass = getRenderPass<PostProcessFXPass>();
    ppFXPass->m_componentsToRenderIn = cameras;
}
