//
// Created by ilya on 14.04.24.
//

#include "EntitiesPool.h"

SGCore::EntitiesPool::EntitiesPool(const SGCore::Ref<SGCore::registry_t>& attachedRegistry)
{
    m_attachedRegistry = attachedRegistry;
}

SGCore::EntitiesPool::~EntitiesPool()
{
    clear();
}

SGCore::entity_t SGCore::EntitiesPool::pop(bool& isCreatedNew) noexcept
{
    std::lock_guard guard(m_mutex);

    isCreatedNew = false;
    
    if(m_freeEntities.empty())
    {
        if(auto lockedRegistry = m_attachedRegistry.lock())
        {
            auto entity = lockedRegistry->create();
            m_busyEntities.push_back(entity);

            isCreatedNew = true;

            return entity;
        }
        else
        {
            clear();
            
            return entt::null;
        }
    }
    else
    {
        entity_t entity = *m_freeEntities.rbegin();
        
        m_freeEntities.pop_back();
        
        m_busyEntities.push_back(entity);
        
        return entity;
    }
}

void SGCore::EntitiesPool::push(const entity_t& entity) noexcept
{
    std::lock_guard guard(m_mutex);

    auto it = std::remove(m_busyEntities.begin(), m_busyEntities.end(), entity);

    if(it != m_busyEntities.end())
    {
        m_freeEntities.push_back(entity);
    }
}

void SGCore::EntitiesPool::clear() noexcept
{
    std::lock_guard guard(m_mutex);

    auto lockedRegistry = m_attachedRegistry.lock();

    if(lockedRegistry)
    {
        for (const auto& e: m_freeEntities)
        {
            lockedRegistry->destroy(e);
        }

        for (const auto& e: m_busyEntities)
        {
            lockedRegistry->destroy(e);
        }
    }
    else
    {
        m_freeEntities.clear();
        m_busyEntities.clear();
    }
}

SGCore::Ref<SGCore::registry_t> SGCore::EntitiesPool::getAttachedRegistry() const noexcept
{
    return m_attachedRegistry.lock();
}

SGCore::EntitiesPool& SGCore::EntitiesPool::operator=(const SGCore::EntitiesPool& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    clear();

    m_attachedRegistry = other.m_attachedRegistry;

    for(const auto& e : other.m_freeEntities)
    {
        m_freeEntities.push_back(e);
    }

    for(const auto& e : other.m_busyEntities)
    {
        m_busyEntities.push_back(e);
    }

    return *this;
}
