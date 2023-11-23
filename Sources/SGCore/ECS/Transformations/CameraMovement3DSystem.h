#pragma once

#ifndef SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H
#define SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct CameraMovement3DSystem : public ISystem
    {
        void fixedUpdate(const Ref<Scene>& scene) final;

        SG_CUSTOM_CTOR_SINGLETON(CameraMovement3DSystem)
        SG_COPY_SINGLETON(CameraMovement3DSystem)
        SG_MOVE_SINGLETON(CameraMovement3DSystem)
    };
}

#endif //SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H
