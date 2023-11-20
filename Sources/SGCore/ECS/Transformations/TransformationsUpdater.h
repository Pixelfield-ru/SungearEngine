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

    class TransformationsUpdater : public ISystem
    {
        SG_DECLARE_SINGLETON(TransformationsUpdater)

    public:
        void fixedUpdate(const Ref<Scene>& scene) final;

        void cacheEntity(const Ref<Entity>& entity) final;
    };
}

#endif //NATIVECORE_TRANSFORMATIONSSYSTEM_H
