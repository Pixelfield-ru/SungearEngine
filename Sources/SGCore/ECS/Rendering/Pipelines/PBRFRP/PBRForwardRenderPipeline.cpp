//
// Created by stuka on 04.07.2023.
//

#include "PBRForwardRenderPipeline.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Rendering/Skybox.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "SGCore/ECS/Transformations/TransformationsUpdater.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsCollector.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCastersCollector.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCaster.h"
#include "SGCore/ECS/Rendering/SkyboxesCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/LinesGizmosCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/ComplexGizmosCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/LineGizmo.h"
#include "SGCore/ECS/Rendering/Gizmos/IComplexGizmo.h"
#include "SGCore/ECS/Rendering/Pipelines/PBRFRP/PBRFRPSkyboxesPass.h"
#include "SGCore/ECS/Rendering/CamerasCollector.h"
#include "PBRFRPShadowsPass.h"
#include "PBRFRPGeometryPass.h"
#include "SGCore/ECS/Rendering/Pipelines/PostProcessFXPass.h"
#include "SGCore/ECS/Rendering/Pipelines/ComplexGizmosPass.h"
#include "PBRFRPDirectionalLightsPass.h"
#include "SGCore/ECS/Rendering/Pipelines/LinesGizmosPass.h"

SGCore::PBRForwardRenderPipeline::PBRForwardRenderPipeline()
{
    auto& cameras = SGSingleton::getSharedPtrInstance<CamerasCollector>()->m_componentsCollector.m_cachedEntities;
    auto& shadowsCasters = SGSingleton::getSharedPtrInstance<ShadowsCastersCollector>()->m_componentsCollector.m_cachedEntities;

    auto& meshes = SGSingleton::getSharedPtrInstance<MeshesCollector>()->m_componentsCollector.m_cachedEntities;

    auto& linesGizmos = SGSingleton::getSharedPtrInstance<LinesGizmosCollector>()->m_componentsCollector.m_cachedEntities;

    auto& complexGizmos = SGSingleton::getSharedPtrInstance<ComplexGizmosCollector>()->m_componentsCollector.m_cachedEntities;

    m_prepareFunc = [&, cameras, shadowsCasters]()
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

    // -----------------------------

    {
        auto directionalLightsPass = MakeRef<PBRFRPDirectionalLightsPass>();

        m_renderPasses.push_back(directionalLightsPass);
    }

    // configure render passes --------
    {
        auto skyboxesPass = MakeRef<PBRFRPSkyboxesPass>();

        skyboxesPass->m_shader = Ref<IShader>(
                CoreMain::getRenderer().createShader(
                        ShadersPaths::getMainInstance()["Skybox"]["DefaultShader"]
                )
        );

        skyboxesPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_SKYBOX,
                sgStandardTextureTypeToString(SGTextureType::SGTP_SKYBOX),
                1
        );

        skyboxesPass->m_shaderMarkup.calculateBlocksOffsets();

        skyboxesPass->m_componentsToRenderIn = cameras;
        skyboxesPass->m_componentsToRender =
                SGSingleton::getSharedPtrInstance<SkyboxesCollector>()->m_componentsCollector.m_cachedEntities;

        m_renderPasses.push_back(skyboxesPass);
    }

    {
        auto shadowsPass = MakeRef<PBRFRPShadowsPass>();

        shadowsPass->m_shader = Ref<IShader>(
                CoreMain::getRenderer().createShader(
                        ShadersPaths::getMainInstance()["ShadowsGeneration"]["DefaultShader"]
                )
        );

        shadowsPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_DIFFUSE,
                sgStandardTextureTypeToString(SGTextureType::SGTP_DIFFUSE),
                1
        );

        shadowsPass->m_shaderMarkup.calculateBlocksOffsets();

        shadowsPass->m_componentsToRenderIn = shadowsCasters;
        shadowsPass->m_componentsToRender = meshes;

        m_renderPasses.push_back(shadowsPass);
    }

    {
        auto geometryPass = MakeRef<PBRFRPGeometryPass>();

        geometryPass->m_shader = Ref<IShader>(
                CoreMain::getRenderer().createShader(
                        ShadersPaths::getMainInstance()["PBR"]["DefaultShader"]
                )
        );

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
                SGTextureType::SGTP_BASE_COLOR,
                sgStandardTextureTypeToString(SGTextureType::SGTP_BASE_COLOR),
                1
        );
        geometryPass->m_shaderMarkup.addTexturesBlockDeclaration(
                SGTextureType::SGTP_SHADOW_MAP,
                sgStandardTextureTypeToString(SGTextureType::SGTP_SHADOW_MAP),
                5,
                false
        );

        geometryPass->m_shaderMarkup.calculateBlocksOffsets();

        geometryPass->m_componentsToRenderIn = cameras;
        geometryPass->m_componentsToRender = meshes;

        m_renderPasses.push_back(geometryPass);
    }

    {
        auto linesGizmosPass = MakeRef<LinesGizmosPass>();

        linesGizmosPass->m_shader = Ref<IShader>(
                CoreMain::getRenderer().createShader(
                        ShadersPaths::getMainInstance()["Gizmos"]["LinesGizmosShader"]
                )
        );

        linesGizmosPass->m_componentsToRenderIn = cameras;
        linesGizmosPass->m_componentsToRender = linesGizmos;

        m_renderPasses.push_back(linesGizmosPass);
    }

    {
        auto complexGizmosPass = MakeRef<ComplexGizmosPass>();

        complexGizmosPass->m_shader = Ref<IShader>(
                CoreMain::getRenderer().createShader(
                        ShadersPaths::getMainInstance()["Gizmos"]["ComplexGizmosShader"]
                )
        );

        complexGizmosPass->m_componentsToRenderIn = cameras;
        complexGizmosPass->m_componentsToRender = complexGizmos;

        m_renderPasses.push_back(complexGizmosPass);
    }

    // WARNING! PostProcessFXPass ALWAYS AFTER OTHER PASSES
    {
        auto postProcessFXPass = MakeRef<PostProcessFXPass>();

        postProcessFXPass->m_componentsToRenderIn = cameras;

        m_renderPasses.push_back(postProcessFXPass);
    }
}
