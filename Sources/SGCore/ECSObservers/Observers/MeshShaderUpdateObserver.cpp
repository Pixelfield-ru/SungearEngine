//
// Created by Ilya on 05.02.2024.
//

#include "MeshShaderUpdateObserver.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Lighting/DirectionalLight.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ISubPassShader.h"

void SGCore::MeshShaderUpdateObserver::onComponentConstruct(entt::registry& registry, const entt::entity& entity) noexcept
{
    Mesh* mesh = registry.try_get<Mesh>(entity);

    if(mesh)
    {
        auto directionalLightsView = registry.view<DirectionalLight, RenderingBase, Transform>();

        for(auto& subPassShaderPair : mesh->m_base.m_meshData->m_material->getShader()->getSubPassesShaders())
        {
            auto& subPassShader = subPassShaderPair.second;

            subPassShader->bind();

            size_t dirLightsCount = 0;

            // setup directional lights
            directionalLightsView.each(
                    [&subPassShader, &dirLightsCount](DirectionalLight& directionalLight, RenderingBase& renderingBase, Transform& transform)
                    {
                        LightBase& lightBase = directionalLight.m_base;

                        std::string curDirLightName = "directionalLight[" + std::to_string(dirLightsCount) + "]";

                        subPassShader->useVectorf(curDirLightName + ".lightPart.color", lightBase.m_color);
                        subPassShader->useFloat(curDirLightName + ".lightPart.intensity", lightBase.m_intensity);

                        subPassShader->useMatrix(curDirLightName + ".lightPart.renderingPart.spaceMatrix",
                                                 renderingBase.m_spaceMatrix);
                        subPassShader->useVectorf(curDirLightName + ".lightPart.renderingPart.position",
                                                  transform.m_ownTransform.m_position);

                        ++dirLightsCount;
                    });

            subPassShader->useInteger("DIRECTIONAL_LIGHTS_COUNT", dirLightsCount);
        }
    }
}
