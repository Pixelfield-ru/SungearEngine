//
// Created by stuka on 28.07.2025.
//

#ifndef SUNGEARENGINE_IPARTICLECONTROLLER_H
#define SUNGEARENGINE_IPARTICLECONTROLLER_H

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct ParticlesEmitter;

    struct IParticlesController
    {
        bool m_isActive = true;

        virtual ~IParticlesController() = default;

        virtual void onParticleAdd(ParticlesEmitter& particlesEmitter, ECS::entity_t particleEntity, ECS::registry_t& inRegistry, bool isAbsoluteNew) noexcept { }
        virtual void onParticleRemove(ParticlesEmitter& particlesEmitter, ECS::entity_t particleEntity, ECS::registry_t& inRegistry) noexcept { }

        virtual void onAddInEmitter(ParticlesEmitter& particlesEmitter, ECS::registry_t& inRegistry) noexcept { }
        virtual void onRemoveFromEmitter(ParticlesEmitter& particlesEmitter, ECS::registry_t& inRegistry) noexcept { }

        virtual void processParticle(ParticlesEmitter& particlesEmitter, ECS::entity_t particleEntity, const ECS::registry_t& inRegistry) noexcept = 0;
    };
}

#endif // SUNGEARENGINE_IPARTICLECONTROLLER_H
