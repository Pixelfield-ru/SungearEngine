//
// Created by stuka on 27.11.2023.
//

#include "LinesGizmosPass.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/ECS/Rendering/Gizmos/LineGizmo.h"

void SGCore::LinesGizmosPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    m_shader->bind();

    SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRenderIn, camerasLayer, cameraEntity)
            Ref<Transform> cameraTransformComponent = cameraEntity.getComponent<Transform>();
            if(!cameraTransformComponent) continue;
            Ref<Camera> cameraComponent = cameraEntity.getComponent<Camera>();
            if(!cameraComponent) continue;

            CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);

            m_shader->useUniformBuffer(CoreMain::getRenderer().m_viewMatricesBuffer);

            for(auto& cachedPrimitivesLayer : *m_componentsToRender)
            {
                const auto& layer = cachedPrimitivesLayer.first;

                cameraComponent->bindPostProcessFrameBuffer(layer);

                for(auto& cachedGizmo : cachedPrimitivesLayer.second)
                {
                    auto& gizmoComponents = cachedGizmo.second;

                    Ref<Transform> linePrimitiveTransform = gizmoComponents.getComponent<Transform>();
                    if(!linePrimitiveTransform) continue;
                    std::list<Ref<LineGizmo>> lineGizmos =
                            gizmoComponents.getComponents<LineGizmo>();

                    for(const auto& lineGizmo : lineGizmos)
                    {
                        float x0, y0, z0, x1, y1, z1;

                        lineGizmo->m_meshData->getVertexPosition(0, x0, y0, z0);
                        lineGizmo->m_meshData->getVertexPosition(1, x1, y1, z1);

                        m_shader->useVectorf(
                                "u_verticesPositions[" + std::to_string(0) + "]",
                                x0, y0, z0);

                        m_shader->useVectorf(
                                "u_verticesPositions[" + std::to_string(1) + "]",
                                x1, y1, z1);

                        m_shader->useMatrix(
                                "u_primitiveModelMatrix", linePrimitiveTransform->m_modelMatrix
                        );

                        m_shader->useVectorf(
                                "u_offset", lineGizmo->m_offset
                        );

                        m_shader->useVectorf(
                                "u_color", lineGizmo->m_color
                        );

                        CoreMain::getRenderer().renderMeshData(
                                lineGizmo->m_meshData,
                                lineGizmo->m_meshDataRenderInfo
                        );
                    }
                }

                cameraComponent->unbindPostProcessFrameBuffer();
            }
    SG_END_ITERATE_CACHED_ENTITIES
}
