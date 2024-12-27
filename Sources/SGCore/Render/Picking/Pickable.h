//
// Created by stuka on 19.12.2024.
//

#ifndef SUNGEARENGINE_PICKABLE_H
#define SUNGEARENGINE_PICKABLE_H

#include <sgcore_export.h>
#include <vector>
#include "SGCore/Scene/EntityRef.h"

namespace SGCore
{
    struct SGCORE_EXPORT Pickable
    {
        // refs to cameras entities
        std::vector<EntityRef> m_pickableForCameras;

        [[nodiscard]] bool isPickableForCamera(const entity_t& cameraEntity) const noexcept;
    };
}

#endif //SUNGEARENGINE_PICKABLE_H
