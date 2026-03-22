#pragma once

#pragma once

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "TransformBase.h"

namespace SGCore
{
    struct SGCORE_EXPORT Controllables3DUpdater : public ISystem
    {
        sg_implement_type_id(SGCore::Controllables3DUpdater)

        void fixedUpdate(double dt, double fixedDt) final;
    };
}
