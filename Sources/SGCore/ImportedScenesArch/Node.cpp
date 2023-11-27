//
// Created by stuka on 27.11.2023.
//

#include "Node.h"

#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "glm/gtc/quaternion.hpp"

void SGCore::Node::addOnScene(const Ref<Scene>& scene,
                              const std::string& layerName,
                              const Node::EachEntityFunc& eachEntityFunc,
                              const Node::MeshFunc& meshFunc) noexcept
{
    Ref<Entity> nodeEntity = MakeRef<Entity>();

    auto nodeTransform = MakeRef<Transform>();
    nodeTransform->m_position = m_position;
    nodeTransform->m_rotation = glm::eulerAngles(m_rotationQuaternion);
    nodeTransform->m_scale = m_scale;

    nodeEntity->addComponent(nodeTransform);

    nodeEntity->m_name = m_name;

    scene->addEntity(nodeEntity, layerName);

    eachEntityFunc(nodeEntity);

    for(auto& mesh : m_meshesData)
    {
        Ref<Transform> meshedEntityTransformComponent = MakeRef<Transform>();

        Ref<Mesh> meshComponent = SGCore::MakeRef<Mesh>();
        meshComponent->m_meshData = mesh;

        Ref<Entity> meshedEntity = MakeRef<Entity>();
        meshedEntity->addComponent(meshedEntityTransformComponent);
        meshedEntity->addComponent(meshComponent);

        scene->addEntity(meshedEntity, layerName);

        meshFunc(nodeEntity, meshedEntity);
        eachEntityFunc(meshedEntity);
    }

    for(auto& node : m_children)
    {
        node->addOnScene(scene, layerName, eachEntityFunc, meshFunc);
    }
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
