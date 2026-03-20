//
// Created by stuka on 19.12.2024.
//

#pragma once

#include <sgcore_export.h>
#include <vector>
#include "SGCore/Scene/EntityRef.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct SGCORE_EXPORT Pickable : ECS::Component<Pickable, const Pickable>
    {
        // refs to cameras entities
        std::vector<EntityRef> m_pickableForCameras;

        [[nodiscard]] bool isPickableForCamera(const ECS::entity_t& cameraEntity) const noexcept;
    };
}
