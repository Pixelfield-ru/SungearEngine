//
// Created by stuka on 28.12.2024.
//

#pragma once

#include <entt/entity/entity.hpp>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Serde/Defines.h"

sg_predeclare_serde()

namespace SGCore
{
    struct EntityBaseInfo;
}

namespace SGCore::ECS
{
    template<typename ComponentRegT, typename ConstComponentRegT, typename ComponentAccessor = decltype([](auto& component) -> decltype(auto) { return *component; })>
    struct Component
    {
        friend struct SGCore::EntityBaseInfo;

        template<typename>
        friend struct Registry;

        sg_serde_as_friend()

        /**
         * Type of component is registry.
         */
        using reg_t = ComponentRegT;
        /**
         * Cons type of component is registry.
         */
        using const_reg_t = ConstComponentRegT;

        using accessor_t = ComponentAccessor;

        virtual ~Component() = default;

        Component() noexcept = default;

        Component(const Component& other) noexcept
        {
            // do not copy m_entity field!!
            // ======================

            m_isActive = other.m_isActive;
        }

        Component(Component&& other) noexcept = default;

        virtual entity_t getThisEntity() const noexcept
        {
            return m_thisEntity;
        }

        virtual void setActive(bool active) noexcept
        {
            m_isActive = active;
        }

        bool isActive() const noexcept
        {
            return m_isActive;
        }

        Component& operator=(const Component& other)
        {
            // do not copy m_entity field!!
            // ======================

            m_isActive = other.m_isActive;

            return *this;
        }

        // moving entity
        Component& operator=(Component&& other) noexcept = default;

        template<typename NestedComponentT = ComponentRegT>
        requires(std::is_base_of_v<Component, NestedComponentT>)
        NestedComponentT& operator*() noexcept
        {
            return static_cast<NestedComponentT&>(*this);
        }

        template<typename NestedComponentT = ConstComponentRegT>
        requires(std::is_base_of_v<Component, NestedComponentT>)
        NestedComponentT& operator*() const noexcept
        {
            return static_cast<const NestedComponentT&>(*this);
        }

    private:
        bool m_isActive = true;

        entity_t m_thisEntity = entt::null;
    };
}
