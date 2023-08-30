//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_ISYSTEM_H
#define NATIVECORE_ISYSTEM_H

#include "Scene.h"
#include "SGCore/Patterns/Marker.h"

namespace Core::ECS
{
    struct SystemsFlags
    {
        // system`s functions will call not per entity
        static inline const std::uint16_t SGSF_NOT_PER_ENTITY = 1;
        static inline const std::uint16_t SGSF_PER_ENTITY = 1 << 1;
    };

    class ISystem : public Patterns::Marker<ISystem>
    {
    public:
        bool m_active = true;

        virtual void update(const std::shared_ptr<Scene>& scene) { };

        virtual void deltaUpdate(const std::shared_ptr<Scene>& scene, const double& deltaTime) { };

        virtual void update(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) { };

        virtual void deltaUpdate(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity,
                                 const double& deltaTime) { };
    };
}

#endif //NATIVECORE_ISYSTEM_H
