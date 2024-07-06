//
// Created by stuka on 25.01.2024.
//

#ifndef SUNGEARENGINE_ENTITYBASEINFO_H
#define SUNGEARENGINE_ENTITYBASEINFO_H

#include <SGCore/pch.h>

#include "SGCore/Render/LayeredFrameReceiver.h"

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Annotations/Annotations.h"

namespace SGCore
{
    struct Layer;

    sg_struct(extends = ["SGCore::UniqueNameWrapper"])
    // sizeof
    struct EntityBaseInfo : public UniqueNameWrapper
    {
        sg_serializer_as_friend(EntityBaseInfo)

        sg_member()
        entity_t m_parent = entt::null;

        Weak<Layer> m_layer;
        std::unordered_map<LayeredFrameReceiver*, Weak<PostProcessLayer>> m_postProcessLayers;
    };
}

#endif //SUNGEARENGINE_ENTITYBASEINFO_H
