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
#include "SGCore/Transformations/TransformUtils.h"

SGCore::ECS::entity_t SGCore::Node::addOnScene(const SGCore::Ref<Scene>& scene,
                                               const std::string& layerName,
                                               const SGCore::Node::EachEntityFunc& eachEntityFunc,
                                               const SGCore::Node::MeshFunc& meshFunc,
                                               const bool& rootAdd) noexcept
{
    // todo: make layers
    // auto layer = scene->getLayers().find(layerName)->second;

    // if(!layer) return entt::null;

    auto registry = scene->getECSRegistry();

    ECS::entity_t parentEntity = registry->create();

    registry->emplace<Pickable>(parentEntity);
    auto& nodeBaseInfo = registry->get<EntityBaseInfo>(parentEntity);
    auto nodeTransform = registry->emplace<Transform>(parentEntity, MakeRef<Transform>());
    nodeTransform->m_ownTransform.m_position = m_position;
    // auto eulerRot = glm::eulerAngles(m_rotationQuaternion);
    nodeTransform->m_ownTransform.m_rotation = m_rotationQuaternion;
    nodeTransform->m_ownTransform.m_scale = m_scale;
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
        ECS::entity_t meshEntity = registry->create();

        registry->emplace<Pickable>(meshEntity);
        auto& meshEntityBaseInfo = registry->get<EntityBaseInfo>(meshEntity);
        auto meshTransform = registry->emplace<Transform>(meshEntity, MakeRef<Transform>());
        auto& meshEntityMesh = registry->emplace<Mesh>(meshEntity);
        // NOT STANDARD
        // auto cullable = registry->emplace<Ref<OctreeCullable>>(meshEntity, MakeRef<OctreeCullable>());
        meshEntityMesh.m_base.setMeshData(mesh);

        if(mesh->m_material->m_transparencyType == MaterialTransparencyType::MAT_BLEND ||
           mesh->m_material->m_transparencyType == MaterialTransparencyType::MAT_MASK)
        {
            registry->emplace<TransparentEntityTag>(meshEntity);
        }
        else
        {
            registry->emplace<OpaqueEntityTag>(meshEntity);
        }
        // meshEntityMesh.m_base.m_meshData->setData(mesh);

        // meshComponent->addRequiredShaderPath("GeometryShader");

        // meshEntityBaseInfo.m_layer = layer;

        meshEntityBaseInfo.setParent(parentEntity, *registry);
        meshEntityBaseInfo.setRawName(mesh->m_name);

        meshFunc(parentEntity, meshEntity);
        eachEntityFunc(meshEntity);
    }

    for(auto& childNode : m_children)
    {
        auto childNodeEntity = childNode->addOnScene(scene, layerName, eachEntityFunc, meshFunc, false);
        auto& childEntityBaseInfo = registry->get<EntityBaseInfo>(childNodeEntity);
        childEntityBaseInfo.setParent(parentEntity, *registry);
        // parentEntity->addChild(childNodeEntity);
    }

    return parentEntity;
}

void SGCore::Node::addOnScene(const Ref<Scene>& scene,
                              const std::string& layerName,
                              const Node::EachEntityFunc& eachEntityFunc,
                              const Node::MeshFunc& meshFunc) noexcept
{
    addOnScene(scene, layerName, eachEntityFunc, meshFunc, true);
}

void SGCore::Node::addOnScene
(const Ref<Scene>& scene, const std::string& layerName, const Node::EachEntityFunc& eachEntityFunc) noexcept
{
    addOnScene(scene, layerName, eachEntityFunc, [](const ECS::entity_t&, const ECS::entity_t&)
               {}
    );
}

void SGCore::Node::addOnScene
(const Ref<Scene>& scene, const std::string& layerName, const Node::MeshFunc& meshFunc) noexcept
{
    addOnScene(scene, layerName, [](const ECS::entity_t&)
               {}, meshFunc
    );
}

void SGCore::Node::addOnScene(const Ref<Scene>& scene, const std::string& layerName) noexcept
{
    addOnScene(scene, layerName,
               [](const ECS::entity_t&)
               {},
               [](const ECS::entity_t&, const ECS::entity_t&)
               {}
    );
}

void SGCore::Node::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
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
