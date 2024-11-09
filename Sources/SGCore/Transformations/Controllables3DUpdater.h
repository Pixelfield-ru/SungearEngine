#pragma once

#ifndef SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H
#define SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "TransformBase.h"

namespace SGCore
{
    struct Controllables3DUpdater : public ISystem
    {
        sg_implement_type_id(Controllables3DUpdater, 16)

        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}

#endif //SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H
