#pragma once

#ifndef SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H
#define SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct CameraMovement3DSystem : public ISystem
    {
        CameraMovement3DSystem();
        SG_COPY_CTOR(CameraMovement3DSystem);
        SG_MOVE_CTOR(CameraMovement3DSystem);

        void fixedUpdate(const Ref<Scene>& scene) final;
    };
}

#endif //SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H
