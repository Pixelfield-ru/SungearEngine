//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMATIONSSYSTEM_H
#define NATIVECORE_TRANSFORMATIONSSYSTEM_H

#include "SGCore/Scene/ISystem.h"
#include "SGUtils/Utils.h"
#include "SGECS/RegistryView.h"

namespace SGCore
{
    class IMeshData;

    struct Transform;

    struct TransformationsUpdater : public ISystem
    {
        void fixedUpdate(const Ref<Scene>& scene) final;

        void attachToScene(const Ref<Scene>& scene) noexcept final;

    private:
        SGECS::RegistryView<Transform> m_transforms;
    };
}

#endif //NATIVECORE_TRANSFORMATIONSSYSTEM_H
