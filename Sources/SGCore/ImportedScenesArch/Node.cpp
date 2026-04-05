//
// Created by stuka on 27.11.2023.
//

#include "Node.h"

#include <glm/gtc/quaternion.hpp>

#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/SpacePartitioning/OctreeCullable.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Alpha/OpaqueEntityTag.h"
#include "SGCore/Render/RenderAbilities/EnableMeshPass.h"
#include "SGCore/Transformations/TransformUtils.h"

SGCore::ECS::entity_t SGCore::Node::addOnScene(const Ref<Scene>& scene,
                                               const entity_func& eachEntityFunc,
                                               const mesh_func& meshFunc,
                                               const bool& rootAdd) const noexcept
{
    auto registry = scene->getECSRegistry();

    ECS::entity_t parentEntity = registry->create();

    registry->emplace<Pickable>(parentEntity);
    auto& nodeBaseInfo = registry->get<EntityBaseInfo>(parentEntity);
    auto nodeTransform = registry->emplace<Transform>(parentEntity, MakeRef<Transform>());
    nodeTransform->m_localTransform.m_position = m_position;
    // auto eulerRot = glm::eulerAngles(m_rotationQuaternion);
    nodeTransform->m_localTransform.m_rotation = m_rotationQuaternion;
    nodeTransform->m_localTransform.m_scale = m_scale;
    nodeTransform->m_initialModelMatrix = TransformUtils::calculateModelMatrix(m_position, m_rotationQuaternion, m_scale);

    nodeBaseInfo.setRawName(m_name);

    if(rootAdd)
    {
        // scene->addEntity(parentEntity, layerName);
    }
    else
    {
        // parentEntity->setParentScene(scene);
        // parentEntity->m_layer = layer;
    }

    eachEntityFunc(parentEntity);

    for(auto& mesh : m_meshesData)
    {
        const auto meshEntity = mesh->addOnScene(scene);
        auto& meshEntityBaseInfo = registry->get<EntityBaseInfo>(meshEntity);

        meshEntityBaseInfo.setParent(parentEntity, *registry);

        meshFunc(parentEntity, meshEntity);
        eachEntityFunc(meshEntity);
    }

    for(auto& childNode : m_children)
    {
        auto childNodeEntity = childNode->addOnScene(scene, eachEntityFunc, meshFunc, false);
        auto& childEntityBaseInfo = registry->get<EntityBaseInfo>(childNodeEntity);
        childEntityBaseInfo.setParent(parentEntity, *registry);
        // parentEntity->addChild(childNodeEntity);
    }

    return parentEntity;
}

void SGCore::Node::addOnScene(const Ref<Scene>& scene,
                              const entity_func& eachEntityFunc,
                              const mesh_func& meshFunc) const noexcept
{
    addOnScene(scene, eachEntityFunc, meshFunc, true);
}

void SGCore::Node::addOnScene
(const Ref<Scene>& scene, const entity_func& eachEntityFunc) const noexcept
{
    addOnScene(scene, eachEntityFunc, [](ECS::entity_t, ECS::entity_t)
               {}
    );
}

void SGCore::Node::addOnScene
(const Ref<Scene>& scene, const mesh_func& meshFunc) const noexcept
{
    addOnScene(scene, [](ECS::entity_t)
               {}, meshFunc
    );
}

std::vector<SGCore::ECS::entity_t> SGCore::Node::addOnScene(const Ref<Scene>& scene) const noexcept
{
    std::vector<ECS::entity_t> allEntities;
    addOnScene(scene, [&allEntities](ECS::entity_t e) {
        allEntities.push_back(e);
    });

    return allEntities;
}

void SGCore::Node::doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept
{
    for(auto& meshData : m_meshesData)
    {
        meshData->doLoadFromBinaryFile(parentAssetManager);
    }

    for(auto& node : m_children)
    {
        node->doLoadFromBinaryFile(parentAssetManager);
    }
}

void SGCore::Node::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    for(auto& meshData : m_meshesData)
    {
        // resolving reference
        AssetManager::resolveAssetReference(updatedAssetManager, meshData);
    }
}

SGCore::AssetRef<SGCore::IMeshData> SGCore::Node::findMesh(const std::string& name) const noexcept
{
    for(size_t i = 0; i < m_meshesData.size(); ++i)
    {
        const auto& mesh = m_meshesData[i];

        if(mesh->m_name == name)
        {
            return mesh;
        }
    }

    for(size_t i = 0; i < m_children.size(); ++i)
    {
        auto meshData = m_children[i]->findMesh(name);
        if(meshData)
        {
            return meshData;
        }
    }

    return nullptr;
}
