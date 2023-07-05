//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_ISYSTEM_H
#define NATIVECORE_ISYSTEM_H

#include "Entity.h"
#include "Scene.h"

namespace Core::ECS
{
    class ISystem
    {
    public:
        bool m_active = true;

        virtual void update(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) = 0;

        virtual void deltaUpdate(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity, const double& deltaTime) = 0;
    };
}

#endif //NATIVECORE_ISYSTEM_H
