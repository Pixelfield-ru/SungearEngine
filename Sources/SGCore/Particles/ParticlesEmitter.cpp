//
// Created by stuka on 28.07.2025.
//

#include "ParticlesEmitter.h"

#include "SGCore/ECS/Registry.h"

void SGCore::ParticlesEmitter::processParticles(const ECS::registry_t& inRegistry) noexcept
{
    for(auto&& particle : m_particles)
    {
        const auto& particleBaseInfo = inRegistry.get<EntityBaseInfo>(particle);

        if(!particleBaseInfo.m_isActive) continue;

        for(auto&& particleController : m_particleControllers)
        {
            particleController->processParticle(*this, particle, inRegistry);
        }
    }
}

SGCore::ParticlesEmitter::ParticleCreateResult SGCore::ParticlesEmitter::createParticle(ECS::registry_t& inRegistry) noexcept
{
    ECS::entity_t particleEntity = entt::null;
    bool isParticleAbsoluteNew = false;

    if(m_removedParticles.empty())
    {
        particleEntity = inRegistry.create();

        m_particles.push_back(particleEntity);

        m_particlesMap[particleEntity] = m_particles.size() - 1;

        isParticleAbsoluteNew = true;
    }
    else
    {
        particleEntity = m_removedParticles.back();

        auto& particle = m_particles[m_particlesMap[particleEntity]];
        auto& particleBaseInfo = inRegistry.get<EntityBaseInfo>(particle);
        particleBaseInfo.setActiveRecursive(true, inRegistry);

        m_removedParticles.pop_back();
    }


    for(auto&& particleController : m_particleControllers)
    {
        particleController->onParticleAdd(*this, particleEntity, inRegistry, isParticleAbsoluteNew);
    }

    return {
        .m_particleEntity = particleEntity,
        .m_isAbsoluteNew = isParticleAbsoluteNew
    };
}

void SGCore::ParticlesEmitter::removeParticle(ECS::entity_t particleEntity, ECS::registry_t& inRegistry) noexcept
{
    auto mapIt = m_particlesMap.find(particleEntity);
    if(mapIt == m_particlesMap.end()) return;

    m_removedParticles.push_back(mapIt->first);

    auto& particleBaseInfo = inRegistry.get<EntityBaseInfo>(particleEntity);
    particleBaseInfo.setActiveRecursive(false, inRegistry);

    for(auto&& particleController : m_particleControllers)
    {
        particleController->onParticleRemove(*this, particleEntity, inRegistry);
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
