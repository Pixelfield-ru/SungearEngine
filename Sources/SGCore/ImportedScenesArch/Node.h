#ifndef SUNGEARENGINE_NODE_H
#define SUNGEARENGINE_NODE_H

#include "IMeshData.h"
#include "glm/detail/type_quat.hpp"
#include "SGCore/ECS/Scene.h"
#include "SGCore/ECS/Rendering/Mesh.h"

namespace SGCore
{
    class Node
    {
    private:
        using MeshFunc = std::function<void(const Ref<Entity>& nodeEntity, const Ref<Entity>& meshEntity)>;
        using EachEntityFunc = std::function<void(const Ref<Entity>& nodeEntity)>;

    public:
        std::string m_name;

        std::list<std::shared_ptr<Node>> m_children;
        std::list<std::shared_ptr<IMeshData>> m_meshesData;

        glm::vec3 m_position { 1.0 };
        glm::quat m_rotationQuaternion = glm::identity<glm::quat>();
        glm::vec3 m_scale { 1.0 };

        void addOnScene(const Ref<Scene>& scene,
                        const std::string& layerName,
                        const EachEntityFunc& eachEntityFunc,
                        const MeshFunc& meshFunc) noexcept;
        void addOnScene(const Ref<Scene>& scene, const std::string& layerName, const EachEntityFunc& eachEntityFunc) noexcept;
        void addOnScene(const Ref<Scene>& scene, const std::string& layerName, const MeshFunc& meshFunc) noexcept;
        void addOnScene(const Ref<Scene>& scene, const std::string& layerName) noexcept;
    };
}

#endif //SUNGEARENGINE_NODE_H
