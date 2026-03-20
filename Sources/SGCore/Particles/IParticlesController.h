//
// Created by stuka on 28.07.2025.
//

#pragma once

#include <sgcore_export.h>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct ParticlesEmitter;

    struct SGCORE_EXPORT IParticlesController
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
