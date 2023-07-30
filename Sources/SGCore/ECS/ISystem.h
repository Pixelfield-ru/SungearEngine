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
        static inline const std::uint16_t SGSF_NOT_PER_ENTITY = 0 << 1;
        static inline const std::uint16_t SGSF_PER_ENTITY = 0 << 2;
    };

    class ISystem : public Patterns::Marker<ISystem>
    {
    public:
        bool m_active = true;

        virtual void update(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) = 0;

        virtual void deltaUpdate(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity,
                                 const double& deltaTime) = 0;

    private:
        void init() override
        {
            addFlag(SystemsFlags::SGSF_PER_ENTITY);
        }
    };
}

#endif //NATIVECORE_ISYSTEM_H
