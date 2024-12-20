//
// Created by stuka on 19.12.2024.
//

#ifndef SUNGEARENGINE_PICKABLE_H
#define SUNGEARENGINE_PICKABLE_H

#include <vector>
#include "SGCore/Scene/EntityRef.h"

namespace SGCore
{
    struct Pickable
    {
        // refs to cameras entities
        std::vector<EntityRef> m_pickableForCameras;

        [[nodiscard]] bool isPickableForCamera(const entity_t& cameraEntity) const noexcept;
    };
}

#endif //SUNGEARENGINE_PICKABLE_H
