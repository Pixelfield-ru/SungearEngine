//
// Created by stuka on 04.02.2024.
//

#include "ModelMatrixChangedObserver.h"
#include "SGUtils/TypeInfo.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ISubPassShader.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Flags/ModelMatrixChangedFlag.h"

void SGCore::ModelMatrixChangedObserver::onFlagChanged(const SGCore::Ref<SGCore::Scene>& scene,
                                                       const entt::entity& flagOwner,
                                                       const size_t& flagTypeID) noexcept
{
    if(flagTypeID == SGUtils::TypeID<ModelMatrixChangedFlag>::id())
    {
        entt::registry& registry = scene->getECSRegistry();

        Mesh* mesh = registry.try_get<Mesh>(flagOwner);
        Transform* transform = registry.try_get<Transform>(flagOwner);

        if(mesh && transform)
        {
            Ref<ISubPassShader> geomPassShader = mesh->m_base.m_meshData->m_material->getShader()->getSubPassShader("GeometryPass");
            if(geomPassShader)
            {
                std::cout << "updated" << std::endl;

                geomPassShader->bind();
                geomPassShader->useMatrix("objectModelMatrix", transform->m_finalTransform.m_modelMatrix);
            }
        }
    }
}
