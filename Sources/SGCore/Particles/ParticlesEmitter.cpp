//
// Created by stuka on 28.07.2025.
//

#include "ParticlesEmitter.h"

void SGCore::ParticlesEmitter::processParticles(const ECS::registry_t& inRegistry) const noexcept
{
    for(auto&& particleEntity : m_particlesEntities)
    {
        for(auto&& particleController : m_particleControllers)
        {
            particleController->processParticle(particleEntity, inRegistry);
        }
    }
}

void SGCore::ParticlesEmitter::addParticle(ECS::entity_t particleEntity, ECS::registry_t& inRegistry) noexcept
{
    m_particlesEntities.push_back(particleEntity);

    for(auto&& particleController : m_particleControllers)
    {
        particleController->onParticleAdd(particleEntity, inRegistry);
    }
}

void SGCore::ParticlesEmitter::removeParticle(ECS::entity_t particleEntity, ECS::registry_t& inRegistry) noexcept
{
    const auto cnt = std::erase(m_particlesEntities, particleEntity);

    if(cnt == 0) return;

    for(auto&& particleController : m_particleControllers)
    {
        particleController->onParticleRemove(particleEntity, inRegistry);
    }
}

void SGCore::ParticlesEmitter::addController(const Ref<IParticleController>& controller,
                                             ECS::registry_t& inRegistry) noexcept
{
    const auto cnt = std::erase(m_particleControllers, controller);

    if(cnt == 0) return;

    controller->onAddInEmitter(*this, inRegistry);
}

void SGCore::ParticlesEmitter::removeController(const Ref<IParticleController>& controller,
                                                ECS::registry_t& inRegistry) noexcept
{
    const auto cnt = std::erase(m_particleControllers, controller);

    if(cnt == 0) return;

    controller->onRemoveFromEmitter(*this, inRegistry);
}
