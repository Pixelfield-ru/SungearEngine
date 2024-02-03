//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMATIONSSYSTEM_H
#define NATIVECORE_TRANSFORMATIONSSYSTEM_H

#include "SGCore/Scene/ISystem.h"
#include "SGUtils/Utils.h"

namespace SGCore
{
    class IMeshData;

    struct TransformBase;

    struct TransformationsUpdater : public ISystem
    {
        void fixedUpdate(const Ref<Scene>& scene) final;
    };
}

#endif //NATIVECORE_TRANSFORMATIONSSYSTEM_H
