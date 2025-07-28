//
// Created by stuka on 28.07.2025.
//

#ifndef SUNGEARENGINE_PARTICLESEMITTER_H
#define SUNGEARENGINE_PARTICLESEMITTER_H

#include "IParticleController.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct ParticlesEmitter : ECS::Component<ParticlesEmitter, const ParticlesEmitter>
    {
        struct ParticleCreateResult
        {
            ECS::entity_t m_particleEntity = entt::null;
            bool m_isAbsoluteNew = false;
        };

        void processParticles(const ECS::registry_t& inRegistry) noexcept;

        ParticleCreateResult createParticle(ECS::registry_t& inRegistry) noexcept;
        void removeParticle(ECS::entity_t particleEntity, ECS::registry_t& inRegistry) noexcept;

        void addController(const Ref<IParticleController>& controller, ECS::registry_t& inRegistry) noexcept;
        void removeController(const Ref<IParticleController>& controller, ECS::registry_t& inRegistry) noexcept;

    private:
        std::vector<ECS::entity_t> m_particles;
        std::vector<ECS::entity_t> m_removedParticles;
        // all particles map
        std::unordered_map<ECS::entity_t, size_t> m_particlesMap;

        std::vector<Ref<IParticleController>> m_particleControllers;
    };
}

#endif // SUNGEARENGINE_PARTICLESCONTROLLER_H
