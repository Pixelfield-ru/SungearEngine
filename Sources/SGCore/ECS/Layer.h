//
// Created by stuka on 24.10.2023.
//

#ifndef SUNGEARENGINE_LAYER_H
#define SUNGEARENGINE_LAYER_H

#include <list>

#include "Entity.h"
#include "SGCore/Graphics/API/MarkedShader.h"

// STANDARD LAYERS DEFINES

#define SG_LAYER_OPAQUE_NAME        "Opaque"
#define SG_LAYER_TRANSPARENT_NAME   "Transparent"

namespace Core::ECS
{
    class Layer
    {
        friend class Scene;
        friend class ISystem;
        friend class LayersComparator;

    public:
        std::list<std::shared_ptr<Entity>> m_entities;

        std::shared_ptr<Graphics::MarkedShader> m_markedShader = std::make_shared<Graphics::MarkedShader>();

        std::string m_name;

        bool m_isOpaque = true;
        bool m_useCustomShader = false;

    private:
        size_t m_index = 0;
    };

    struct LayersComparator
    {
        bool operator()(const std::shared_ptr<Layer>& l0, const std::shared_ptr<Layer>& l1) const
        {
            return l0->m_index > l1->m_index;
        }
    };
}

#endif //SUNGEARENGINE_LAYER_H
