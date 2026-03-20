#pragma once

#pragma once

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "TransformBase.h"

namespace SGCore
{
    struct SGCORE_EXPORT Controllables3DUpdater : public ISystem
    {
        sg_implement_type_id(Controllables3DUpdater, 16)

        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}
