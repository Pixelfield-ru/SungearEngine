#pragma once

#ifndef SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H
#define SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H

#include "SGCore/Scene/ISystem.h"
#include "SGECS/RegistryView.h"
#include "SGCore/Render/ICamera.h"
#include "Transform.h"

namespace SGCore
{
    struct CameraMovement3DSystem : public ISystem
    {
        void fixedUpdate(const Ref<Scene>& scene) final;

        void attachToScene(const Ref<Scene>& scene) final;

    private:
        SGECS::RegistryView<ICamera, Transform> m_cameras;
    };
}

#endif //SUNGEARENGINE_CAMERA3DMOVEMENTSYSTEM_H
