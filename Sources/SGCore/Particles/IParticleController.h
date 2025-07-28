//
// Created by stuka on 28.07.2025.
//

#ifndef SUNGEARENGINE_IPARTICLECONTROLLER_H
#define SUNGEARENGINE_IPARTICLECONTROLLER_H

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct ParticlesEmitter;

    struct IParticleController
    {
        virtual ~IParticleController() = default;

        virtual void onParticleAdd(ECS::entity_t particleEntity, ECS::registry_t& inRegistry) noexcept { }
        virtual void onParticleRemove(ECS::entity_t particleEntity, ECS::registry_t& inRegistry) noexcept { }
        virtual void onAddInEmitter(ParticlesEmitter& particlesEmitter, ECS::registry_t& inRegistry) noexcept { }
        virtual void onRemoveFromEmitter(ParticlesEmitter& particlesEmitter, ECS::registry_t& inRegistry) noexcept { }

        virtual void processParticle(ECS::entity_t particleEntity, const ECS::registry_t& inRegistry) noexcept = 0;
    };
}

#endif // SUNGEARENGINE_IPARTICLECONTROLLER_H
