//
// Created by Ilya on 05.02.2024.
//

#include "MeshCreateObserver.h"
#include "SGCore/Render/Mesh.h"
#include "MeshShaderUpdateObserver.h"

void SGCore::MeshCreateObserver::onMeshCreate(entt::registry& registry, const entt::entity& entity) noexcept
{
    Mesh* createdMesh = registry.try_get<Mesh>(entity);

    // registry.
    // MeshShaderUpdateObserver
}