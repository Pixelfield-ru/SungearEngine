#ifndef SUNGEARENGINE_NODE_H
#define SUNGEARENGINE_NODE_H

#include "IMesh.h"

namespace Core::ImportedScene
{
    class Node
    {
    public:
        std::string m_name;

        std::list<std::shared_ptr<Node>> m_children;
        std::list<std::shared_ptr<IMesh>> m_meshes;
    };
}

#endif //SUNGEARENGINE_NODE_H
