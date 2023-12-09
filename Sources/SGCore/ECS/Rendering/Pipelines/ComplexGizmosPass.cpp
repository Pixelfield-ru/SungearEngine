//
// Created by stuka on 26.11.2023.
//

#include "ComplexGizmosPass.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/ECS/Rendering/Gizmos/IComplexGizmo.h"

void SGCore::ComplexGizmosPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
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

                    Ref<Transform> complexPrimitiveTransform = gizmoComponents.getComponent<Transform>();
                    if(!complexPrimitiveTransform) continue;
                    std::list<Ref<IComplexGizmo>> complexPrimitiveComponents =
                            gizmoComponents.getComponents<IComplexGizmo>();

                    for(const auto& complexPrimitiveComponent: complexPrimitiveComponents)
                    {
                        m_shader->useMatrix(
                                "u_primitiveModelMatrix", complexPrimitiveTransform->m_modelMatrix
                        );

                        m_shader->useVectorf(
                                "u_offset", complexPrimitiveComponent->m_offset
                        );

                        m_shader->useVectorf(
                                "u_color", complexPrimitiveComponent->m_color
                        );

                        CoreMain::getRenderer().renderMeshData(
                                complexPrimitiveComponent->m_meshData,
                                complexPrimitiveComponent->m_meshDataRenderInfo
                        );
                    }
                }

                cameraComponent->unbindPostProcessFrameBuffer();
            }
    SG_END_ITERATE_CACHED_ENTITIES
}
