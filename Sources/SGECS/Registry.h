//
// Created by stuka on 04.01.2024.
//

#ifndef ECS_REGISTRY_H
#define ECS_REGISTRY_H

#include <cstddef>
#include <memory_resource>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <tsl/sparse_map.h>
#include "RegistryView.h"
#include "SGUtils/Utils.h"
#include "ComponentsAllocator.h"
#include "ComponentWrapper.h"
#include "SGUtils/Event.h"
#include "SGUtils/TypeInfo.h"

namespace SGECS
{
    class Registry
    {
    public:
        size_t createEntity() noexcept
        {
            return m_entitiesCount++;
        }

        template<typename ComponentT, typename... Args>
        ComponentT* emplace(const size_t& entity, Args&& ... args)
        {
            auto* componentWrapper = m_allocator.allocate<ComponentT>(std::forward<Args>(args)...);
            if(!componentWrapper) return nullptr;
            componentWrapper->m_entityID = entity;

            (*m_componentCreatedEvent)(m_allocator, entity, componentWrapper, SGUtils::TypeInfoUtils::getTypeInfo<ComponentT>().getID());

            return &(componentWrapper->m_component);
        }

        template<typename ComponentT>
        void remove(const size_t& entity, ComponentT* component)
        {
            m_allocator.deallocate(component);
            (*m_componentRemovedEvent)(m_allocator, entity, component, SGUtils::TypeInfoUtils::getTypeInfo<ComponentT>().getID());
        }

        void removeEntity(const size_t& entity)
        {
            (*m_entityRemovedEvent)(m_allocator, entity);
        }

        template<typename... ComponentsT>
        RegistryView<ComponentsT...> view()
        {
            RegistryView<ComponentsT...> registryView;

            (*m_registryCreatedEvent) += registryView.m_onRegistryCreated;
            (*m_entityRemovedEvent) += registryView.m_onEntityRemoved;
            (*m_componentCreatedEvent) += registryView.m_onComponentCreated;
            (*m_componentRemovedEvent) += registryView.m_onComponentRemoved;
            (*registryView.m_onRegistryCreated)(m_allocator);

            return registryView;
        }

        template<typename... ComponentsT>
        RegistryView<ComponentsT...> view(const std::initializer_list<size_t>& forEntities)
        {
            RegistryView<ComponentsT...> registryView(forEntities);

            (*m_registryCreatedEvent) += registryView.m_onRegistryCreated;
            (*m_entityRemovedEvent) += registryView.m_onEntityRemoved;
            (*m_componentCreatedEvent) += registryView.m_onComponentCreated;
            (*m_componentRemovedEvent) += registryView.m_onComponentRemoved;
            (*registryView.m_onRegistryCreated)(m_allocator);

            return registryView;
        }

    private:
        std::shared_ptr<SGUtils::Event<void(RegistryAllocator&)>> m_registryCreatedEvent
                = std::make_shared<SGUtils::Event<void(RegistryAllocator&)>>();

        std::shared_ptr<SGUtils::Event<void(RegistryAllocator&, const size_t&)>> m_entityRemovedEvent
                = std::make_shared<SGUtils::Event<void(RegistryAllocator&, const size_t&)>>();

        std::shared_ptr<SGUtils::Event<void(RegistryAllocator&, const size_t&, const void*, const size_t&)>> m_componentCreatedEvent =
                std::make_shared<SGUtils::Event<void(RegistryAllocator&, const size_t&, const void*, const size_t&)>>();

        std::shared_ptr<SGUtils::Event<void(RegistryAllocator&, const size_t&, const void*, const size_t&)>> m_componentRemovedEvent =
                std::make_shared<SGUtils::Event<void(RegistryAllocator&, const size_t&, const void*, const size_t&)>>();

        // todo: make normal
        size_t m_entitiesCount = 0;

        RegistryAllocator m_allocator;
    };
}

#endif //ECS_REGISTRY_H
