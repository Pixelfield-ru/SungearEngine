//
// Created by stuka on 27.11.2023.
//

#include "Node.h"

#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "glm/gtc/quaternion.hpp"

SGCore::Ref<SGCore::Entity> SGCore::Node::addOnScene(const SGCore::Ref<Scene>& scene, const std::string& layerName,
                                                     const SGCore::Node::EachEntityFunc& eachEntityFunc,
                                                     const SGCore::Node::MeshFunc& meshFunc,
                                                     const bool& rootAdd) noexcept
{
    auto layer = scene->getLayers().find(layerName)->second;

    if(!layer) return nullptr;

    Ref<Entity> nodeEntity = MakeRef<Entity>();

    auto nodeTransform = MakeRef<Transform>();
    nodeTransform->m_position = m_position;
    nodeTransform->m_rotation = glm::eulerAngles(m_rotationQuaternion);
    nodeTransform->m_scale = m_scale;

    nodeEntity->addComponent(nodeTransform);

    nodeEntity->setRawName(m_name);

    if(rootAdd)
    {
        scene->addEntity(nodeEntity, layerName);
    }
    else
    {
        nodeEntity->setParentScene(scene);
        nodeEntity->m_layer = layer;
    }

    scene->recacheEntity(nodeEntity);

    eachEntityFunc(nodeEntity);

    for(auto& mesh : m_meshesData)
    {
        Ref<Transform> meshedEntityTransformComponent = MakeRef<Transform>();

        Ref<Mesh> meshComponent = SGCore::MakeRef<Mesh>(scene->m_renderPipeline);
        // todo: make
        meshComponent->m_meshData = mesh;
        // meshComponent->addRequiredShaderPath("GeometryShader");

        Ref<Entity> meshedEntity = MakeRef<Entity>();
        meshedEntity->addComponent(meshedEntityTransformComponent);
        meshedEntity->addComponent(meshComponent);

        meshedEntity->m_layer = layer;
        nodeEntity->addChild(meshedEntity);

        scene->recacheEntity(meshedEntity);

        meshFunc(nodeEntity, meshedEntity);
        eachEntityFunc(meshedEntity);
    }

    for(auto& childNode : m_children)
    {
        auto childNodeEntity = childNode->addOnScene(scene, layerName, eachEntityFunc, meshFunc, false);
        nodeEntity->addChild(childNodeEntity);
    }

    return nodeEntity;
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
    addOnScene(scene, layerName, eachEntityFunc, [](const Ref<Entity>&, const Ref<Entity>&)
               {}
    );
}

void SGCore::Node::addOnScene
(const Ref<Scene>& scene, const std::string& layerName, const Node::MeshFunc& meshFunc) noexcept
{
    addOnScene(scene, layerName, [](const Ref<Entity>&)
               {}, meshFunc
    );
}

void SGCore::Node::addOnScene(const Ref<Scene>& scene, const std::string& layerName) noexcept
{
    addOnScene(scene, layerName,
               [](const Ref<Entity>&)
               {},
               [](const Ref<Entity>&, const Ref<Entity>&)
               {}
    );
}
