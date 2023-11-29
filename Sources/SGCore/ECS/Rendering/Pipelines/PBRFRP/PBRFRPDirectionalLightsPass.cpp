//
// Created by stuka on 26.11.2023.
//

#include "PBRFRPDirectionalLightsPass.h"
#include "SGCore/ECS/ECSUtils.h"
#include "PBRFRPGeometryPass.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCaster.h"
#include "SGCore/ECS/Scene.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsCollector.h"

void SGCore::PBRFRPDirectionalLightsPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    m_renderTimer.startFrame();

    m_renderTimerCallback->setUpdateFunction([&, renderPipeline]()
                                             {

                                                 auto geometryPass = renderPipeline->getRenderPass<PBRFRPGeometryPass>();

                                                 if(geometryPass || !geometryPass->m_active)
                                                 {
                                                     geometryPass->m_shader->bind();

                                                     // ------ updating all lights in geom shader -------------

                                                     size_t directionalLightsCount = 0;

                                                     SG_BEGIN_ITERATE_CACHED_ENTITIES(
                                                             *(scene->getSystem<DirectionalLightsCollector>()->m_componentsCollector.m_cachedEntities),
                                                             dirLightsLayer, directionalLightEntity)

                                                             auto directionalLightComponents =
                                                                     directionalLightEntity.getComponents<DirectionalLight>();

                                                             auto directionalLightTransform =
                                                                     directionalLightEntity.getComponent<Transform>();

                                                             if(!directionalLightTransform) continue;

                                                             for(const auto& directionalLightComponent: directionalLightComponents)
                                                             {
                                                                 std::string directionalLightString =
                                                                         "directionalLights[" +
                                                                         std::to_string(directionalLightsCount) + "]";

                                                                 geometryPass->m_shader->useVectorf(
                                                                         directionalLightString + ".color",
                                                                         directionalLightComponent->m_color
                                                                 );

                                                                 geometryPass->m_shader->useFloat(
                                                                         directionalLightString + ".intensity",
                                                                         directionalLightComponent->m_intensity
                                                                 );

                                                                 // todo: take into account the type of transformation and the direction of rotation
                                                                 geometryPass->m_shader->useVectorf(
                                                                         directionalLightString + ".position",
                                                                         directionalLightTransform->m_position
                                                                 );

                                                                 ++directionalLightsCount;
                                                             }
                                                     SG_END_ITERATE_CACHED_ENTITIES

                                                     // todo: make name as define
                                                     geometryPass->m_shader->useInteger("DIRECTIONAL_LIGHTS_COUNT",
                                                                                        directionalLightsCount
                                                     );
                                                 }
                                             }
    );
}
