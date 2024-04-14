//
// Created by stuka on 27.11.2023.
//

#include "Node.h"

#include "glm/gtc/quaternion.hpp"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/SpacePartitioning/CullableMesh.h"
#include "SGCore/Render/SpacePartitioning/OctreeCullableInfo.h"

SGCore::entity_t SGCore::Node::addOnScene(const SGCore::Ref<Scene>& scene,
                                      const std::string& layerName,
                                      const SGCore::Node::EachEntityFunc& eachEntityFunc,
                                      const SGCore::Node::MeshFunc& meshFunc,
                                      const bool& rootAdd) noexcept
{
    // todo: make layers
    // auto layer = scene->getLayers().find(layerName)->second;

    // if(!layer) return entt::null;

    auto& registry = scene->getECSRegistry();

    entity_t parentEntity = registry->create();

    EntityBaseInfo& nodeBaseInfo = registry->emplace<EntityBaseInfo>(parentEntity);
    Ref<Transform> nodeTransform = registry->emplace<Ref<Transform>>(parentEntity, MakeRef<Transform>());
    nodeTransform->m_ownTransform.m_position = m_position;
    // auto eulerRot = glm::eulerAngles(m_rotationQuaternion);
    nodeTransform->m_ownTransform.m_rotation = glm::degrees(glm::eulerAngles(m_rotationQuaternion));
    nodeTransform->m_ownTransform.m_scale = m_scale;

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
        entity_t meshEntity = registry->create();

        EntityBaseInfo& meshEntityBaseInfo = registry->emplace<EntityBaseInfo>(meshEntity);
        Ref<Transform>& meshTransform = registry->emplace<Ref<Transform>>(meshEntity, MakeRef<Transform>());
        Mesh& meshEntityMesh = registry->emplace<Mesh>(meshEntity);
        auto cullableMesh = registry->emplace<Ref<CullableMesh>>(meshEntity, MakeRef<CullableMesh>());
        auto cullableInfo = registry->emplace<Ref<OctreeCullableInfo>>(meshEntity, MakeRef<OctreeCullableInfo>());
        meshEntityMesh.m_base.m_meshData = mesh;
        // meshEntityMesh.m_base.m_meshData->setData(mesh);

        // meshComponent->addRequiredShaderPath("GeometryShader");

        // meshEntityBaseInfo.m_layer = layer;

        meshEntityBaseInfo.m_parent = parentEntity;

        meshFunc(parentEntity, meshEntity);
        eachEntityFunc(meshEntity);
    }

    for(auto& childNode : m_children)
    {
        auto childNodeEntity = childNode->addOnScene(scene, layerName, eachEntityFunc, meshFunc, false);
        EntityBaseInfo& childEntityBaseInfo = registry->get<EntityBaseInfo>(childNodeEntity);
        childEntityBaseInfo.m_parent = parentEntity;
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
    addOnScene(scene, layerName, eachEntityFunc, [](const entity_t&, const entity_t&)
               {}
    );
}

void SGCore::Node::addOnScene
(const Ref<Scene>& scene, const std::string& layerName, const Node::MeshFunc& meshFunc) noexcept
{
    addOnScene(scene, layerName, [](const entity_t&)
               {}, meshFunc
    );
}

void SGCore::Node::addOnScene(const Ref<Scene>& scene, const std::string& layerName) noexcept
{
    addOnScene(scene, layerName,
               [](const entity_t&)
               {},
               [](const entity_t&, const entity_t&)
               {}
    );
}