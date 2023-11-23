//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMATIONSSYSTEM_H
#define NATIVECORE_TRANSFORMATIONSSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    class IMeshData;

    class Transform;

    struct TransformationsUpdater : public ISystem
    {
        void fixedUpdate(const Ref<Scene>& scene) final;

        SG_CUSTOM_CTOR_SINGLETON(TransformationsUpdater)
        SG_COPY_SINGLETON(TransformationsUpdater)
        SG_MOVE_SINGLETON(TransformationsUpdater)
    };
}

#endif //NATIVECORE_TRANSFORMATIONSSYSTEM_H
