//
// Created by stuka on 25.11.2023.
//

#include "PBRFRPGeometryPass.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ECS/Rendering/Mesh.h"

void SGCore::PBRFRPGeometryPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    m_shader->bind();
    m_shader->useShaderMarkup(m_shaderMarkup);

    SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRenderIn, camerasLayer, cameraEntity)
            auto cameraTransformComponent = cameraEntity.getComponent<Transform>();
            if(!cameraTransformComponent) continue;
            auto cameraComponent = cameraEntity.getComponent<Camera>();
            if(!cameraComponent) continue;

            CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);
            m_shader->useUniformBuffer(CoreMain::getRenderer().m_viewMatricesBuffer);

            // todo: make less bindings

            for(auto& meshesLayer : *m_componentsToRender)
            {
                const auto& layer = meshesLayer.first;

                cameraComponent->bindPostProcessFrameBuffer(layer,SGG_COLOR_ATTACHMENT0);

                for(auto& meshesEntity: meshesLayer.second)
                {
                    Ref<Transform> transformComponent = meshesEntity.second.getComponent<Transform>();

                    if(!transformComponent) continue;

                    auto meshComponents =
                            meshesEntity.second.getComponents<Mesh>();

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
