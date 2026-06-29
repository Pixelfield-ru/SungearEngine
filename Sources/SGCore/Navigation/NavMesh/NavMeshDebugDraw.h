//
// Created by stuka on 29.06.2026.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore::Navigation
{
    struct SGCORE_EXPORT NavMeshDebugDraw final : SGCore::ISystem
    {
        sg_implement_type_id(SGCore::Navigation::NavMeshDebugDraw)

        void update(double dt, double fixedDt) override;
    };
}
