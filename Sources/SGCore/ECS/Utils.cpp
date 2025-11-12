//
// Created by stuka on 01.05.2025.
//

#include "Utils.h"

#include "Registry.h"
#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Decals/Decal.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Scene/EntityBaseInfo.h"

SGCore::ECS::entity_t SGCore::ECS::Utils::createDecal(registry_t& inRegistry, entity_t parentEntity) noexcept
{
    static auto mainAssetManager = AssetManager::getInstance();
    static auto planeMeshData = mainAssetManager->loadAsset<ModelAsset>("${enginePath}/Resources/models/standard/cube.obj")->m_rootNode->m_children[0]->m_meshesData[0];
    static auto planeMaterial = []() {
        auto mat = mainAssetManager->getOrAddAssetByAlias<IMaterial>("sg_decals_standard_material");
        mat->findAndAddTexture2D(SGTextureSlot::SGTT_DIFFUSE, "${enginePath}/Resources/textures/chess.jpg");
        mat->m_meshRenderState.m_useFacesCulling = false;

        return mat;
    }();

    const entity_t decalEntity = inRegistry.create();

    auto& decalBaseInfo = inRegistry.get<EntityBaseInfo>(decalEntity);
    decalBaseInfo.setParent(parentEntity, inRegistry);

    auto& decalTransform = inRegistry.emplace<Transform>(decalEntity, MakeRef<Transform>());
    // decalTransform->m_ownTransform.m_rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3 { 1.0f, 0.0f, 0.0f });

    auto& decalMesh = inRegistry.emplace<Mesh>(decalEntity);
    decalMesh.m_base.setMeshData(planeMeshData);
    decalMesh.m_base.setMaterial(planeMaterial);

    auto& decalDecal = inRegistry.emplace<Decal>(decalEntity);

    return decalEntity;
}
