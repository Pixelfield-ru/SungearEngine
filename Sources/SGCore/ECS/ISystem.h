//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_ISYSTEM_H
#define NATIVECORE_ISYSTEM_H

#include "Entity.h"
#include "Scene.h"
#include "SGCore/Patterns/Marker.h"

namespace Core::ECS
{
    struct SystemsFlags
    {
        static const std::uint8_t SGSF_NOT_PER_ENTITY = 1;
        static const std::uint8_t SGSF_PER_ENTITY = SGSF_NOT_PER_ENTITY << 1;
    };

    class ISystem : Patterns
    {
    public:
        std::uint8_t m_flags = SystemsFlags::SGSF_PER_ENTITY;

        bool m_active = true;

        virtual void update(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) = 0;

        virtual void deltaUpdate(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity, const double& deltaTime) = 0;
    };
}

#endif //NATIVECORE_ISYSTEM_H
