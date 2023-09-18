#ifndef SUNGEARENGINE_CAMERACOMPONENT_H
#define SUNGEARENGINE_CAMERACOMPONENT_H

#include "SGCore/ECS/Transformations/Camera3DMovementSystem.h"

#include "IRenderingComponent.h"

namespace Core::ECS
{
    class CameraComponent : public IRenderingComponent
    {
        friend class Camera3DMovementSystem;

    private:
        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_CAMERACOMPONENT_H
