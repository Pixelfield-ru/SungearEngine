#ifndef SUNGEARENGINE_NODE_H
#define SUNGEARENGINE_NODE_H

#include "IMeshData.h"
#include "glm/detail/type_quat.hpp"

namespace SGCore
{
    class Node
    {
    public:
        std::string m_name;

        std::list<std::shared_ptr<Node>> m_children;
        std::list<std::shared_ptr<IMeshData>> m_meshesData;

        glm::vec3 m_position { 1.0 };
        glm::quat m_rotationQuaternion;
        glm::vec3 m_scale { 1.0 };
    };
}

#endif //SUNGEARENGINE_NODE_H
