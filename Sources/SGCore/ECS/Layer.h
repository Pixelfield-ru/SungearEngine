//
// Created by stuka on 24.10.2023.
//

#ifndef SUNGEARENGINE_LAYER_H
#define SUNGEARENGINE_LAYER_H

#include <list>

#include "Entity.h"

// STANDARD LAYERS DEFINES

#define SG_LAYER_OPAQUE_NAME        "Opaque"
#define SG_LAYER_TRANSPARENT_NAME   "Transparent"

namespace SGCore
{
    class Layer
    {
        friend class Scene;
        friend class ISystem;
        friend class LayersComparator;
        friend struct CachedEntitiesComparator;

    public:
        std::string m_name;

        bool m_isOpaque = true;

    private:
        size_t m_index = 0;
    };

    struct LayersComparator
    {
        bool operator()(const Ref<Layer>& l0, const Ref<Layer>& l1) const
        {
            return l0->m_index > l1->m_index;
        }
    };
}

#endif //SUNGEARENGINE_LAYER_H