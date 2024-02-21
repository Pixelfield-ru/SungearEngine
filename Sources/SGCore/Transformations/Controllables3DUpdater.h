#pragma once

#ifndef SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H
#define SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Render/Camera.h"
#include "TransformBase.h"

namespace SGCore
{
    struct Controllables3DUpdater : public ISystem
    {
        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}

#endif //SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H
