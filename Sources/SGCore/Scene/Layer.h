//
// Created by stuka on 24.10.2023.
//

#ifndef SUNGEARENGINE_LAYER_H
#define SUNGEARENGINE_LAYER_H

#include <SGCore/pch.h>
#include "SGCore/Annotations/Annotations.h"

// STANDARD LAYERS DEFINES

#define SG_LAYER_OPAQUE_NAME        "Opaque"
#define SG_LAYER_TRANSPARENT_NAME   "Transparent"

namespace SGCore
{
    template<typename>
    struct SerializerSpec;

    sg_struct()
    struct Layer
    {
        sg_serializer_as_friend(Layer)

        friend class Scene;
        friend class ISystem;

        sg_member()
        std::string m_name;

        sg_member()
        bool m_isOpaque = true;

    private:
        sg_member()
        size_t m_index = 0;
    };
}

#endif //SUNGEARENGINE_LAYER_H
