//
// Created by ilya on 14.04.24.
//

#ifndef SUNGEARENGINE_ENTITIESPOOL_H
#define SUNGEARENGINE_ENTITIESPOOL_H

#include <SGCore/pch.h>

#include "SGUtils/EventListener.h"
#include "SGUtils/Event.h"

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct EntitiesPool
    {
        EntitiesPool() = default;
        explicit EntitiesPool(const Ref<registry_t>& attachedRegistry);
        ~EntitiesPool();
        
        entity_t pop(bool& isCreatedNew) noexcept;
        void push(const entity_t& entity) noexcept;

        void clear() noexcept;

        [[nodiscard]] Ref<registry_t> getAttachedRegistry() const noexcept;

        EntitiesPool& operator=(const EntitiesPool& other) noexcept;
    
    private:
        std::mutex m_mutex;
        
        Weak<registry_t> m_attachedRegistry;
        
        std::vector<entity_t> m_busyEntities;
        std::vector<entity_t> m_freeEntities;
    };
}

#endif // SUNGEARENGINE_ENTITIESPOOL_H
