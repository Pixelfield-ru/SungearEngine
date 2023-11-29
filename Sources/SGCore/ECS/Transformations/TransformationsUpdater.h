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
        TransformationsUpdater();
        SG_COPY_CTOR(TransformationsUpdater);
        SG_MOVE_CTOR(TransformationsUpdater);

        void fixedUpdate(const Ref<Scene>& scene) final;
    };
}

#endif //NATIVECORE_TRANSFORMATIONSSYSTEM_H
