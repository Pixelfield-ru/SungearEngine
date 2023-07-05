#pragma once

#ifndef SUNGEARENGINE_ECSWORLD_H
#define SUNGEARENGINE_ECSWORLD_H

#include <list>
#include <memory>

#include "IComponent.h"
#include "ISystem.h"
#include "Scene.h"

namespace Core::ECS
{
    class ECSWorld
    {
    private:
        static inline std::list<std::shared_ptr<ISystem>> m_systems;

    public:
        static void init() noexcept;

        static void update(const std::shared_ptr<Scene>& scene);
        static void deltaUpdate(const std::shared_ptr<Scene>& scene, const double& deltaTime);
    };
}

#endif //SUNGEARENGINE_ECSWORLD_H
