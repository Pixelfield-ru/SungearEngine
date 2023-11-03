//
// Created by stuka on 27.07.2023.
//

#include "ShadowsCasterSystem.h"
#include "ShadowsCasterComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Rendering/SkyboxComponent.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/MeshedEntitiesCollectorSystem.h"

void Core::ECS::ShadowsCasterSystem::fixedUpdate(const std::shared_ptr<Scene>& scene)
{
    // double t0 = glfwGetTime();

    /*const auto& meshedCachedEntities = Patterns::Singleton::getInstance<MeshedEntitiesCollectorSystem>()->getCachedEntities();

    if(meshedCachedEntities.empty()) return;

    size_t totalShadowCasters = 0;

    for(const auto& layer0 : m_cachedEntities)
    {
        for(const auto& cachedEntity: layer0.second)
        {
            if(!cachedEntity.second) continue;

            // todo: make process all ShadowsCasterComponent (cachedEntities.second->getComponents)
            std::shared_ptr<ShadowsCasterComponent> shadowsCasterComponent = cachedEntity.second->getComponent<ShadowsCasterComponent>();
            std::shared_ptr<TransformComponent> shadowsCasterTransform = cachedEntity.second->getComponent<TransformComponent>();

            if(!shadowsCasterTransform || !shadowsCasterComponent) continue;

            shadowsCasterComponent->m_frameBuffer->bind()->clear();
            Core::Main::CoreMain::getRenderer().prepareUniformBuffers(shadowsCasterComponent, nullptr);

            for(const auto& layer1 : meshedCachedEntities)
            {
                for(const auto& meshedEntity: layer1.second)
                {
                    if(!meshedEntity.second) continue;

                    auto entityTransformComponent = meshedEntity.second->getComponent<TransformComponent>();

                    auto meshComponents =
                            meshedEntity.second->getComponents<MeshComponent>();

                    if(!entityTransformComponent || meshComponents.empty()) continue;

                    for(const auto& meshComponent: meshComponents)
                    {
                        const auto& materialShader = meshComponent->m_mesh->m_material->getCurrentShader();

                        if(!materialShader) return;

                        std::uint8_t shadowMapsBlockOffset = meshComponent->
                                m_mesh->
                                m_material->getBlocks()[SGTextureType::SGTP_SHADOW_MAP].m_texturesUnitOffset;

                        std::uint8_t finalTextureBlock = shadowMapsBlockOffset + totalShadowCasters;

                        shadowsCasterComponent->m_frameBuffer->bindAttachment(
                                SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT,
                                finalTextureBlock
                        );

                        std::string totalShadowsCastersStr = std::to_string(totalShadowCasters);

                        materialShader->bind();

                        materialShader->useTexture(
                                "shadowsCastersShadowMaps[" + totalShadowsCastersStr + "]",
                                finalTextureBlock
                        );

                        // todo: maybe make uniform buffer for shadows casters
                        if(shadowsCasterComponent->m_spaceMatrixChanged)
                        {
                            materialShader->useMatrix(
                                    "shadowsCasters[" + totalShadowsCastersStr + "].shadowsCasterSpace",
                                    shadowsCasterComponent->m_spaceMatrix
                            );
                        }

                        if(shadowsCasterTransform->m_positionChanged)
                        {
                            materialShader->useVectorf(
                                    "shadowsCasters[" + totalShadowsCastersStr + "].position",
                                    shadowsCasterTransform->m_position
                            );
                        }

                        //auto lastFacesCullingType = meshComponent->m_facesCullingFaceType;
                        //auto lastFacesCullingPolygonsOrder = meshComponent->m_facesCullingPolygonsOrder;
                        meshComponent->m_mesh->m_material->setCurrentShader(SGMAT_SHADOW_GEN_SHADER_NAME);
                        //meshComponent->m_facesCullingFaceType = SGFaceType::SGG_FRONT_FACE;
                        //meshComponent->m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;

                        Core::Main::CoreMain::getRenderer().renderMesh(entityTransformComponent, meshComponent);

                        meshComponent->m_mesh->m_material->setCurrentShader(SGMAT_STANDARD_SHADER_NAME);
                        //meshComponent->m_facesCullingFaceType = lastFacesCullingType;
                        //meshComponent->m_facesCullingPolygonsOrder = lastFacesCullingPolygonsOrder;
                    }
                }
            }

            shadowsCasterComponent->m_frameBuffer->bind()->unbind();

            totalShadowCasters++;
        }
    }*/

    // double t1 = glfwGetTime();

    // 13,601600
    // 9,531500 current
    // std::cout << "ms for shadows caster: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::ShadowsCasterSystem::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity)
{
    cacheEntityComponents<ShadowsCasterComponent, TransformComponent>(entity);
}

