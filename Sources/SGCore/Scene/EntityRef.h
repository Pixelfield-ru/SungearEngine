//
// Created by stuka on 17.12.2024.
//

#ifndef SUNGEARENGINE_ENTITYREF_H
#define SUNGEARENGINE_ENTITYREF_H

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct EntityRef
    {
        Ref<ECS::entity_t> m_referencedEntity;

        EntityRef() = default;
        EntityRef(const ECS::entity_t& e) noexcept : m_referencedEntity(MakeRef<ECS::entity_t>(e)) { }
        EntityRef(const EntityRef&) noexcept = default;
        EntityRef(EntityRef&&) noexcept = default;

        EntityRef& operator=(const ECS::entity_t& e) noexcept
        {
            m_referencedEntity = MakeRef<ECS::entity_t>(e);

            return *this;
        }
        EntityRef& operator=(const EntityRef& e) noexcept = default;
        EntityRef& operator=(EntityRef&& e) noexcept = default;

        bool operator==(const EntityRef& other) const noexcept
        {
            return *m_referencedEntity == *other.m_referencedEntity;
        }

        bool operator==(const ECS::entity_t& other) const noexcept
        {
            return *m_referencedEntity == other;
        }
    };

    static bool operator==(const EntityRef& e0, const EntityRef& e1) noexcept
    {
        return *e0.m_referencedEntity == *e1.m_referencedEntity;
    }

    static bool operator==(const ECS::entity_t& e0, const EntityRef& e1) noexcept
    {
        return e0 == *e1.m_referencedEntity;
    }

    static bool operator==(const EntityRef& e0, const ECS::entity_t& e1) noexcept
    {
        return *e0.m_referencedEntity == e1;
    }

    struct EntityRefHash
    {
        using is_transparent = void;

        std::size_t operator()(const EntityRef& ref) const noexcept
        {
            return std::hash<ECS::entity_t>()(*ref.m_referencedEntity);
        }

        std::size_t operator()(const ECS::entity_t& entity) const noexcept
        {
            return std::hash<ECS::entity_t>()(entity);
        }
    };

    struct EntityRefEqual
    {
        using is_transparent = void;

        bool operator()(const EntityRef& lhs, const EntityRef& rhs) const noexcept
        {
            return *lhs.m_referencedEntity == *rhs.m_referencedEntity;
        }

        bool operator()(const EntityRef& lhs, const ECS::entity_t& rhs) const noexcept
        {
            return *lhs.m_referencedEntity == rhs;
        }

        bool operator()(const ECS::entity_t& lhs, const EntityRef& rhs) const noexcept
        {
            return lhs == *rhs.m_referencedEntity;
        }
    };
}

#endif //SUNGEARENGINE_ENTITYREF_H
