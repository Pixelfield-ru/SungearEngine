//
// Created by Ilya on 25.11.2023.
//

#include "PBRFRPSkyboxesPass.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/Main/CoreMain.h"

void SGCore::PBRFRPSkyboxesPass::render(const Ref<IRenderPipeline>& renderPipeline)
{
    m_shader->bind();
    m_shader->useShaderMarkup(m_shaderMarkup);

    SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRenderIn, camerasLayer, cameraEntity)
            auto cameraComponent = cameraEntity.getComponent<Camera>();
            if(!cameraComponent) continue;
            auto cameraTransformComponent = cameraEntity.getComponent<Transform>();
            if(!cameraTransformComponent) continue;

            CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);
            m_shader->useUniformBuffer(CoreMain::getRenderer().m_viewMatricesBuffer);

            for(auto& skyboxesLayerPair : *m_componentsToRender)
            {
                const auto& layer = skyboxesLayerPair.first;

                cameraComponent->bindPostProcessFrameBuffer(layer,
                                                            SGG_COLOR_ATTACHMENT0);

                for(auto& skyboxEntityPair : skyboxesLayerPair.second)
                {
                    auto& skyboxComponents = skyboxEntityPair.second;

                    Ref<Transform> transformComponent = skyboxComponents.getComponent<Transform>();

                    if(!transformComponent) continue;

                    auto meshComponents =
                            skyboxComponents.getComponents<Mesh>();

                    for(const auto& meshComponent: meshComponents)
                    {
                        meshComponent->m_meshData->m_material->bind(m_shader, m_shaderMarkup);
                        m_shader->useMatrix("objectModelMatrix",
                                          transformComponent->m_modelMatrix
                        );

                        CoreMain::getRenderer().renderMeshData(
                                meshComponent->m_meshData,
                                meshComponent->m_meshDataRenderInfo
                        );
                    }
                }

                cameraComponent->unbindPostProcessFrameBuffer();
            }

    SG_END_ITERATE_CACHED_ENTITIES
}
