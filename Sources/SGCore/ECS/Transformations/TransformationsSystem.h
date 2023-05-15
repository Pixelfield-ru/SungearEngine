//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMATIONSSYSTEM_H
#define NATIVECORE_TRANSFORMATIONSSYSTEM_H

#include "../ISystem.h"

namespace Core::ECS::Transformations
{
    class TransformationsSystem : ISystem
    {
    public:
        void update(const Core::ECS::Entity&) override;

        void deltaUpdate(const Core::ECS::Entity&, const float&) override;
    };
}

#endif //NATIVECORE_TRANSFORMATIONSSYSTEM_H
