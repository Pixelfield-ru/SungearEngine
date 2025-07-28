//
// Created by stuka on 28.07.2025.
//

#include "AutoInstancing.h"

#include "Instancing.h"
#include "SGCore/ECS/Registry.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/Mesh.h"

SGCore::ECS::entity_t SGCore::AutoInstancing::addEntity(const ECS::entity_t& entity,
                                                        ECS::registry_t& inRegistry) noexcept
{
    const auto* tmpEntityMesh = inRegistry.tryGet<Mesh>(entity);
    if(!tmpEntityMesh || !tmpEntityMesh->m_base.getMeshData()) return entt::null;

    const auto& entityMesh = *tmpEntityMesh;

    const auto instancingView = inRegistry.view<Instancing>();

    for(auto e : instancingView)
    {
        auto& instancing = instancingView[e];

        if(instancing.getBaseMeshData()->getHash() == entityMesh.m_base.getMeshData()->getHash() &&
           instancing.getBaseMaterial()->getHash() == entityMesh.m_base.getMaterial()->getHash())
        {
            instancing.m_entities.push_back(entity);
            return e;
        }
    }

    const auto newInstancingEntity = inRegistry.create();

    auto& newInstancing = inRegistry.emplace<Instancing>(newInstancingEntity);

    newInstancing.setBaseMeshData(entityMesh.m_base.getMeshData());
    newInstancing.setBaseMaterial(entityMesh.m_base.getMaterial());

    newInstancing.m_entities.push_back(entity);

    return newInstancingEntity;
}
